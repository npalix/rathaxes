#include <linux/delay.h>
#include "rs232.h"


extern struct file_operations   fops;
extern rs232_t rsdev;




ssize_t rs232_read(struct file *filp, char __user *ubuff, size_t size, loff_t *offset)
{
    char*   kbuff;
    int     ret;
    int     cnt = 0;
    rs232_t *dev;

    dbg();
    kbuff = kmalloc(sizeof(*kbuff) * size + 1, GFP_KERNEL);
    if(!kbuff)
    {
        dbg("not enought memory !");
        return (-ENOMEM);
    }

    dev = filp->private_data;
    //  blocking io if there is nothing to read wait...
    while((!(inb(dev->iobase + LSR) & LSR_DATA_AVAILABLE)))
        msleep(100); // wait 10 micro seconds
    while(cnt < size && (inb(dev->iobase + LSR) & LSR_DATA_AVAILABLE))
    {
        kbuff[cnt] = inb(dev->iobase);
        dbg("read one char %c", kbuff[cnt]);
        cnt++;
    }
    kbuff[cnt] = '\0';
    dbg("kernel readed buffer <%s>", kbuff);

    ret = copy_to_user(ubuff, kbuff, size);
    if (ret)
    {
        dbg("\tERROR\t corrupt userland pointer");
        kfree(kbuff);
        return (-1);
    } 

    kfree(kbuff);
    return(cnt);
}



ssize_t rs232_write(struct file *filp, const char __user *ubuff, size_t size, loff_t *offset)
{
    char*   kbuff;
    int     ret;
    int     cnt = 0;
    rs232_t *dev;
    dbg();


    kbuff = kmalloc(sizeof(*kbuff) * size + 1, GFP_KERNEL);
    if (!kbuff)
    {
        dbg("not enought memory !");
        return (-ENOMEM);
    }
    ret = copy_from_user(kbuff, ubuff, size);
    if (ret)
    {
        dbg("\tERROR\t corrupt userland pointer");
        kfree(kbuff);
        return (-1);
    }
    dev = filp->private_data;
    // pour l'affichage du debug
    kbuff[size] = '\0';
    dbg("got buffer from userland <%s>", kbuff);
    dbg("write @ 0x%x", dev->iobase + THR);

    // blocking io if the line is not read y we waite
    while (!(inb(dev->iobase + LSR) && LSR_THR_IDL))
        msleep(100);
    while ((cnt < size) && (inb(dev->iobase + LSR) && LSR_THR_EMPTY))
    {
        outb(*(kbuff + cnt), dev->iobase + THR);
        cnt++;
        msleep(5);
    }
    dbg("end");
    kfree(kbuff);
    return(cnt);
}


/*
   choosed (from ioctl-numbers) ioctl magic number = 0xf5
*/
int rs232_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
    unsigned long arg)
{
    rs232_t *dev;
    rsconf conf;
    int retval = 0;

    dev = filp->private_data;
    dbg();
    switch (cmd)
    {
        case    RS_BAUDRATESET:
            if (!capable(CAP_SYS_RAWIO))
                return -EPERM;
            dbg("set baudrate value: 0x%lx", arg);
            setreg(LCR_DLAB_LATCH_MODE, LCR_DLAB, dev->iobase + LCR);
            outb(arg & 0x00FF, dev->iobase + DLL);
            outb((arg & 0xFF00)>>4, dev->iobase + DLM);
            setreg(LCR_DLAB_RW_MODE, LCR_DLAB, dev->iobase + LCR);
            break;
        case    RS_WORDLENGTHSET:
            if (!capable(CAP_SYS_RAWIO))
                return -EPERM;
            dbg("set word length value: 0x%lx", arg);
            setreg(arg, LCR_WDL, dev->iobase + LCR);
            break;
        case    RS_BAUDRATEGET:
            dbg("get baudrate value: 0x%lx", arg);
            setreg(LCR_DLAB_LATCH_MODE, LCR_DLAB, dev->iobase + LCR);
            retval = inb(dev->iobase + DLM);
            retval <<= 4;
            retval += inb(dev->iobase + DLL) & 0x00FF;
            setreg(LCR_DLAB_RW_MODE, LCR_DLAB, dev->iobase + LCR);
            break;
        case    RS_CONFSET:
            if (!capable(CAP_SYS_RAWIO))
                return -EPERM;
            get_user(conf.wl, &((rsconf*)arg)->wl);
            get_user(conf.div, &((rsconf*)arg)->div);
            dbg("set word length value: 0x%lx", arg);
            setreg(conf.wl, LCR_WDL, dev->iobase + LCR);
            dbg("set baudrate value: 0x%lx", conf.wl);
            setreg(LCR_DLAB_LATCH_MODE, LCR_DLAB, dev->iobase + LCR);
            outb(conf.div & 0x00FF, dev->iobase + DLL);
            outb((conf.div & 0xFF00)>>4, dev->iobase + DLM);
            setreg(LCR_DLAB_RW_MODE, LCR_DLAB, dev->iobase + LCR);

        default:
            return -EINVAL;

    }
    return retval;
}

