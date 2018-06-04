#include "types.h"
#include "stat.h"
#include "user.h"

void runTaskMgr(void);
int getCommand(char *buf);
void procCommand(char *cmd);
int fork2(void);

void printTaskMgrInfo(void);
void printString();
void printPageInfo();

void
runTaskMgr(void)
{
  static char buf[2];
  char *c;

  while(getCommand(buf) >= 0)
  {
    if(fork2() == 0)
    {
      c = buf;
      procCommand(c);
    }
    sleep(1);
    printTaskMgrInfo();
  }
}

int
getCommand(char *buf)
{
  memset(buf, 0, sizeof(char) * 2);
  gets(buf, 2);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

void
procCommand(char *cmd)
{
  printf(1, cmd);
}

int fork2(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    exit();
  return pid;
}

void
printTaskMgrInfo(void)
{
  printf(1, "printTaskMgrInfo\n");
}

void
printString()
{

}

void
printPageInfo()
{

}

int main(void)
{
  runTaskMgr();
  return 0;
}
