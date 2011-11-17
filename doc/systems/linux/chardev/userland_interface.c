/************************************************/
/*  This device store a string on write         */
/*  and give it back on read. using ioctl       */
/*  it is possible to change the buffer size    */
/*  and tu flush it                             */
/*  The device will remeber the content of his  */
/*  buffer accross different open               */
/************************************************/

#include <linux/module.h>
#include <linux/kernel.h>   //container_of
#include <linux/init.h>
#include <linux/fs.h>       // chardev functions, file_operations
#include <linux/types.h>    // dev_t, ...
#include <linux/cdev.h>     // cdev structure and related functions
#include <asm/uaccess.h>    // copy_to/from_user, put/get_user

#include "userland_interface.h"

struct my_dev   *datas;
struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = my_driver_open,
    .write = my_driver_write,
    .read = my_driver_read,
    .ioctl = my_driver_ioctl
};

int     my_driver_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct my_dev   *devstruct;
    int             ret = 0;
    
    //  getting the specific structure
    devstruct = filp->private_data;
    switch (cmd)
    {
        case MD_GET_TSIZE:
            put_user(devstruct->tsize, (int *)arg);
            break;
        case MD_SET_TSIZE:
            ret = my_device_set_size(devstruct, arg);
            break;
        case MD_FLUSH:
            //  verify the user has the right to perform io
            if (!capable(CAP_SYS_RAWIO))
                return -EPERM;
            devstruct->size = 0;
            break;
        default:
            return -EINVAL;     // invalid command supplied
    }
    return ret;
}


int my_device_set_size(struct my_dev *devstruct, int new_size)
{
    //  realloc a buffer at the size requested by the user
    kfree (devstruct->buffer);
    devstruct->buffer = kmalloc(sizeof(char) * new_size +1, GFP_KERNEL);
    if (!devstruct->buffer)
    {
        return -ENOMEM;
    }
    devstruct->tsize = new_size;
    return 0;
}

ssize_t my_driver_read(struct file *filp, char __user *user_buff, size_t size, loff_t *offp)
{
    int             ret;
    struct my_dev   *devstruct;
    
    //  getting the specific structure
    devstruct = filp->private_data;

    //  read only present data
    if (size > devstruct->size)
        size = devstruct-> size;
    ret = copy_to_user(user_buff, devstruct->buffer, size);
    if (ret)
    {
        //  userland corrupt pointer => segfault
        return -EFAULT;
    }
    //  if everything is ok return the size readed
    return size;
}

ssize_t my_driver_write(struct file *filp, const char __user *user_buff, size_t size, loff_t *offp)
{
    int             ret;
    struct my_dev   *devstruct;
    
    //  getting the specific structure
    devstruct = filp->private_data;

    if(size > devstruct->tsize)
    {
        //  trying to copy outside the buffer => segfault
        return -EFAULT;
    }
    ret = copy_from_user(devstruct->buffer, user_buff, size);
    if (ret)
    {
        //  userland corrupt pointer => segfault
        return -EFAULT;
    }
    devstruct->size = size;
    devstruct->buffer[size] = '\0';
        //  if everything is ok return the size readed
    return size;
}

int my_driver_open(struct inode *inode, struct file *filp)
{
    //  no particular initialisation to be made in this case

    //  make the device specific function available everywhere
    struct my_dev   *devstruct;
    
    devstruct = container_of(inode->i_cdev, struct my_dev, cdev);
    filp->private_data = devstruct;
    return 0;
}

static int __init my_driver_init(void)
{
    dev_t       devicenumber;
    int         minor;
    
    // preparing the device specific data structure setting default parameters
    datas = kmalloc(sizeof(*datas), GFP_KERNEL);
    if (!datas)
        return -ENOMEM;
    datas->buffer = kmalloc(sizeof(char) * 42 + 1, GFP_KERNEL);
    if (!datas->buffer)
        return -ENOMEM;
    datas->size = 0;
    datas->tsize = 42;
    
    //  registering the char dev number (automatic method)
    minor = 0;
    if (alloc_chrdev_region(&devicenumber, minor, 1, DEVICE_NAME))
    {
        //  cannot register a char device number
        return -1;
    }

    //  registering the device
    cdev_init(&datas->cdev, &fops);
    if (cdev_add(&datas->cdev, devicenumber, 1))
    {
        //  the device cannot be added leaving...
        return -1;
    }
    datas->cdev.owner = THIS_MODULE;

    return 0;
}


static void __exit  my_driver_exit(void)
{
        //  release the char dev number
    unregister_chrdev_region(datas->cdev.dev, 1);
    //  release the deivce
    cdev_del(&datas->cdev);
}

module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("rathaxes");
