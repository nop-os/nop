#include <devs/fbr.h>
#include <nop/type.h>
#include <nop/conn.h>

conn_hand_t fbr_hand = (conn_hand_t){
  0, "fbr", NULL,
  fbr_init, NULL,
  fbr_connect, NULL,
  fbr_write, fbr_read, NULL
};