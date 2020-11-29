#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/cdev.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DD");
static dev_t devnum;      //dev num
static struct cdev _cdev; //represents char dev

static int sample_open(struct inode *inodep,struct file *filep){
        printk("sample open fun\n");
        return 0;
}
static int sample_close(struct inode *inodep,struct file *filep){
        printk("sample close fun\n");
        return 0;
}

static ssize_t sample_read(struct file *filep,char __user *ubuff,size_t cnt,loff_t *offset){
        printk("sample read\n");
        return 0;
}

static ssize_t sample_write(struct file *filep,const char __user *ubuff,size_t cnt,loff_t *offset){
        printk("sample write\n");
        return 0;
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

        ret=cdev_add(&_cdev,devnum,1);  // it adds the device to kernel means the device is alive
        if(ret){
                printk("unable to add cdev to kernel\n");
                unregister_chrdev_region(devnum,1);
                return ret;
        }

        printk("Hello world init done\n");
        return 0;
}

static void __exit sample_exit(void){
        cdev_del(&_cdev);
        unregister_chrdev_region(devnum,1);
        printk("goodbye\n");
}
module_init(sample_init);
module_exit(sample_exit);


