#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "ioctl_ui.h"


int main(int ac, char** av)
{
    int fd;
    int ret;
    int size;
    char    buff[256];

    if (ac != 2)
    {
        printf("speify the path please\n");
        return -1;
    }

    printf("openning %s...\n", av[1]);
    fd = open(av[1], O_RDWR);
    if (fd == -1)
    {
        perror("open failed");
        return -1;
    }
    printf("file descriptor: %d\nioctl\n", fd);

    printf("write\n");
    ret = write(fd, "plop & co", 9);
    printf("write return %d\n", ret);
    buff[read(fd, buff, 20)] = 0;
    printf("just readed<%s>\n", buff);

    ioctl(fd, MD_GET_TSIZE, &size);
    printf("buffer total size %d\n", size);

    ioctl(fd, MD_SET_TSIZE, 15);
    ret = write(fd, "123456789012345", 15);
    printf("write return %d\n", ret);
    buff[read(fd, buff, 4)] = 0;
    printf("just readed<%s>\n", buff);
    buff[read(fd, buff, 10)] = 0;
    printf("just readed<%s>\n", buff);
    printf("flush (ioctl)\n");
    ioctl(fd, MD_FLUSH);
    buff[read(fd, buff, 20)] = 0;
    printf("just readed<%s>\n", buff);

    printf("** bad address **\n");
    ret = write(fd, "bon bah on va pas raconter un roman , c'est juste pour faire long, comme Jack\n", 77);
    perror("bon c'est fait expres");
    buff[read(fd, buff, 80)] = 0;
    printf("just readed<%s>\n", buff);

    return 0;
}
