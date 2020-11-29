#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 16
int main(){
        int fd,ret;
        char buffR[SIZE];//read buffer
        char buffW[]="from user";//write buffer
        fd=open("/dev/sample0",O_RDWR);//sample0 c 42 0
        if(fd<0){
                perror("Error in opening\n");
                return fd;
         }
        ret=read(fd,buffR,SIZE);
        ret=write(fd,buffW,sizeof(buffW));
        return 0;
}

