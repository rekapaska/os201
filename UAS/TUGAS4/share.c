/*
// Kontributor: 
rekapaska, shanniadm, chelmel18, zxrsvy, tolhassianipar, hnsmchl, jacobstmrg, akhmadiponegoro
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 19:04:35 WIB 2020
*/

#include     "share.h"
char*        progs[]={P01, P02, P03, P04, P05,P06,P07};
char         tmpStr[tmpSize]={};
extern char* akunGitHub;
extern int   delay;
extern int   boss;
extern int   initValue;
myshare*     mymap;

// Kontributor: rekapaska, tolhassianipar
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 10:42:30 WIB 2020
void initBoss(int argc, char* argv[]) {
  int ssize=sizeof(myshare);
  int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
  fchmod   (fd, CHMOD);
  ftruncate(fd, ssize);
  mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
  close(fd);
  
  sem_init (&(mymap->mutex), SSHARE, SMUTEX);
  sem_init (&(mymap->sync),  SSHARE, SSYNC);
  sem_init (&(mymap->progs[0].sync),  SSHARE, SMUTEX);
  sem_init (&(mymap->progs[1].sync),  SSHARE, SMUTEX);

  if (argc > 1) {
    sleep(delay);
    if (mymap->state == CLOSED){
      mymap->state = OPEN; 
    } else {
      int i;
      mymap->mutexctr = 0;
      mymap->entry = 0;
      mymap->state = 0;
      for(i =0; i<=MAXPROGS;i++){
        sem_t*   resetMutex = &((mymap->progs[i]).sync);
        usrinfo structVar = {"", 0, *(resetMutex)};
        mymap->progs[i]=structVar;
      }
    }
    exit(0);
  } else {
      mymap->state = OPEN;
      for (int i = 0; i < sizeof(progs)/sizeof(char*); i++) {
        if (fork() == 0) {
            execlp(progs[i], progs[i], NULL);
        }
    }
    doBoss();
  }
}

// Kontributor: shanniadm
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 11:13:20 WIB 2020
void initNotBoss(void) {
  sleep (delay);
  if( access(SHAREMEM, F_OK ) == -1 ) {
    exit(-1);
  }
  int fd=open(SHAREMEM, O_RDWR, CHMOD);
  int ssize=sizeof(myshare);
  mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
  close(fd);
  doNotBoss();
}

// Kontributor: chelmel18
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 13:21:38 WIB 2020
void init(int isboss, int argc, char* argv[]) {
  if (isboss == BOSS) {
    initBoss(argc, argv);
  } else {
    initNotBoss();
  }
}

// Kontributor: chelmel18
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 14:28:19 WIB 2020
void myPrint(char* str1, char* str2) {
    printf("%-16s[%s]\n", str1, str2);
    fflush(NULL);
}

// Kontributor: chelmel18, zxrsvy
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 15:27:15 WIB 2020
int getEntry(char* akunGitHub) {
    int entry=0;
    int i;
    sem_wait (&(mymap->mutex));
    for (i = 0; i <= mymap->entry; i++){
      entry = i;
      if(strcmp(akunGitHub, mymap->progs[i].akun) == 0){ 
        incrementMutex();
        sem_post (&(mymap->mutex));
        return entry;
      }
    }

    entry = mymap->entry++;
    incrementMutex();
    sem_post (&(mymap->mutex));
    return entry;
}

// Kontributor: zxrsvy, shanniadm
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 13:21:48 WIB 2020
void incrementMutex() {
  mymap->mutexctr++;
}

// Kontributor: zxrsvy
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 17:32:16 WIB 2020
void display(int entry) {
    sem_wait (&(mymap->mutex));
    incrementMutex();
    int i;
    char akun[256] = {}; 
    for (i = 0; i < mymap->entry; i++) {
        sprintf(akun, "%s[%s]", akun, mymap->progs[i].akun);
    }
    
    char state[5] = {}; 
    if (mymap->state == 255) 
        sprintf(state, "OPEN"); 

    sprintf(tmpStr, "progs[%2.2d] TIME[%3.3d] MMAP[%s] VALUE[%3.3d] %s",
        entry, mymap->mutexctr, state, mymap->progs[entry].value, akun);

    myPrint(akunGitHub, tmpStr);
    sem_post (&(mymap->mutex));
}

