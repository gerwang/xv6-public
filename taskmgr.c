#include "types.h"
#include "stat.h"
#include "user.h"

#define NPROC 64  // maximum number of processes
#define KEY_UP 0xE2
#define KEY_DN 0xE3
#define KEY_LF 0xE4
#define KEY_RT 0xE5

const char procstate[6][9] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};

char buff[96][80];
int curline = 4;  //表示当前高亮显示的行序号，范围4-19
int curpage = 0;  //表示当前页码，范围0-3
int procID[NPROC];
char procName[NPROC][16];
int procState[NPROC];
uint procSize[NPROC];

void runTaskMgr(void);
int getCmd(char *buf);
void procCmd(char *cmd);
//int fork2(void);
void calsTaskMgrInfo(void);
//void printTaskMgrInfo(void);
//void printString(char *fmt, int line, ...);
//void printPageInfo();
//int printint(int xx, int base, int sign, int pos, int limit, char *buff);

void
runTaskMgr(void)
{
  char buf[2];
  char *c;

  calsTaskMgrInfo();
  updscrcont(buff[curpage*24], curline);
  while(getCmd(buf) >= 0)
  {
    /*
    if(fork2() == 0)
    {
      c = buf;
      procCommand(c);
    }
    sleep(1);
    */
    c = buf;
    procCmd(c);
    calsTaskMgrInfo();
    updscrcont(buff[curpage*24], curline);
  }
}

int
getCmd(char *buf)
{
  memset(buf, 0, sizeof(char) * 2);
  gets(buf, 2);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

void
procCmd(char *cmd)
{
  switch((*cmd) & 0xff){
  case KEY_UP:
    if(curline > 4)
      curline--;
    break;
  case KEY_DN:
    if(curline < 19)
      curline++;
    break;
  case KEY_LF:
    if(curpage > 0)
      curpage--;
    break;
  case KEY_RT:
    if(curpage < 3)
      curpage++;
    break;
  case 'k':
    if(strcmp("taskmgr", procName[curpage*16+curline-4]) == 0){
      closetaskmgr();
      exit();
      break;
    }
    else{
      kill(procID[curpage*16+curline-4]);
      break;
    }
  case 'q':
    closetaskmgr();
    exit();
    break;
  default:
    break;
  }
}

/*
int
fork2(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    exit();
  return pid;
}
*/

void
calsTaskMgrInfo(void)
{
  int i, j;
  getprocinfo(procID, procName, procState, procSize);
  for(i = 0; i < 96; i++)
    for(j = 0; j < 80; j++)
      buff[i][j] = '1';
}

/*
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
*/

int
main(void)
{
  inittaskmgr();
  runTaskMgr();
  exit();
}
