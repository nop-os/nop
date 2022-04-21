#include <arch/i586.h>
#include <nop/alloc.h>
#include <nop/call.h>
#include <nop/term.h>
#include <nop/file.h>
#include <nop/time.h>
#include <nop/type.h>
#include <nop/idt.h>
#include <nop/ps2.h>

int ps2_keyb_hand = 0;
int ps2_mouse_hand = 0;

int ps2_mode = 0;
int ps2_extra = 0;

int ps2_caps_lock = 0;

int ps2_cook_mode = 1;
int ps2_echo_mode = 1;

uint16_t *ps2_keymap = NULL;
uint8_t ps2_keys[384] = {0};

uint16_t ps2_queue[PS2_BUFFER_SIZE];
int ps2_read_head = 0;
int ps2_write_head = 0;

int ps2_enter_head = 0;

int ps2_wait_in(void) {
  uint64_t start = time_read();
  
  while (!(i586_inb(0x64) & 1)) {
    if (time_read() - start > 10 * 1024) return 1;
    i586_wait();
  }
  
  return 0;
}

void ps2_wait_out(void) {
  while (i586_inb(0x64) & 2);
}

void ps2_ack(void) {
  while (i586_inb(0x60) != 0xFA);
}

void ps2_clr(void) {
  while (i586_inb(0x64) & 1) {
    i586_inb(0x60);
  }
}

void ps2_dev_cmd(int id, uint8_t cmd) {
  if (id) {
    ps2_wait_out();
    i586_outb(0xD4, 0x64);
  }
  
  ps2_wait_out();
  i586_outb(cmd, 0x60);
  
  ps2_wait_out();
  ps2_ack();
}

void ps2_ctl_cmd(uint8_t cmd) {
  ps2_wait_out();
  i586_outb(cmd, 0x64);
}

void ps2_ctl_out(uint8_t val) {
  ps2_wait_out();
  i586_outb(val, 0x60);
}

uint16_t ps2_ctl_in(void) {
  if (ps2_wait_in()) return 0xFFFF;
  return i586_inb(0x60);
}

void ps2_dev_out(int id, uint8_t val) {
  if (id) {
    ps2_wait_out();
    i586_outb(0xD4, 0x64);
  }
  
  ps2_ctl_out(val);
  ps2_ack();
}

uint16_t ps2_dev_in(void) {
  return ps2_ctl_in();
}

void ps2_init(const char *path) {
  int file = file_open(path);
  ps2_keymap = malloc(0x0318);
  
  if (!file) {
    term_failf("cannot open keymap\n");
    term_panic();
  }
  
  if (!file_read(file, ps2_keymap, 0x0318)) {
    term_failf("cannot read keymap\n");
    term_panic();
  }
  
  file_close(file, 0);
  
  ps2_ctl_cmd(0xAD); // disable port 1
  ps2_ctl_cmd(0xA7); // disable port 2
  
  term_infof("requesting current configuration\n");
  ps2_ctl_cmd(0x20); // request curr. conf.
  uint8_t conf = ps2_ctl_in();
  
  term_infof("current configuration: 0x%02X\n", conf);
  
  conf |=  (1 << 0); // enable port 1 int.
  conf |=  (1 << 1); // enable port 2 int.
  conf &= ~(1 << 4); // enable port 1 clk.
  conf &= ~(1 << 5); // enable port 2 clk.
  conf &= ~(1 << 6); // no idea of what this does
  
  term_infof("setting configuration: 0x%02X\n", conf);
  
  ps2_ctl_cmd(0x60);
  ps2_ctl_out(conf);
  
  term_infof("setting scan code set 2\n");
  
  ps2_dev_cmd(0, 0xF0);
  ps2_dev_out(0, 0x02);
  
  term_infof("setting mouse sampling rate\n");
  
  ps2_dev_cmd(1, 0xF6);
  ps2_dev_cmd(1, 0xF3);
  ps2_dev_out(1, 0x14);
  ps2_dev_cmd(1, 0xF4);
  
  term_infof("setting up interrupts\n");
  
  ps2_keyb_hand = idt_add(ps2_keyb, IDT_KEYB);
  ps2_mouse_hand = idt_add(ps2_mouse, IDT_MOUSE);
  
  term_infof("enabling ports\n");
  
  ps2_ctl_cmd(0xAE); // enable port 1
  ps2_ctl_cmd(0xA8); // enable port 2
  
  ps2_clr();
  term_donef("initialized PS/2 controllers\n");
}

