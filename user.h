#ifndef USER_H
#define USER_H

#include "usyscall.h"

// ulib.c
int stat(char*, struct stat*);
void printf(int, char*, ...);
char* gets(char*, int max);
int atoi(const char*);

#include <stdlib.h>
#include <string.h>

#endif

//char* strcpy(char*, char*);
//void *memmove(void*, void*, int);
//char* strchr(const char*, char c);
//int strcmp(const char*, const char*);
//uint strlen(char*);
//void* memset(void*, int, uint);
//void* malloc(uint);
//void free(void*);
