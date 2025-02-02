#include <klib.h>
#include <rtthread.h>

static int shutdown(void) {
  halt(0);
  return 0;
}
MSH_CMD_EXPORT(shutdown, Shutdown);
