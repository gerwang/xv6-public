// Create a zombie process that
// must be reparented at exit. 测试进程调度的reparent功能

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  if(fork() > 0)
    sleep(5);  // Let child exit before parent. 睡50ms
  exit();
}
// gerw done