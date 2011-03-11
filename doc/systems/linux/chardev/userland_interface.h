#ifndef USERLAND_INTERFACE_H
#define USERLAND_INTERFACE_H

#include <linux/cdev.h>
#include <linux/types.h>

#include "ioctl_ui.h"

#define DEVICE_NAME "my_driver"

struct my_dev
{
    char            *buffer;
    int             size;
    int             tsize;
    struct cdev     cdev;
};

//  functions prototypes
int my_driver_open(struct inode *inode, struct file *filp);
static int __init my_driver_init(void);
static void __exit  my_driver_exit(void);
ssize_t my_driver_write(struct file *filp, const char __user *user_buff, size_t size, loff_t *offp);
ssize_t my_driver_read(struct file *filp, char __user *user_buff, size_t size, loff_t *offp);
int     my_driver_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
int my_device_set_size(struct my_dev *devstruct, int new_size);
#endif  // USERLAND_INTERFACE_H
