 #include "rs232.h"

// personal structure containing all neeed information
rs232_t rsdev;

//extern struct file_operations   fops;
struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = rs232_read,
    .write = rs232_write,
    .open = rs232_open,
    .release = rs232_release,
    .ioctl = rs232_ioctl
};


int __init   rs232_init(void)
{
    dev_t   rsdevnbr; // uint32
    unsigned int    rsminor = 0;
    unsigned int    rsnbr = 1;
    char            *rsname = "rathaxes_rs232";
    int     ret;
    struct  resource *rsioport;


    dbg();

    //  This part iss only checking and have no effect on the system (except to
    //  write in /proc/ioports)
    rsdev.iobase = 0x3f8; // serial normal ioport
    dbg("request_region(rsdev.iobase[%x], 8, rsdev.device_name[%s])", rsdev.iobase, rsname);
    rsioport = request_region(rsdev.iobase, 8, rsname);
    dbg("rsioport addrsee: 0x%p\n", rsioport);
    rsdev.resource = rsioport;
    if (!rsioport)
    {
        dbg("ioport aleready in use !");
        return (-ENODEV);
    }
    else
        dbg("nice ioport register");
    //dbg("resource\n"
    //        "\tstart %x\n"
    //        "\tend %x\n"
    //        "\tname %s\n"
    //        "\tflag %ld",
    //        rsioport->start,
    //        rsioport->end,
    //        rsioport->name,
    //        rsioport->flags
    //        );


    // get a major dynamically
    ret = alloc_chrdev_region(&rsdevnbr, rsminor, rsnbr, rsname);
    if (ret)
    {
        dbg("\tERROR\tchardev alloc returned %d", ret);
        return(-1);
   }
    dbg("major %d\tminor %d", MAJOR(rsdevnbr), MINOR(rsdevnbr));


    // setup the cdev struct inside my personal rsdev
    cdev_init(&(rsdev.cdev), &fops); 
    rsdev.device_name = rsname;
    rsdev.cdev.owner = THIS_MODULE; 
    rsdev.cdev.dev = rsdevnbr;
    rsdev.cdev.count = rsnbr;
    // register the chardev 
    ret = cdev_add(&rsdev.cdev, rsdev.cdev.dev, rsdev.cdev.count);
    if (ret < 0)
    {
        dbg("cannot add cdev in kernell !!");
        return (-1);
    }
    return (0);
}


void __exit    rs232_exit(void)
{
    dbg("unregister_chrdev_region(devnbr[%u], count[%u]", rsdev.cdev.dev, rsdev.cdev.count);
    cdev_del(&rsdev.cdev);
    unregister_chrdev_region(rsdev.cdev.dev, rsdev.cdev.count);
    dbg("release region(%x, %d)",rsdev.resource->start, rsdev.cdev.count);
    release_region(rsdev.resource->start, 8);
}

module_init(rs232_init);
module_exit(rs232_exit);
