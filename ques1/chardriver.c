// Question 1: IOCTL Functionality

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h> 
#include <linux/fs.h> 
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include "ioctl.h"

#define MYDEVNAME IOCTL

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NIKHILA");
MODULE_DESCRIPTION("User Read/Write operations using IOCTL functionality");

int MYDEVNAME_open(struct inode *inode,struct file *filp);
int MYDEVNAME_release(struct inode *inode,struct file *filp);
ssize_t MYDEVNAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t MYDEVNAME_read(struct file *filp,char __user *Ubuff, size_t count, loff_t *offp);
long MYDEVNAME_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations fops=
{
 	.owner  	  = THIS_MODULE,
 	.open   	  = MYDEVNAME_open,
 	.read   	  = MYDEVNAME_read,
 	.write   	  = MYDEVNAME_write,
 	.unlocked_ioctl   = MYDEVNAME_ioctl,
 	.release	  = MYDEVNAME_release,
 };
 
struct cdev *my_cdev;

struct Device
{
	int Baudrate;
	int StopBits;
	int Parity;
};

struct stats
{
	int size;
	char buff[];
	int r_w;
};


 dev_t Mydevno=0;
 
 //Init Module
static int __init chardevice_init(void)
{

 	int result;
	int MAJOR,MINOR;
 	result=alloc_chrdev_region(&Mydevno,0,1,"IOCTL"); // Dynamic registration
 	MAJOR=MAJOR(Mydevno);
	MINOR=MINOR(Mydevno);
 	printk(KERN_ALERT "\nThis is init function\n");
 	printk(KERN_ALERT "\nit is Major no.= %d and Minor no.=%d\n",MAJOR,MINOR);
 	if(result<0)
	 {
  		printk(KERN_ALERT "\n region is not obtain \n");
 		return(-1);
	 }

	 my_cdev = cdev_alloc();
	 my_cdev ->ops = &fops;
  
  	 result= cdev_add(my_cdev,Mydevno,1);
 
        if(result<0)
 	{
		 printk(KERN_ALERT "\n a char  driver as not been created \n");
	 	 unregister_chrdev_region(Mydevno,1);
 	}
	return 0;
 }
 
//Clean up Module 
void __exit chardevice_exit(void)
{
	int MAJOR,MINOR;
	MAJOR=MAJOR(Mydevno);
	MINOR=MINOR(Mydevno);
	printk(KERN_ALERT "a Major no.= %d and Minor no.=%d",MAJOR,MINOR);
	unregister_chrdev_region(Mydevno,1);
	cdev_del(my_cdev);
	printk(KERN_ALERT "\n Deallocated stuff GOODBYE!! \n");
}


//Open Call
int MYDEVNAME_open(struct inode *inode,struct file *filp)
{
	printk(KERN_ALERT "\nThis is kernel's  Open call..\n");
	return 0;
}

//Write functionality
ssize_t MYDEVNAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp)
{
 	char kbuff[80];
	unsigned long result;
	ssize_t retval;
	result=copy_from_user((char *)kbuff,(char *)Ubuff,count);
	if(result == 0)
	{	
 		printk(KERN_ALERT "\nMessage from User..\n %s \n",kbuff);
 		retval = count;
 		return retval;
        }
	else 
 	{
 		printk(KERN_ALERT "\n Error in Writing the Data\n");
 		retval =-EFAULT;
 		return retval;
	}
 }
 
//Read functionality
ssize_t Name_read(struct file *filp,char __user *Ubuff, size_t count, loff_t *offp)
{
	char kbuff[]="Data from kernel to User..\n";
	unsigned long result;
	ssize_t retval;
	result=copy_to_user((char *)Ubuff,(char*)kbuff,sizeof(kbuff));
	if(result==0)
 	{
	 	printk(KERN_ALERT "\n Data Successfully read\n");
 		retvalue =count;
		return retval;
 	}
	else 
 	{
 		printk(KERN_ALERT "\n Error in writind data to the user\n");
 		retvalue =-EFAULT;
 		return retval;
 	}
}
 

//Close Call
int Name_release(struct inode *inode,struct file *filp)
{
	printk(KERN_ALERT "\nKernel Release call.. Exit of my character driver\n");
	return 0;
}

//IOCTL Function
long MYDEVNAME_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	unsigned int level, STOP_BITS;
	char Ubuff[20];
	struct Device *ToSend;
	printk(KERN_INFO "\nIOCTL Implementation" );
	switch(cmd) {
 		case SET_BAUDRATE:
			get_user (value,(int __user *)arg);
			printk("\n BAUDRATE value = %d",level);
			level=10;
			put_user(value,(int __user *)arg);
			break;
		case SET_DIRECTION_WRITE:
			printk("\n The direction is set to be write\n");
			break;
		case SET_NON_STOP_BITS:
			get_user(STOP_BITS,(int __user *)arg);
			printk("\n STOP_BITS = %d",STOP_BITS);
			break;

		case DEFAULT_DATA:
			ToSend =(struct Device *)Ubuff;
			copy_from_user (Ubuff ,(struct Device *)arg,sizeof(struct Device));
			printk("\n Device Information = %d , %d , %d",ToSend->Baudrate,ToSend->StopBits,ToSend->Parity);
			break;
		default :
			printk("\n Command not found");
			return (-EINVAL);
		}

	return 0;
}

module_init(chardevice_init);
module_exit(chardevice_exit);
