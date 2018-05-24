// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){//如果没有设备文件 创建一个设备文件
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout 1
  dup(0);  // stderr 2

  for(;;){//会不断的重启shell
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)//谁是zombie 这句话应该是专门测试zombie.c的，儿子比父亲先死
      printf(1, "zombie!\n");
  }
}

//gerw done