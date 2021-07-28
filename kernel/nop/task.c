#include <nop/task.h>
#include <nop/type.h>
#include <nop/virt.h>

int task_call(task_t task) {
  virt_load(task.table);
  int value = task.start();

  virt_load(virt_table);
  return value;
}