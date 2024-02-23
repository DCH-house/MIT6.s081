#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

// Your goal is to use pipe and fork to set up the pipeline. The first process feeds the numbers 2 through 35 into the pipeline. 
// For each prime number, you will arrange to create one process that reads from its left neighbor over a pipe and 
// writes to its right neighbor over another pipe. Since xv6 has limited number of file descriptors and processes, 
// the first process can stop at 35
#define PIPE_READ 0
#define PIPE_WRITE 1
int N = 35;

void child(int pipe_left[2]){
    int pipe_right[2];
    pipe(pipe_right);
    
    int num = 0,prime = 0;
    int cnt = 0;

    while(read(pipe_left[PIPE_READ],&num,sizeof (int))){
        if(num == 0)break;
        if(cnt == 0){
            cnt++;
            prime = num;
            printf("prime %d\n",prime);
        }else if(num % prime){
            write(pipe_right[PIPE_WRITE],&num,sizeof (int));   
        }
    }
    close(pipe_right[PIPE_WRITE]);//close the wirte end after the parent process finishing the writing work  
    close(pipe_left[PIPE_READ]);//close the read end after the parent child finishing the reading work 
    if(prime != 0){
        if(fork() == 0){
            child(pipe_right);
        }
    }
    wait(0);
    exit(0);
}

int main(){
    int pipe_left[2];
    pipe(pipe_left);

    for(int i = 2; i <= N; i ++){
        write(pipe_left[PIPE_WRITE],&i,sizeof (int));
    }
    close(pipe_left[PIPE_WRITE]);
    child(pipe_left);
    wait(0);
    exit(0);
}