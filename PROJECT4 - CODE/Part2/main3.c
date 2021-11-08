#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>

int main()
{
    //Enable gpios
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "17", 2);
    close(fd);

    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "5", 1);
    close(fd);

    //Set gpios as input and output
    fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    fd = open("/sys/class/gpio/gpio5/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    //Set gpio17 interrupt
    fd = open("/sys/class/gpio/gpio17/edge", O_WRONLY);
    write(fd, "both", 4);
    close(fd);

    //Wait for event
    fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);
    int fo = open("/sys/class/gpio/gpio5/value", O_WRONLY);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLPRI;

    while(1)
    {
        lseek(fd, 0, SEEK_SET);
        int ret = poll(&pfd, 1, 3000);
        char c;
        read(fd, &c, 1);
        if(ret == 0)
            printf("Timeout\n");
        else
            if(c == '0'){
                write(fo, "0", 1);
                printf("Push\n");
            }
            else{
                write(fo, "1", 1);
                printf("Release\n");
            }
    }
    close(fo);
    close(fd);

    //Disable gpio17
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "17", 2);
    close(fd);

    return(0);
}