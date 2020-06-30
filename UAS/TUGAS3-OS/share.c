/*
// Kontributor: 
JahnsMichael, rekapaska, wulanmantiri, glendaesutanto, shanniadm, chelmel18, zxrsvy, tolhassianipar
// Tue Jun  9  17:46:47 WIB 2020
// Tue Jun  30 14:18:55 WIB 2020
 */

#include     "share.h"
char*        progs[]={P01, P02, P03, P04, P05, P06, P07};
char         tmpStr[256]={};
extern char* akunGitHub;
extern int   delay;
extern int   boss;
myshare*     mymap;

// Kontibutor : JahnsMichael, tolhassianipar
// Tue Jun  9 17:46:47 WIB 2020
// Tue Jun  29 23:56:10 WIB 2020
// Tue Jun  30 00:51:05 WIB 2020
// Tue Jun  30 09:47:30 WIB 2020
// Tue Jun  30 14:18:55 WIB 2020
void init(int isboss, int argc, char* argv[]) {
    if (isboss == BOSS) {
        int ssize=sizeof(myshare);
        int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
        fchmod   (fd, CHMOD);
        ftruncate(fd, ssize);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
        
        mymap->state = OPEN;

        sem_init (&(mymap->mutex), 1, MUTEX);
        
        if (argc > 1) {
          sleep(delay);
          printf("ShareMemory is OPEN, BYE BYE ==== ====\n");
          exit(0);
        } else {
          for (int i = 0; i < sizeof(progs)/sizeof(char*); i++) {
              if (fork() == 0) {
                  execlp(progs[i], progs[i], NULL);
              }
          }
        }

    } else {
        sleep(delay);
        if( access(SHAREMEM, F_OK ) == -1 ) {
            printf("No \"%s\" file.\n", SHAREMEM);
            exit(-1);
        }
        int fd=open(SHAREMEM, O_RDWR, CHMOD);
        int ssize=sizeof(myshare);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
    }
}

// Kontributor: glendaesutanto
// Tue Jun  9 17:46:47 WIB 2020
// Tue Jun 30 14:01:15 WIB 2020
void myPrint(char* str1, char* str2) {
    printf("%-14s[%s]\n", str1, str2);
    fflush(NULL);
}

// Kontributor: glendaesutanto, tolhassianipar, chelmel18, rekapaska
// Tue Jun  9 17:46:47 WIB 2020
// Tue Jun  29 22:38:45 WIB 2020
// Tue Jun  30 00:51:05 WIB 2020
int getEntry(char* akunGitHub) {
    int entry=0;
    int i;
    sem_wait (&(mymap->mutex));
    for (i = 0; i <= mymap->entry; i++){
      entry = i;
      if(strcmp(akunGitHub, mymap->progs[i].akun) == 0){ 
        incrementMutex(entry);
        sem_post (&(mymap->mutex));
        return entry;
      }
    }

    entry = mymap->entry++;
    incrementMutex(entry);
    sem_post (&(mymap->mutex));
    return entry;
}

// Kontributor: wulanmantiri, glendaesutanto, chelmel18
// Tue Jun  9  17:46:47 WIB 2020
// Tue Jun  30 13:55:47 WIB 2020
void display(int entry) {
    sem_wait (&(mymap->mutex));
    incrementMutex(entry);
    int i;
    char akun[256] = {}; 
    for (i = 0; i < mymap->entry; i++) {
        sprintf(akun, "%s[%s]", akun, mymap->progs[i].akun);
    }
    
    char state[5] = {}; 
    if (mymap->state == 255) 
        sprintf(state, "OPEN");
        
    sprintf(tmpStr, "progs[%2.2d] TIME[%2.2d] MUTEX[%2.2d] MMAP[%s] %s",
        entry, mymap->mutexctr, mymap->progs[entry].stamp, state, akun);

    myPrint(akunGitHub, tmpStr);
    sem_post (&(mymap->mutex));
}

// Kontributor: rekapaska, wulanmantiri, zxrsvy
// Tue Jun  9  17:46:47 WIB 2020
// Tue Jun  30 00:34:46 WIB 2020
void putInfo(char* akun, int entry) {
    sem_wait (&(mymap->mutex));
    strcpy(mymap->progs[entry].akun, akun);
    incrementMutex(entry);
    sem_post (&(mymap->mutex));
}

// Kontributor: wulanmantiri, shanniadm, zxrsvy
// Tue Jun  9  17:46:47 WIB 2020
// Tue Jun  30 13:52:06 WIB 2020
void checkOpen(void) {
    if (mymap->state == CLOSED) {
      printf("ShareMemory is NOT OPEN, BYE BYE ==== ====\n");
      exit(0);
    }
}

// Kontributor : shanniadm, JahnsMichael, tolhassianipar
// Tue Jun  9 17:46:47 WIB 2020
// Tue Jun  30 13:32:28 WIB 2020
void myWait(int boss, int entry) {
  if (boss == BOSS) {
    while(wait(NULL)>0);
    mymap->state = CLOSED;
  }
}

// Kontributor: wulanmantiri, JahnsMichael, shanniadm, zxrsvy
// Tue Jun  9  17:46:47 WIB 2020
// Tue Jun  30 09:31:25 WIB 2020
// Tue Jun  30 13:45:40 WIB 2020
void incrementMutex(int entry) {
  mymap->mutexctr++;
  mymap->progs[entry].stamp++;
}

// Kontributor : -
// Tue Jun  9 17:46:47 WIB 2020
int main(int argc, char* argv[]) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myPrint(akunGitHub, tmpStr);
    init(boss, argc, argv);
    checkOpen();
    sleep  (delay);
    int entry = getEntry(akunGitHub);
    sleep  (delay);
    display(entry);
    sleep  (delay);
    putInfo(akunGitHub, entry);
    sleep  (delay);
    display(entry);
    myWait (boss, entry);
    myPrint(akunGitHub, "BYEBYE =====  ===== =====");
}