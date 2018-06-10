#include "types.h"
#include "stat.h"
#include "user.h"

char buff[80][80];

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
printString(char *fmt, int line, ...) // 打印在第line行
{
  int c, digit, count, remain, currentDigit = 0; //目前輸出到第line行第currentDigit位
  char *s;
  memset(buff[line/80], 0, sizeof(char)*80);
  uint *argp;
  
  if (fmt == 0)
  {
    printf(1, "null fmt\n");
  }
  
  for(int i = 0; (c = fmt[i] & 0xff) != 0; i++)
  {
    if (currentDigit = 0)
    {
      goto bad;
    }
    if (c != '%')
    {
      if (c == '\n')
      {
        buff[line/80][currentDigit++] = 0;
        currentDigit = 80;
      }
      else
      {
        buff[line/80][currentDigit++] = c;
      }
      continue;
    }
    c = fmt[++i] & 0xff;
    if (c == 0)
    {
      break;
    }
    digit = 0;
    while (c >= '0' && c <= '9')
    {
      digit = digit * 10 + c - '0';
      c = fmt[++i] & 0xff;
    }
    if (currentDigit + digit > 80)
    {
      goto bad;
    }
    remain = (digit == 0) ? (80 - currentDigit) : digit;
    
    switch(c)
    {
      case 'd':
        break;
      case 'x':
      case 'p':
        break;
      case 's':
        break;
      case '%':
        break;
      default:
        break;
    }
    return;
  }
  bad:
  
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
