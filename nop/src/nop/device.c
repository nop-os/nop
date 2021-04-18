#include <nop/device.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

device_t device_list[MAX_DEVICES];

void device_init(void) {
  // Set all devices as not present, and empty
  // their names, pretty easy to understand :)
  for (int i = 0; i < MAX_DEVICES; i++) {
    memset(device_list[i].name, 0, 5);
    device_list[i].present = 0;
  }
}

device_t *device_add(const char *name) {
  // Look for a not present device...
  for (int i = 0; i < MAX_DEVICES; i++) {
    // Hey, this one seems like it's not!
    if (!device_list[i].present) {
      // Now set it as present and set its name
      device_list[i].present = 1;
      strncpy(device_list[i].name, name, 4);

      // And finally return it
      return &(device_list[i]);
    }
  }

  // We didn't find any not present device :(
  return NULL;
}

void device_remove(device_t *device) {
  // I wonder what this does...
  device->present = 0;
}
