/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 4
#define  akunGitHub "chelmel18"
#include "p00.h"
int      entry;

char*    progs[]={P01, P02, P03, P04, P05, P06};
myshare* mymap;

int init(void) {
    sleep(DELAY);
    // exit when file is not initiated by parent (p00)
    struct  stat fsize;
    int     ssize = sizeof(myshare);
    int     fd = open(SHAREMEM, MYFLAGS, CHMOD);
    fstat(fd, &fsize);
    if (fsize.st_size < ssize) {
        printf("No \"SharedMemoryFile.bin\" file.\n");
        exit(1);
    }

    fchmod(fd, CHMOD);
    ftruncate(fd, ssize);
    mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    return NOTBOSS;
}

char tmpStr[256]={};
void myprint(char* str1, char* str2) {
    printf("%-14s[%s]\n", str1, str2);
}

int getEntry(void) {
    return entry;
}

void display(int entry) {
    // display an entry of MMAP. Eg.
    // akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
    int i;
    char akun[256] = {}; 
    for (i = 0; i < entry; i++) {
        sprintf(akun, "%s[%s]", akun, mymap->progs[i].akun);
    }

    char state[5] = {}; 
    if (mymap->state == 255) 
        sprintf(state, "OPEN");

    sprintf(tmpStr, "progs[%2.2d] TIME[%2.2d] MUTEX[%2.2d] MMAP[%s] %s",
        getEntry(), mymap->mutexctr, mymap->progs[getEntry()].stamp, state, akun);
    myprint(akunGitHub, tmpStr);
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
    strcpy(mymap->progs[entry].akun, akun);
}

void checkOpen(void) {
    // exit if MMAP is closed.
   if (mymap->state == CLOSED) {
        printf("CLOSED, BYE BYE ==== ====\n");
        exit(1);
    }
}

void incrementMutex(void) {
    mymap->mutexctr++;
    mymap->progs[getEntry()].stamp++;
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();
    sleep (DELAY);

    sem_wait(&mymap->mutex);
    entry = mymap->entry++;
    incrementMutex();
    sem_post(&mymap->mutex);

    sem_wait(&mymap->mutex);
    incrementMutex();
    display(mymap->entry);
    sem_post(&mymap->mutex);

    sleep (DELAY);
    sem_wait(&mymap->mutex);
    incrementMutex();
    putInfo(akunGitHub, getEntry());
    sem_post(&mymap->mutex);

    sleep (DELAY);
    sem_wait(&mymap->mutex);
    incrementMutex();
    display(mymap->entry);
    sem_post(&mymap->mutex);

    sem_wait(&mymap->mutex);
    incrementMutex();
    display(mymap->entry);
    sem_post(&mymap->mutex);

    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}