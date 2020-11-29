#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/circ_buf.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DD");

#define SIZE 32
static dev_t devnum;      //dev num
static struct cdev _cdev; //represents char dev
static struct circ_buf cbuf;//circularbuffers
static int sample_open(struct inode *inodep,struct file *filep){
        printk("sample open fun\n");
        return 0;
}
static int sample_close(struct inode *inodep,struct file *filep){
        printk("sample close fun\n");
        return 0;
}

static ssize_t sample_read(struct file *filep,char __user *ubuff,size_t cnt,loff_t *offset){
	int i,ret,mini;
	mini=min(cnt,(size_t)CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
        printk("size from userspace for read:%d\n",(int)cnt);
	for(i=0;i<cnt;i++){
		ret=copy_to_user(ubuff+i,cbuf.buf+cbuf.tail,1);
	if(ret){
                printk("error copying to user\n");
		return -EFAULT;
	}
	printk("copied %c to user\n",ubuff[i]);
	cbuf.tail=(cbuf.tail+1)&(SIZE-1);
	}
	

        return i;
}

static ssize_t sample_write(struct file *filep,const char __user *ubuff,size_t cnt,loff_t *offset){
	int ret,i;

        printk("size sent from userspace%d\n",(int)cnt);
	for(i=0;i<cnt;i++){
        ret=copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1);
	if(ret){
		printk("error copying\n");
		return -EFAULT;

	}
	printk("copied from user %c\t",cbuf.buf[cbuf.head]);
	cbuf.head=(cbuf.head+1)&(SIZE-1);
	}
        return i;
}

struct file_operations fops={
        .open =sample_open,
        .release =sample_close,
        .read=sample_read,
        .write=sample_write,
};

static int __init sample_init(void){
        int ret;
        devnum=MKDEV(42,0);// is used for constructing a device num
        ret=register_chrdev_region(devnum,1,"sample_dev");//requesting kernel
        if(ret){
                printk("request denied\n");
                return ret;
        }

        //control :we got device number

        cdev_init(&_cdev,&fops);//binds your cdev with file operations
        
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL);//flag which tells how to allocates memory
	if(!cbuf.buf){
		printk("memory is not allocated\n");
		unregister_chrdev_region(devnum,1);
		return -1;
	}

	ret=cdev_add(&_cdev,devnum,1);  // it adds the device to kernel means the device is alive
        if(ret){
                printk("unable to add cdev to kernel\n");
		kfree(cbuf.buf);
                unregister_chrdev_region(devnum,1);
                return ret;
        }

        printk("Hello world init done\n");
        return 0;
}

static void __exit sample_exit(void){
        cdev_del(&_cdev);
	kfree(cbuf.buf);
        unregister_chrdev_region(devnum,1);
        printk("goodbye\n");
}
module_init(sample_init);
module_exit(sample_exit);

