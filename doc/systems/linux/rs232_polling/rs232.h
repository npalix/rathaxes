#ifndef RS232_H_
#define RS232_H_



#include <linux/errno.h>          //errors code
#include <linux/module.h>   // THIS_MODULE macro
#include <linux/kernel.h>   // printfk function
#include <linux/init.h>     // module_init & module_exit __init __exit macros
#include <linux/fs.h>       // register_chrdev_region, struct file_operation
#include <linux/types.h>    // dev_t type
#include <linux/kdev_t.h>   // MAJOR and MINOR macro
#include <linux/ioctl.h>    // ioctl relatives commands _IO _IOR _IOW ...
#include <linux/cdev.h>     // chardev structure and function
#include <asm/uaccess.h>    // function to acces userland
#include <linux/ioport.h>   // ioports registration request_region
#include <asm/io.h>
#include "rs232_define.h"



#define MY_NAME "[RS232] "
#define dbg(fmt, arg...)                            \
    do{                                             \
        if (debug)                                  \
        printk (KERN_DEBUG "%s: %s: " fmt "\n",     \
                MY_NAME , __FUNCTION__ , ## arg);   \
    } while (0)
//
// custom structure for serial driver
typedef struct rs232_s
{
    char *device_name;
    int iobase;
    struct  resource *resource;
    struct cdev cdev;
}   rs232_t;
/*
struct cdev {
	struct kobject kobj;
	struct module *owner;
	const struct file_operations *ops;
	struct list_head list;
	dev_t dev;
	unsigned int count;
};
*/
static int debug = 1;

// usefull functions
int    write_one(rs232_t *dev, char word);
unsigned char     read_one(rs232_t *dev);
void    setreg(unsigned char value, unsigned char mask, int ioport);

// callbacks
ssize_t rs232_read(struct file *filp, char __user *ubuff, size_t size, loff_t *offset);
ssize_t rs232_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *offset);
int rs232_open(struct inode *inode, struct file *file);
int rs232_release(struct inode *inode, struct file *file);
int rs232_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
    unsigned long arg);

//  entry and exit point
int __init   rs232_init(void);
void __exit    rs232_exit(void);


#endif //RS232_H_
