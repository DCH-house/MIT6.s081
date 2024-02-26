#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name.
// Look at user/ls.c to see how to read directories.
// Use recursion to allow find to descend into sub-directories.
// Don't recurse into "." and "..".
// Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
// You'll need to use C strings. Have a look at K&R (the C book), for example Section 5.5.
// Note that == does not compare strings like in Python. Use strcmp() instead.
// Add the program to UPROGS in Makefile

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  //check if this file can be poened
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  //printf("st.type = %d\n",st.type);
  switch(st.type){
  case T_FILE://a file type
    printf("fmtname:%s path: %s %d %d %l\n", fmtname(path), path, st.type, st.ino, st.size);
    break;

  case T_DIR://a directory type
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc == 1 || argc > 3){
    printf("find (directory) target!");
    exit(1);
  }
  if(argc == 2){
    ls(".",argv[0]);
    exit(0);
  }
  if(argc == 3){
    ls(argv[0],argv[1]);
  }

}