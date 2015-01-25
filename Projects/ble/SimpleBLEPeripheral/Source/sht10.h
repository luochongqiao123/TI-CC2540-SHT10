#include "hal_types.h"

#define BIT0                   (0x0001)
#define BIT1                   (0x0002)
#define BIT2                   (0x0004)
#define BIT3                   (0x0008)
#define BIT4                   (0x0010)
#define BIT5                   (0x0020)
#define BIT6                   (0x0040)
#define BIT7                   (0x0080)


#define STATUS_REG_W 0x06    //000   0011    0
#define STATUS_REG_R 0x07    //000   0011    1
#define MEASURE_TEMP 0x03    //000   0001    1
#define MEASURE_HUMI 0x05    //000   0010    1
#define RESET         0x1e    //000   1111    0
#define bitselect     0x01    //选择温度与湿度的低位读
#define noACK         0
#define ACK           1
#define HUMIDITY      2
#define TEMPERATURE   1
#define SCK           BIT0	//P0.0 = SCK
#define SDA           BIT2	//P0.2 = SDA


#define SCK_H         P0 |= SCK
#define SCK_L         P0 &= ~SCK
#define SDA_H         P0 |= SDA
#define SDA_L         P0 &= ~SDA

#define SDA_IN	   P0DIR &= ~SDA
#define SDA_OUT    P0DIR |= SDA

#define NOP()  asm("NOP")


void SHT10_Init(void);//SHT10 Init
void SHT10_TranStart(void);//SHT10 transport begin
uint8 SHT10_WriteCommand(uint8 command);//write command to sht10
uint8 SHT10_ReadByte(uint8 ack);//read data form SDA
uint8 SHT10_Measurement(uint8* pReceiveBuf,uint8 mode);	//measurement,mode can be Temp or Humi
uint8 SHT10_WriteStatusReg(uint8 RegValue);	//write status register
void SHT10_ConnectReset(void);	//reset the connection
uint8 SHT10_SoftReset(void);	//Soft reset, write reset command to the sensor and the Data in Reg will be erased
								//it should wait for 11 ms to write command again
























