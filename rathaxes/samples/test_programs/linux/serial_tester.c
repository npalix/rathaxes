#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    int     rd;
    char    buff[10];

    fd = open("/dev/my_rs232", O_RDWR);
    if (fd == -1)
    {
        perror("could not open file.");
        return 0;
    }
    rd = read(fd, buff, 10);
    if (rd == -1)
    {
        perror("could not read file.");
        return 0;
    }
    printf("%s\n", buff);

    return 0;
}
