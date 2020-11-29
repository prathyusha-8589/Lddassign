#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 16
int main(){
        int fd,ret;
       
        char buffW[]="fromuser";//write buffer
        fd=open("/dev/sample0",O_RDWR);//sample0 c 42 0
        if(fd<0){
                perror("Error in opening\n");
                return fd;
         }
     
        ret=write(fd,buffW,sizeof(buffW));
	printf("write %d bytes\n",ret);
        return 0;
}
