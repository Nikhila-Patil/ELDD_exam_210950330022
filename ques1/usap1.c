#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>
#include "ioctl.h"

struct Device
{
	int Baudrate;
	int StopBits;
	int Parity;
};

int main()
{
	int fd,Result,STP_BITS = 8,*test;
	unsigned long BAUDRATE= 9600;
	struct Device ToSend={9800,1,0};
	fd=open("/dev/IOCTL",O_RDWR,0777);
	char Ubuff[]="Data from user to kernel";
	char kbuff[100];
	if(fd < 0)
	{
	printf("\n Error not able open file\n");
	exit(1);
	}

	Result = ioctl (fd,SET_BAUDRATE,&BAUDRATE);
	if(Result < 0)
	{
	printf("\nIOCTL ERROR\n");
	return (-1);
	}
	
	printf("\n Baudrate Confirmation =%ld",BAUDRATE);
	test =&STP_BITS;
	ioctl (fd,SET_NON_STOP_BITS,&STP_BITS);
	ioctl (fd,SET_DIRECTION_WRITE,NULL);
	ioctl (fd,DEFAULT_DATA,&ToSend);
	write(fd,Ubuff,sizeof(Ubuff));
	read(fd,kbuff,1000);
	printf("\n Data from kernel buffer = %s \n",kbuff);
	close(fd);
	return 0;
}


