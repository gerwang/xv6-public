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
int printint(int xx, int base, int sign, int pos, int limit, char *buff);

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
  int c, digitsDesired, digitsUsed, remain, currentDigit = 0; //目前輸出到第line行第currentDigit位
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
    digitsDesired = 0;
    digitsUsed = 0;
    while (c >= '0' && c <= '9')
    {
      digitsDesired = digitsDesired * 10 + c - '0';
      c = fmt[++i] & 0xff;
    }
    if (currentDigit + digitsDesired > 80)
    {
      goto bad;
    }
    remain = (digitsDesired == 0) ? (80 - currentDigit) : digitsDesired;
    
    switch(c)
    {
      case 'd':
        digitsUsed = printint(*argp++, 10, 1, currentDigit, remain, buff[line / 80]);
        currentDigit += digitsUsed;
        while(digitsUsed <= digitsDesired)
        {
          buff[line / 80][currentDigit++] = 0;
          digitsUsed++;
        }
        break;
      case 'x':
      case 'p':
        digitsUsed = printint(*argp++, 16, 0, currentDigit, remain, buff[line / 80]);
        currentDigit += digitsUsed;
        while(digitsUsed <= digitsDesired)
        {
          buff[line / 80][currentDigit++] = 0;
          digitsUsed++;
        }
        break;
      case 's':
        if((s = (char*)*argp++) == 0)
    	    s = "(null)";
        while(digitsUsed < remain)
        {
          buff[line / 80][currentDigit++] = *s;
          s++;
          digitsUsed++;
        }
	  		while(digitsUsed <= digitsDesired) //補空格
	  		{
				  buff[line / 80][currentDigit++] = 0;
				  digitsUsed++;
	  		}
        break;
      case '%':
        buff[line / 80][currentDigit] = c;
        break;
      default:
        if (currentDigit > 78)
        {
          goto bad;
        }
        else
        {
          buff[line / 80][currentDigit++] = '%';
          buff[line / 80][currentDigit++] = c;
        }
        break;
    }
    return;
  }
  bad:
  
}

int printint(int xx, int base, int sign, int pos, int limit, char *buff)
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
