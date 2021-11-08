/* Arduino Serial Command Processor */
#include <Arduino.h>

#define SET_OP 0x01
#define STOP_NODE 0x02
#define SET_PREOP 0x80
#define RESET_NODE 0x81
#define RESET_COMMS 0x82

uint16_t ModRTU_CRC(uint8_t buf[], int len);
void ModRTU_write(char *buf);
void ModRTU_read(char *buf);
void printBuffer(char buf[], int len);

const unsigned char ADDR = 0x02;
const unsigned char READ_FUNC = 0x03;
const unsigned char WRITE_FUNC = 0x06;
uint16_t *reg;
uint8_t lastState = 0x00;

void setup()
{ // called once on start up
  // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8N1);
}

void loop()
{ // loops forever
  const int len = 8;
  char buf[len];
  char msg[len - 2];
  uint16_t crc;
  uint16_t check;
  if (Serial.available() > 0)
  {
    Serial.readBytes(buf, len);
    for (int i = 0; i < len - 2; i++)
    {
      msg[i] = buf[i];
    }
    check = (uint8_t(buf[6]) << 8) + uint8_t(buf[7]);
    crc = ModRTU_CRC(msg, len - 2);
    if (check == crc)
    {
      if (buf[0] == ADDR)
      {
        switch (buf[1])
        {
        case READ_FUNC:
          ModRTU_read(buf);
          break;
        case WRITE_FUNC:
          ModRTU_write(buf);
          break;
        default:
          break;
        }
      }
    }
  }
  switch (reg[0])
  {
  case SET_OP:
    digitalWrite(LED_BUILTIN, HIGH);
    lastState = SET_OP;
    break;
  case STOP_NODE:
    digitalWrite(LED_BUILTIN, LOW);
    lastState = STOP_NODE;
    break;
  case SET_PREOP:

    lastState = SET_OP;
    break;
  case RESET_NODE:

    lastState = RESET_NODE;
    break;
  case RESET_COMMS:

    lastState = RESET_NODE;
    break;
  }
}

uint16_t ModRTU_CRC(uint8_t buf[], int len)
{
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos]; //XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--)
    { //Loop over each bit
      if ((crc & 0x0001) != 0)
      {            //If the LSB is set
        crc >>= 1; //Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else         //Else LSB is not set
        crc >>= 1; //Just shift right
    }
  } //Note, this number has low and high bytes swapped, souse it accordingly(or swap bytes)
  return crc;
}

void ModRTU_write(char *buf)
{
  // uint8_t msbyte = atoi(buf[4]);
  // uint8_t lsbyte = atoi(buf[5]);
  uint8_t regIndex = buf[3];
  // reg[int()] = 255;
  reg[regIndex] = ((uint16_t)(uint8_t)buf[4] << 8) | (uint8_t)buf[5];
  printBuffer(buf, 8);
  // Serial.print(char(uint16_t(buf[4] << 8)) >> 8);
  // for(int i = 0; i < 8; i++){
  // Serial.print(buf[i]);
  // }
}

void ModRTU_read(char *buf)
{
  uint8_t regIndex = buf[3];
  char lo = reg[regIndex] & 0xFF;
  char hi = reg[regIndex] >> 8;
  char bytes = buf[5] * 2;
  char readBuf[5] = {buf[0], buf[1], bytes, hi, lo};
  /*
  Serial.print(buf[0]);
  Serial.print(buf[1]);
  Serial.print(buf[5]);
  Serial.print(hi);
  Serial.print(lo);
  */
  uint16_t crc = ModRTU_CRC(readBuf, 5);
  char crc1 = crc >> 8;
  char crc2 = crc & 0xFF;
  printBuffer(readBuf, 5);
  Serial.print(crc1);
  Serial.print(crc2);

  // printBuffer(buf, 8);
}

void printBuffer(char buf[], int len)
{
  for (int i = 0; i < len; i++)
  {
    Serial.print(buf[i]);
  }
}