/*
// Kontributor: 
rekapaska, shanniadm, chelmel18, zxrsvy, tolhassianipar, hnsmchl, jacobstmrg, akhmadiponegoro
// Harap UPDATE tanggal revisi!
// Thu Jul  2 10:35:03 WIB 2020
*/

#define P00 "./p00"
#define P01 "./p01"
#define P02 "./p02"
#define P03 "./p03"
#define P04 "./p04"
#define P05 "./p05"
#define P06 "./p06"
#define P07 "./p07"

#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define AKUNSIZE     30
#define BOSS         0
#define NOTBOSS      1
#define CHMOD        0666
#define CLOSED       127
#define MAXPROGS     20
#define MYFLAGS      O_CREAT|O_RDWR
#define MYPROTECTION PROT_READ|PROT_WRITE
#define MYVISIBILITY MAP_SHARED
#define OPEN         255
#define SHAREMEM     "SharedMemoryFile.bin"
#define SSYNC         0
#define SMUTEX        1
#define SSHARE        1
#define tmpSize      1024

typedef struct {
    char  akun [AKUNSIZE];
    int   value;
    sem_t sync;
} usrinfo;

typedef struct {
    usrinfo progs[MAXPROGS+1];
    int     entry;
    int     state;
    int     mutexctr;
    sem_t   mutex;
    sem_t   sync;
} myshare;

void checkOpen(void);
void display(int entry);
void doBoss();
void doNotBoss();
int  getEntry(char* akunGitHub);
void init(int isboss, int argc, char* argv[]);
void initBoss(int argc, char* argv[]);
void initNotBoss(void);
void myPrint(char* str1, char* str2);
void myWait(int boss, int entry);
void putInfo(char* akun, int entry);
void setValue(int entry, int initValue);
void incrementMutex();