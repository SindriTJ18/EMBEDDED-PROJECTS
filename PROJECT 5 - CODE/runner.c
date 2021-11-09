/* Simple send and receive C example (line-mode terminal program with local echo) 
* for communicating with the Arduino using /dev/ttyS0. */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>


uint16_t ModRTU_CRC(uint8_t buf[], int len);
uint16_t modCom(uint16_t buf[], int len, int file);
void preOp(int file);

int main(int argc, char *argv[]){
    int file;
    /*
    if(argc!=5){
        printf("Invalid number of arguments, expected: 5 got: %d exiting!\n", argc);
        return -2;
    }
    */
    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
        perror("UART: Failed to open the file.\n");
        return -1;
    }
    
   struct termios options;
   tcgetattr(file, &options);
   cfmakeraw(&options);         // set serial port to raw mode for binary comms

   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;

   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

    const size_t MSG_LEN = 4;
    preOp(file);
    // populate the message with integer values in binary format
    /*
    for (int i = 0; i < MSG_LEN; i++){
        msg16[i] = atoi(argv[i+1]);
    }
    */

    // modCom(msg16, MSG_LEN, file);
    uint16_t msg16[4] = {2, 3, 2, 1};
    uint16_t data;
    while(1){
        data = modCom(msg16, 4, file);
        msg16[0] = 1;
        msg16[1] = 6;
        msg16[2] = 1;
        msg16[3] = data;
        modCom(msg16, 4, file);
        msg16[0] = 2;
        msg16[1] = 3;
        msg16[2] = 2;
        msg16[3] = 1;

        usleep(50000);
    }


    close(file);
    return 0;
}

uint16_t ModRTU_CRC(uint8_t buf[], int len){
    uint16_t crc=0xFFFF;

    for(int pos = 0; pos < len; pos++){
        crc ^= (uint16_t)buf[pos];      //XOR byte into least sig. byte of crc
        for(int i = 8; i != 0; i--){    //Loop over each bit
            if((crc&0x0001)!=0){        //If the LSB is set
                crc >>= 1;              //Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                        //Else LSB is not set
                crc >>= 1;              //Just shift right
            }
        }                               //Note, this number has low and high bytes swapped, souse it accordingly(or swap bytes)
    return crc;
}

uint16_t modCom(uint16_t msg16[], int len, int file){
    int count;
    uint8_t msg8[2*(len-1)];
    msg8[0] = msg16[0];
    msg8[1] = msg16[1];
    for (int i = 2; i < len; i++){
        msg8[2*i - 2] = msg16[i] >> 8;
        msg8[2*i - 1] = msg16[i] & 0x00FF;
    }
    uint16_t crc = ModRTU_CRC(msg8, 6);
    uint8_t msg[2*len]; 
    for (int i = 0; i < 2*(len - 1); i++){
        msg[i] = msg8[i];
    }
    msg[2*len - 2] = crc >> 8;
    msg[2*len - 1] = crc;
    // send the string plus the null character
    if ((count = write(file, msg, 2*len)<0)){
        perror("Failed to write to the output\n");
        return -1;
    }
    else{
        printf("Send request:   ");
        for (int i = 0; i < 2*len; i++){
            printf("%02x ", msg[i]);
        }
        printf("\n");
    }

    usleep(100000);

    uint8_t receive[100];

    if ((count = read(file, (void *)receive, 100))<0){
        perror("Failed to read from the input\n");
        return -1;
    }

    if (count==0) printf("There was no data available to read!\n");
    else {
        receive[count]=0;  //There is no null character sent by the Arduino
        printf("Received reply: ");
        for (int i = 0; i < count; i++){
            printf("%02x ", receive[i]);
        }
        printf("\n");
    }
    uint16_t data = (receive[3] << 8) | receive[4];
    return data;
}

void preOp(int file){
    uint16_t msg16[4] = {1, 6, 0, 128};
    modCom(msg16, 4, file);
    msg16[0] = 2;
    modCom(msg16, 4, file);
    msg16[3] = 1;
    modCom(msg16, 4, file);
    msg16[0] = 1;
    modCom(msg16, 4, file);
}