// Kontributor: shanniadm
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 14:27:42 WIB 2020
void putInfo(char* akun, int entry) {
  sem_wait (&(mymap->mutex));
  strcpy(mymap->progs[entry].akun, akun);
  incrementMutex();
  sem_post (&(mymap->mutex));
}

// Kontributor : shanniadm, rekapaska
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 10:26:28 WIB 2020
void checkOpen(void) {
    if(mymap->state == CLOSED){
      printf("ShareMemory is NOT OPEN, BYE BYE ==== ====\n");
      exit(0);
    }
}

// Kontributor: shanniadm, tolhassianipar
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 16:21:43 WIB 2020
void myWait(int boss, int entry) {
    sleep(delay);
    if (boss == BOSS) {
      while(wait(NULL)>0);
      mymap->state = CLOSED;
    }
}

// Kontributor: rekapaska
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 15:27:19 WIB 2020
void setValue(int entry, int initValue) {
  mymap->progs[entry].value = initValue;
}

// Kontributor: shanniadm, tolhassianipar
// Harap UPDATE tanggal revisi!
// Thu Jul  2 14:54:54 WIB 2020
// Fri Jul  3 17:27:14 WIB 2020
void doBoss() {
  sprintf(tmpStr, "START PID[%5.5d] PPID[%5.5d]", getpid(), getppid());
  myPrint(akunGitHub, tmpStr);
  sprintf(tmpStr, "Boss is Waiting");
  myPrint(akunGitHub, tmpStr);
  sem_wait (&(mymap->progs[0].sync));
  sem_wait (&(mymap->progs[1].sync));

  for (int ii=0; ii < sizeof(progs)/sizeof(char*); ii++)
    sem_wait (&(mymap->sync));
    
  for (int ii=0; ii< sizeof(progs)/sizeof(char*); ii++)
    sem_post (&(mymap->progs[0].sync));

  for (int ii=0; ii < sizeof(progs)/sizeof(char*); ii++)
    sem_wait (&(mymap->sync));

  sprintf(tmpStr, "All NoBosses are ready!");
  myPrint(akunGitHub, tmpStr);
  sem_post (&(mymap->progs[0].sync));
  
  for (int ii=0; ii < sizeof(progs)/sizeof(char*); ii++)
    sem_post (&(mymap->progs[1].sync));
    
  sem_post (&(mymap->progs[1].sync));
}

// Kontributor: shanniadm, tolhassianipar
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Fri Jul  3 17:38:24 WIB 2020
void doNotBoss() {
  sprintf(tmpStr, "START PID[%5.5d] PPID[%5.5d]", getpid(), getppid());
  myPrint(akunGitHub, tmpStr);
  sem_post (&(mymap->sync));
  
  sem_wait (&(mymap->progs[0].sync));
  sleep(delay);
  sprintf(tmpStr, "Good Bye initNoBoss()!");
  myPrint(akunGitHub, tmpStr);
  sem_post (&(mymap->sync));
  sem_wait(&(mymap->progs[1].sync));
}

// Kontributor: shanniadm, tolhassianipar
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Fri Jul  3 17:23:17 WIB 2020
int main(int argc, char* argv[]) {
    init(boss, argc, argv);
    checkOpen();
    sleep  (delay);
    int entry = getEntry(akunGitHub);
    myPrint(akunGitHub, "Yes, I am ready!");
    setValue(entry, initValue);
    sleep  (delay);
    display(entry);
    sleep  (delay);
    putInfo(akunGitHub, entry);
    sleep  (delay);
    display(entry);
    myWait (boss, entry);
    myPrint(akunGitHub, "BYEBYE === ===== ===== =====");
}