#include <arch/i586.h>
#include <nop/type.h>
#include <nop/idt.h>

#define PS2_KEY_NONE      0
#define PS2_KEY_F1        256
#define PS2_KEY_F2        257
#define PS2_KEY_F3        258
#define PS2_KEY_F4        259
#define PS2_KEY_F5        260
#define PS2_KEY_F6        261
#define PS2_KEY_F7        262
#define PS2_KEY_F8        263
#define PS2_KEY_F9        264
#define PS2_KEY_F10       265
#define PS2_KEY_F11       266
#define PS2_KEY_F12       267
#define PS2_KEY_L_ALT     268
#define PS2_KEY_L_SHIFT   269
#define PS2_KEY_L_CTRL    270
#define PS2_KEY_R_ALT     271
#define PS2_KEY_R_SHIFT   272
#define PS2_KEY_R_CTRL    273
#define PS2_KEY_CAPSLOCK  274
#define PS2_KEY_NUMLOCK   275
#define PS2_KEY_SCRLOCK   276
#define PS2_KEY_PAD_0     277
#define PS2_KEY_PAD_1     278
#define PS2_KEY_PAD_2     279
#define PS2_KEY_PAD_3     280
#define PS2_KEY_PAD_4     281
#define PS2_KEY_PAD_5     282
#define PS2_KEY_PAD_6     283
#define PS2_KEY_PAD_7     284
#define PS2_KEY_PAD_8     285
#define PS2_KEY_PAD_9     286
#define PS2_KEY_ESCAPE    '\x1B'
#define PS2_KEY_BACKSPACE '\b'
#define PS2_KEY_ENTER     '\n'
#define PS2_KEY_TAB       '\t'

extern int ps2_keyb_hand;
extern int ps2_mouse_hand;

extern int ps2_mode;
extern int ps2_extra;

extern int ps2_caps_lock;

extern int ps2_raw_mode;

extern uint16_t *ps2_keymap;
extern uint8_t ps2_keys[];

extern uint16_t ps2_queue[];
extern int ps2_read_head;
extern int ps2_write_head;

void ps2_init(const char *path);
uint16_t ps2_read(void);

void ps2_keyb(i586_regs_t *regs);
void ps2_mouse(i586_regs_t *regs);
