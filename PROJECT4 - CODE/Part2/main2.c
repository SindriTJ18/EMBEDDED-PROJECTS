#include <linux/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>

int main(){
    // Export the desired pin by writing to /sys/class/gpio/export
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/export");
        // exit(1);
    }

    if (write(fd, "17", 2) != 2) {
        perror("Error writing to /sys/class/gpio/export");
        // exit(1);
    }
    close(fd);
    
    fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio17/direction");
        // exit(1);
    }

    if (write(fd, "in", 2) != 2) {
        perror("Error writing to /sys/class/gpio/gpio17/direction");
        // exit(1);
    }
    close(fd);

    //
    //
    // Output pin  $#$#$#$#$#$#$#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/export");
        // exit(1);
    }

    if (write(fd, "5", 1) != 1) {
        perror("Error writing to /sys/class/gpio/export");
        // exit(1);
    }
    close(fd);
    
    fd = open("/sys/class/gpio/gpio5/direction", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio5/direction");
        // exit(1);
    }

    if (write(fd, "out", 3) != 3) {
        perror("Error writing to /sys/class/gpio/gpio5/direction");
        // exit(1);
    }

    close(fd);
    fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio17/direction");
        // exit(1);
    }
    char buf[1];
    int ret;

    while(1){
        fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);
        ret = read(fd, buf, sizeof(buf));
        close(fd);
        fd = open("/sys/class/gpio/gpio5/value", O_WRONLY);
        if (buf[0] == '1'){
            printf("dogs %c \n", buf[0]);
            write(fd, "1", 1);
        }
        else{
            write(fd, "0", 1);
        }
        close(fd);
    }
    return 100;
}
