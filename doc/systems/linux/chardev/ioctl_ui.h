#ifndef IOCTL_UI_H
#define IOCTL_UI_H

#include <linux/ioctl.h>

//  IOCTL defines
#define MD_IOCTL_MAGIC  0xf5
#define MD_GET_TSIZE     _IOR(MD_IOCTL_MAGIC, 1, int)
#define MD_SET_TSIZE     _IOW(MD_IOCTL_MAGIC, 2, int)
#define MD_FLUSH        _IO(MD_IOCTL_MAGIC, 3)

#endif  // IOCTL_UI_H