int rs232_open(struct inode *inode, struct file *filp)
{
    rs232_t *dev;

    dbg();
    // get my private structure
    dev = container_of(inode->i_cdev, rs232_t, cdev);
    filp->private_data = dev;
    dbg("private data<%s>", ((rs232_t*)filp->private_data)->device_name);

    //
    // serial reinitialisation
    //

    //  set baudrate@115200
    dbg("+---------------------------------------------------------");
    setreg(LCR_DLAB_LATCH_MODE, LCR_DLAB, dev->iobase + LCR);
    dbg("|\tregister LCR@0x%x = 0x%x", dev->iobase, inb(dev->iobase + LCR));
    outb(DLL_9600, dev->iobase + DLL);
    dbg("|\tregister DLL@0x%x = 0x%x", dev->iobase, inb(dev->iobase + DLL));
    outb(DLM_9600, dev->iobase + DLM);
    dbg("|\tregister DLM@0x%x = 0x%x", dev->iobase, inb(dev->iobase + DLM));

    //  8 bit word 1 stop bit no parity and break disabled
    setreg(LCR_7_BIT_WORD | LCR_1_STOP_BIT | LCR_NO_PARITY
            | LCR_BREAK_SIG_DISABLE | LCR_DLAB_RW_MODE,
            LCR_WDL | LCR_STP | LCR_PAR | LCR_BRK | LCR_DLAB,
            dev->iobase + LCR);
    dbg("|\tregister LCR@0x%x = 0x%x", dev->iobase, inb(dev->iobase + LCR));

    setreg(0x01,0xff, dev->iobase + FCR);
    dbg("|\tregister FCR@0x%x = 0x%x", dev->iobase + FCR, inb(dev->iobase + FCR));
    dbg("+---------------------------------------------------------");

    //  no interrupt available
    outb(IER_NONE, dev->iobase + IER);

    return(0);
}


//
//  m r v   ~m&r|v
//  0 0 0   0
//  0 0 1   1   => err (value not corresponding to mask - should never happend)
//  0 1 0   1
//  0 1 1   1
//
//  1 0 0   0
//  1 0 1   1
//  1 1 0   0
//  1 1 1   1
//
//  ~m&r|v = newvalue
//

void    setreg(unsigned char value, unsigned char mask, int ioport)
{
    unsigned char   new_reg = 0;
    unsigned char   reg;

    reg = inb(ioport); 
    new_reg = ((~mask&reg)|value);
    dbg("setting register@0x%x\n"
            "\t\tmask\t0x%x\n"
            "\t\treg\t0x%x\n"
            "\t\tvalue\t0x%x\n"
            "\t\t new reg\t0x%x\n",
            ioport,
            mask,
            reg,
            value,
            new_reg);
    outb(new_reg, ioport);
}


int rs232_release(struct inode *inode, struct file *filp)
{
    return(0);
}

