#include <devs/ata.h>
#include <devs/fat.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/dbg.h>

conn_hand_t fat_hand;

ssize_t fat_init(conn_hand_t *hand, void *data) {

}

void fat_free(conn_hand_t *hand) {

}

void fat_connect(conn_t *conn, const char *path) {

}

ssize_t fat_write(conn_t *conn, void *buffer, size_t count) {

}

ssize_t fat_read(conn_t *conn, void *buffer, size_t count) {

}

void fat_init_all(void) {
  for (size_t i = 0; i < conn_count; i++) {
    if (!memcmp(conn_hand[i].name, "blk", 3)) {
      conn_t *conn = conn_connect(conn_hand[i].name);
      if (!conn) continue;

      ata_mbr_t mbr;

      dbg_infof("fat: seeking device %s\n", conn_hand[i].name);

      conn_seek(conn, 0x01BE, conn_seek_set);
      conn_read(conn, &mbr, sizeof(ata_mbr_t));
      
      int count = 0;

      for (int j = 0; j < 4; j++) {
        dbg_infof("- part %d: type=0x%02X, addr=0x%08X, size=0x%08X\n", j, mbr.part[j].type, mbr.part[j].lba_addr, mbr.part[j].lba_size);

        if (mbr.part[j].type) {
          // Do something

          count++;
        }
      }

      if (count) {
        conn_release(conn);
      } else {
        
      }
    }
  }
}