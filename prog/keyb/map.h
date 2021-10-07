#ifndef __MAP_H__
#define __MAP_H__

#define KEY_NONE      0
#define KEY_F1        256
#define KEY_F2        257
#define KEY_F3        258
#define KEY_F4        259
#define KEY_F5        260
#define KEY_F6        261
#define KEY_F7        262
#define KEY_F8        263
#define KEY_F9        264
#define KEY_F10       265
#define KEY_F11       266
#define KEY_F12       267
#define KEY_L_ALT     268
#define KEY_L_SHIFT   269
#define KEY_L_CTRL    270
#define KEY_R_ALT     271
#define KEY_R_SHIFT   272
#define KEY_R_CTRL    273
#define KEY_CAPSLOCK  274
#define KEY_NUMLOCK   275
#define KEY_SCRLOCK   276
#define KEY_ESCAPE    277
#define KEY_BACKSPACE '\b'
#define KEY_ENTER     '\n'
#define KEY_TAB       '\t'

const int keyb_map_en[] = {
  // default
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     '`',           KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'q',         '1',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'z',           's',
  'a',          'w',         '2',           KEY_NONE,
  KEY_NONE,     'c',         'x',           'd',
  'e',          '4',         '3',           KEY_NONE,
  KEY_NONE,     ' ',         'v',           'f',
  't',          'r',         '5',           KEY_NONE,
  KEY_NONE,     'n',         'b',           'h',
  'g',          'y',         '6',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'm',           'j',
  'u',          '7',         '8',           KEY_NONE,
  KEY_NONE,     ',',         'k',           'i',
  'o',          '0',         '9',           KEY_NONE,
  KEY_NONE,     '.',         '/',           'l',
  ';',          'p',         '-',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    '\'',          KEY_NONE,
  '[',          '=',         KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     ']',
  KEY_NONE,     '\\',        KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7,
  
  // left/right shift on
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     '~',           KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'Q',         '!',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'Z',           'S',
  'A',          'W',         '@',           KEY_NONE,
  KEY_NONE,     'C',         'X',           'D',
  'E',          '$',         '#',           KEY_NONE,
  KEY_NONE,     ' ',         'V',           'F',
  'T',          'R',         '%',           KEY_NONE,
  KEY_NONE,     'N',         'B',           'H',
  'G',          'Y',         '^',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'M',           'J',
  'U',          '&',         '*',           KEY_NONE,
  KEY_NONE,     '<',         'K',           'I',
  'O',          ')',         '(',           KEY_NONE,
  KEY_NONE,     '>',         '?',           'L',
  ':',          'P',         '_',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    '|',           KEY_NONE,
  '{',          '+',         KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     '}',
  KEY_NONE,     '\\',        KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7,
  
  // right alt on
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     '`',           KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'q',         '1',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'z',           's',
  'a',          'w',         '2',           KEY_NONE,
  KEY_NONE,     'c',         'x',           'd',
  'e',          '4',         '3',           KEY_NONE,
  KEY_NONE,     ' ',         'v',           'f',
  't',          'r',         '5',           KEY_NONE,
  KEY_NONE,     'n',         'b',           'h',
  'g',          'y',         '6',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'm',           'j',
  'u',          '7',         '8',           KEY_NONE,
  KEY_NONE,     ',',         'k',           'i',
  'o',          '0',         '9',           KEY_NONE,
  KEY_NONE,     '.',         '/',           'l',
  ';',          'p',         '-',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    '\'',          KEY_NONE,
  '[',          '=',         KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     ']',
  KEY_NONE,     '\\',        KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7
};

const int keyb_map[] = {
  // default
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'q',         '1',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'z',           's',
  'a',          'w',         '2',           KEY_NONE,
  KEY_NONE,     'c',         'x',           'd',
  'e',          '4',         '3',           KEY_NONE,
  KEY_NONE,     ' ',         'v',           'f',
  't',          'r',         '5',           KEY_NONE,
  KEY_NONE,     'n',         'b',           'h',
  'g',          'y',         '6',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'm',           'j',
  'u',          '7',         '8',           KEY_NONE,
  KEY_NONE,     ',',         'k',           'i',
  'o',          '0',         '9',           KEY_NONE,
  KEY_NONE,     '.',         '-',           'l',
  KEY_NONE,     'p',         '\'',          KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  '`',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     '+',
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_NONE,     '<',         KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7,
  
  // left/right shift on
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'Q',         '!',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'Z',           'S',
  'A',          'W',         '"',           KEY_NONE,
  KEY_NONE,     'C',         'X',           'D',
  'E',          '$',         KEY_NONE,      KEY_NONE,
  KEY_NONE,     ' ',         'V',           'F',
  'T',          'R',         '%',           KEY_NONE,
  KEY_NONE,     'N',         'B',           'H',
  'G',          'Y',         '&',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'M',           'J',
  'U',          '/',         '(',           KEY_NONE,
  KEY_NONE,     ';',         'K',           'I',
  'O',          '=',         ')',           KEY_NONE,
  KEY_NONE,     ':',         '_',           'L',
  KEY_NONE,     'P',         '?',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  '^',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     '*',
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_NONE,     '>',         KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7,
  
  // right alt on
  KEY_NONE,     KEY_F9,      KEY_NONE,      KEY_F5,
  KEY_F3,       KEY_F1,      KEY_F2,        KEY_F12,
  KEY_NONE,     KEY_F10,     KEY_F8,        KEY_F6,
  KEY_F4,       KEY_TAB,     '\\',          KEY_NONE,
  KEY_NONE,     KEY_L_ALT,   KEY_L_SHIFT,   KEY_NONE,
  KEY_L_CTRL,   'q',         '|',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'z',           's',
  'a',          'w',         '@',           KEY_NONE,
  KEY_NONE,     'c',         'x',           'd',
  'e',          '~',         '#',           KEY_NONE,
  KEY_NONE,     ' ',         'v',           'f',
  't',          'r',         '5',           KEY_NONE,
  KEY_NONE,     'n',         'b',           'h',
  'g',          'y',         '6',           KEY_NONE,
  KEY_NONE,     KEY_NONE,    'm',           'j',
  'u',          '7',         '8',           KEY_NONE,
  KEY_NONE,     ',',         'k',           'i',
  'o',          '0',         '9',           KEY_NONE,
  KEY_NONE,     '.',         '-',           'l',
  KEY_NONE,     'p',         '\'',          KEY_NONE,
  KEY_NONE,     KEY_NONE,    '{',           KEY_NONE,
  '[',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  KEY_CAPSLOCK, KEY_R_SHIFT, KEY_ENTER,     ']',
  KEY_NONE,     '}',         KEY_NONE,      KEY_NONE,
  KEY_NONE,     '<',         KEY_NONE,      KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_BACKSPACE, KEY_NONE,
  KEY_NONE,     '1',         KEY_NONE,      '4',
  '7',          KEY_NONE,    KEY_NONE,      KEY_NONE,
  '0',          '.',         '2',           '5',
  '6',          '8',         KEY_ESCAPE,    KEY_NUMLOCK,
  KEY_F11,      '+',         '3',           '-',
  '*',          '9',         KEY_SCRLOCK,   KEY_NONE,
  KEY_NONE,     KEY_NONE,    KEY_NONE,      KEY_F7
};

#endif
