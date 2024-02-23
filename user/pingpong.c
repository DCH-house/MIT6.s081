#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

// Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, 
// one for each direction. The parent should send a byte to the child; the child should print "<pid>: received ping", 
// where <pid> is its process ID, write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, 
// print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.

// Use pipe to create a pipe.
// Use fork to create a child.
// Use read to read from the pipe, and write to write to the pipe.
// Use getpid to find the process ID of the calling process.
// Add the program to UPROGS in Makefile.
// User programs on xv6 have a limited set of library functions available to them. You can see the list in user/user.h; 
// the source (other than for system calls) is in user/ulib.c, user/printf.c, and user/umalloc.c

int main(){
    int p1[2];
    int p2[2];
    char buf[1];

    //create two pipe,one for child write parent recieve,another for parent write child receive
    pipe(p1);//one for child write parent recieve
    pipe(p2);//for parent write child receive
    int pid = fork();
    if(pid < 0){
        printf("Error");
        exit(1);
    }else if(pid == 0){
        //child
        close(p1[0]);
        close(p2[1]);
        read(p2[0],buf,1);//receive a Byte from parent
        printf("%d: received ping\n",getpid());
        write(p1[1]," ",1);//wirte a Byte to parent
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }else{
        //parent
        close(p2[0]);
        close(p1[1]);
        write(p2[1]," ",1);//wirte a Byte to child
        printf("%d: recieved pong\n",getpid());
        read(p1[0],buf,1);//receive a Byte from child
        close(p2[1]);
        close(p1[0]);
        exit(0);
    }
}