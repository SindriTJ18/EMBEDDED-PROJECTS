#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


//1000000 == 10%
void control(int fd, int maxSpeed, int speed, int reqSpeed){
    float actual_percent = speed/maxSpeed;
    char cmd[100];
    sprintf(cmd,"echo %d > /sys/class/pwm/pwmchip0/pwm0/duty_cycle", reqSpeed);
    system(cmd);
}

void init_motor(int fd){
    fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    write(fd, "0", 1);
    close(fd);
    sleep(1);

    fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    write(fd, "10000000", 8);
    close(fd);
    sleep(1);

    fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    write(fd, "10000000", 8);
    close(fd);
    sleep(1);

    fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    write(fd, "1", 1);
    close(fd);
    sleep(1);
}

int main(){
    // OPEN PARAMS OF KERNEL MODULE FOR READING
    int fd, sz;
    fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    write(fd, "0", 1);
    close(fd);
    sleep(1);
    char *c = (char *) calloc(100, sizeof(char));
    int encVal = 0;
    int prevVal = 0;
    int speed = 0;
    int maxSpeed = 1360;
    int reqSpeed = 1000000;
    printf("Enter refrence speed[percent]: ");
    init_motor(fd);
    control(fd, maxSpeed, speed, reqSpeed);
    while(1){
        usleep(100000);
        fd = open("/sys/module/main4/parameters/encoderCount", O_RDONLY);
        if (fd < 0) { perror("r1"); exit(1); }
        sz = read(fd, c, 10);
        c[sz] = '\0';
        encVal = atoi(c);
        speed =  (encVal - prevVal)*10;
        printf("Encoder value: % s\n", c);
        printf("Speed (PPS): % d\n", speed);
        close(fd);
        prevVal = encVal;
    }
    return 300;
}