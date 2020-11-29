#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 16
int main(){
        int fd,ret;
        char buffR[SIZE];//read buffer
        fd=open("/dev/sample0",O_RDWR);//sample0 c 42 0
        if(fd<0){
                perror("Error in opening\n");
                return fd;
         }
        ret=read(fd,buffR,SIZE);
      printf("got:%s no of bytes read :%d\n",buffR,ret);
        return 0;
}
