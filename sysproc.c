#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_inittaskmgr(void)
{
  return inittaskmgr();
}

int
sys_closetaskmgr(void)
{
  return closetaskmgr();
}

int
sys_getprocinfo(void)
{
  int *pid;
  char **name;
  int *state;
  uint *sz;

  if(argptr(0, (void*)&pid, 64*sizeof(int)) < 0)
    return -1;
  if(argptr(1, (void*)&name, 64*sizeof(char*)) < 0)
    return -1;
  if(argptr(2, (void*)&state, 64*sizeof(int)) < 0)
    return -1;
  if(argptr(3, (void*)&sz, 64*sizeof(uint)) < 0)
    return -1;
  return getprocinfo(pid, name, state, sz);
}

int
sys_updscrcont(void)
{
  int pos, n;
  char *buf;

  if(argint(0, &pos) < 0)
    return -1;
  if(argint(2, &n) < 0)
    return -1;
  if(argptr(1, &buf, n) < 0)
    return -1;
  return updscrcont(pos, buf, n);
}

int
sys_chgcurproc(void)
{
  int pos, optr;

  if(argint(0, &pos) < 0 || argint(1, &optr) < 0)
    return -1;
  return chgcurproc(pos, optr);
}

int
sys_chgcurpage(void)
{
  int pos, n, optr;
  char *buf;

  if(argint(0, &pos) < 0)
    return -1;
  if(argint(2, &n) < 0)
    return -1;
  if(argptr(1, &buf, n) < 0)
    return -1;
  if(argint(3, &optr) < 0)
    return -1;
  return chgcurpage(pos, buf, n, optr);
}
