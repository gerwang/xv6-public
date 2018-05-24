#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])//没有环境变量，只是简单的输出字符
{
  int i;

  for(i = 1; i < argc; i++)
    printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
  exit();
}
//gerw done