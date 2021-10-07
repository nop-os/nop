#include <arch/i586.h>
#include <nop/send.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <string.h>

#include "map.h"

int hand;

int mode;
int extra;

int caps_lock;

uint8_t keys[384];
int progs[64];

void keyb_hand(i586_regs_t *regs, int id);

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type == nop_type("INIT")) {
    dbg_init(0x03F8);
    
    i586_outb(0xAD, 0x64); // disable port 1
    i586_outb(0xA7, 0x64); // disable port 2
    
    i586_inb(0x60); // read buffer
    
    dbg_infof("keyb: requesting current configuration\n");
    i586_outb(0x20, 0x64); // request curr. conf.
    
    while (!(i586_inb(0x64) & 1));
    uint8_t conf = i586_inb(0x60);
    
    dbg_infof("keyb: current configuration: 0x%02X\n", conf);
    
    conf |=  (1 << 0); // enable port 1 int.
    conf &= ~(1 << 1); // disable port 2 int.
    
    conf &= ~(1 << 6); // no idea of what this does
    
    dbg_infof("keyb: setting configuration: 0x%02X\n", conf);
    i586_outb(0x60, 0x64); // set conf.
    
    while (i586_inb(0x64) & 2);
    i586_outb(conf, 0x60);
    
    dbg_infof("keyb: setting scan code set 2\n");
    
    while (i586_inb(0x64) & 2);
    i586_outb(0xF0, 0x60); // set scan code set
    
    i586_inb(0x60); // clear buffer
    
    while (i586_inb(0x64) & 2);
    i586_outb(0x02, 0x60);
    
    i586_inb(0x60); // clear buffer
    
    dbg_infof("keyb: enabling port 1\n");
    i586_outb(0xAE, 0x64); // enable port 1
    
    hand = (int)(nop_send(0, "HOOK", 0x21, 0, 0));
    
    if (!hand) {
      dbg_failf("keyb: cannot hook up handler\n");
      dbg_panic();
    }
    
    mode = 0;
    extra = 0;
    
    memset(progs, 0, 64 * sizeof(int));
    caps_lock = 0;
    
    memset(keys, 0, 384);
    dbg_infof("keyb: done!\n");
  } else if (type == nop_type("TRIG")) {
    keyb_hand((void *)(data_1), data_2);
  } else if (type == nop_type("HOOK")) {
    for (int i = 0; i < 64; i++) {
      if (!progs[i]) {
        progs[i] = id;
        break;
      }
    }
  } else if (type == nop_type("RELE") || type == nop_type("DEAD")) {
    for (int i = 0; i < 64; i++) {
      if (progs[i] == data_1) {
        progs[i] = 0;
      }
    }
  } else if (type == nop_type("REQU")) {
    if (data_1 && data_1 < 384) {
      return keys[data_1];
    }
  }
  
  return 0;
}

void keyb_hand(i586_regs_t *regs, int id) {
  uint8_t code = i586_inb(0x60);
  
  if (code == 0xF0) {
    mode = 1;
    return;
  }
  
  if (code == 0xE0) {
    extra = 1;
    return;
  }
  
  if (code < 0x84) {
    int value = keyb_map[code];
    
    if (extra && (value == KEY_L_ALT || value == KEY_L_SHIFT || value == KEY_L_CTRL)) {
      value += (KEY_R_ALT - KEY_L_ALT);
    }
    
    keys[value] = 1 - mode;
    
    if (!mode) {
      if (value == KEY_CAPSLOCK) {
        caps_lock = 1 - caps_lock;
      } else {
        int alpha_case = (keys[KEY_L_SHIFT] | keys[KEY_R_SHIFT]) != caps_lock;
        int extra_case = keys[KEY_L_SHIFT] | keys[KEY_R_SHIFT];
        
        int is_alpha = value >= 'a' && value <= 'z';
        
        if (((alpha_case && is_alpha) || (extra_case && !is_alpha)) && value < 127) {
          value = keyb_map[code + 0x0084];
        } else if (keys[KEY_R_ALT]) {
          value = keyb_map[code + 0x0108];
        }
        
        if (value && value < 127) {
          // nop_send(3, "WRIT", (uint32_t)(&value), 1, 0);
          
          for (int i = 0; i < 64; i++) {
            if (progs[i]) {
              nop_send(progs[i], "KEYB", value, 0, 0);
            }
          }
        }
      }
    }
    
    extra = 0;
    mode = 0;
  }
}