uint16_t ps2_read(void) {
  if (ps2_read_head != ps2_write_head) {
    uint16_t value = ps2_queue[ps2_read_head++];
    ps2_read_head %= PS2_BUFFER_SIZE;
    
    return value;
  }
  
  return 0;
}

void ps2_keyb(i586_regs_t *regs) {
  uint16_t code = ps2_dev_in();
  if (code == 0xFFFF) return;
  
  if (code == 0xF0) {
    ps2_mode = 1;
    return;
  }
  
  if (code == 0xE0) {
    ps2_extra = 1;
    return;
  }
  
  if (code < 0x84) {
    int value = ps2_keymap[code];
    
    if (ps2_extra && (value == PS2_KEY_L_ALT || value == PS2_KEY_L_SHIFT || value == PS2_KEY_L_CTRL)) {
      value += (PS2_KEY_R_ALT - PS2_KEY_L_ALT);
    }
    
    ps2_keys[value] = 1 - ps2_mode;
    
    if (!ps2_mode) {
      if (value == PS2_KEY_CAPSLOCK) {
        ps2_caps_lock = 1 - ps2_caps_lock;
      } else {
        int alpha_case = (ps2_keys[PS2_KEY_L_SHIFT] | ps2_keys[PS2_KEY_R_SHIFT]) != ps2_caps_lock;
        int extra_case = ps2_keys[PS2_KEY_L_SHIFT] | ps2_keys[PS2_KEY_R_SHIFT];
        
        int ctrl_key = ps2_keys[PS2_KEY_L_CTRL] | ps2_keys[PS2_KEY_R_CTRL];
        int is_alpha = value >= 'a' && value <= 'z';
        
        if (((alpha_case && is_alpha) || (extra_case && !is_alpha)) && value < 127) {
          value = ps2_keymap[code + 0x0084];
        } else if (ps2_keys[PS2_KEY_R_ALT]) {
          value = ps2_keymap[code + 0x0108];
        }
        
        if (value && value <= 127) {
          if (ctrl_key && value <= 127) {
            value = value % 32;
          }
          
          if (value == '\b' && ps2_cook_mode) {
            if (ps2_enter_head != ps2_write_head) {
              ps2_enter_head += (PS2_BUFFER_SIZE - 1);
              ps2_enter_head %= PS2_BUFFER_SIZE;
              
              if (ps2_echo_mode) {
                call_kernel(term_putchr, value);
              }
            }
          } else {
            ps2_queue[ps2_enter_head++] = value;
            ps2_enter_head %= PS2_BUFFER_SIZE;
            
            if (ps2_echo_mode) {
              call_kernel(term_putchr, value);
            }
            
            if (value == '\n' || !ps2_cook_mode) {
              ps2_write_head = ps2_enter_head;
            }
          }
        }
      }
    }
    
    ps2_extra = 0;
    ps2_mode = 0;
  }
}

void ps2_mouse(i586_regs_t *regs) {
  uint16_t data_1 = ps2_dev_in();
  if (data_1 == 0xFFFF) return;
  
  uint16_t data_2 = ps2_dev_in();
  if (data_2 == 0xFFFF) return;
  
  uint16_t data_3 = ps2_dev_in();
  if (data_3 == 0xFFFF) return;
  
  int delta_x = data_2;
  if (data_1 & 0x10) delta_x -= 256;
  
  int delta_y = data_3;
  if (data_1 & 0x20) delta_y -= 256;
  
  ps2_clr(); // what?
}
