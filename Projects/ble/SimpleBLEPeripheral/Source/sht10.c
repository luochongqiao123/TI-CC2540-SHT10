#include "sht10.h" 
#include "ioCC2541.h"
#include "UartDebug.h"

//#define UARTDEBUG 0

/*
	Init the SHT10.
	config the Port
*/
void SHT10_Init(void)
{
	P0SEL &= ~(SCK+SDA);//config as gen IO
	P0DIR |= (SCK+SDA);//sck and sda is output

	P2INP &= ~BIT5;	//port0 input mode config as pullup
	P0INP &= ~(SCK+SDA);//SCK and SDA is pullup

}

/*
	SHT10 Transport Strat Operation
*/
void SHT10_TranStart(void)
{
	SDA_OUT;
	SCK_L;NOP();	
	SDA_H;NOP();	//before the start,pullup SDA and pulldown SCK
	
	SCK_H;NOP();
	SDA_L;NOP();			//pulldown SDA while the SCK is High
	SCK_L;NOP();NOP();NOP();	//wait for a SCK Clock
	SCK_H;NOP();			
	SDA_H;NOP();			//then pullup SDA while SCK is High
	SCK_L;NOP();			
	//SDA_L;NOP();
	SDA_IN;					//release the SDA

}

/*
	Write Command to SHT10,like Temp measurement,Humi Measurement and Write Register
*/
uint8 SHT10_WriteCommand(uint8 command)
{
	SDA_OUT;
	SCK_L;	//prepare for the Data write

	for(uint8 i = 0x80;i>0;i/=2){
		if(command & i)
			SDA_H;
		else
			SDA_L;
		NOP();
		SCK_H;
		NOP();NOP();NOP();
		SCK_L;
	}
	SDA_IN;	//after the Data write ,release the SDA

	NOP();
	SCK_H;
	NOP();
	SCK_L;		//one more clk and wait the ack
	NOP();
	
	
	/*while (P0 & SDA);
	HalUartPrint("Measurement Done");
	if(P0 & SDA)
		HalUartPrint("SDA = 1");
	else
		HalUartPrint("SDA = 0");*/

	if(P0 & SDA)
		return ACK;
	else
		return noACK;
}


/*
	read byte from SHT10 after write command to the seneor
	
*/
uint8 SHT10_ReadByte(uint8 ack)
{
	uint8 val= 0;
	
	SDA_IN;	//release the SDA to Receive the Data
	
	#ifdef UARTDEBUG
	HalUartPrint("Packet Receive");
        #endif
	
	SCK_L;NOP();
	
	for(uint8 i = 0x80;i>0;i/=2){
		SCK_H;
		if(P0 & SDA){
			val = val | i;
			#ifdef UARTDEBUG
			HalUartPrint(" 1");
			#endif
		}
		#ifdef UARTDEBUG
		else
			HalUartPrint(" 0");
		#endif
		SCK_L;
	}

	NOP();

	SDA_OUT;
	
	if(ack)		//after receive the Data,write Ack or noAck to deceide to receivce more Data or not
		SDA_L;
	else
		SDA_H;
	NOP();
	SCK_H;
	NOP();NOP();NOP();
	SCK_L;
	SDA_IN;

	return val;
	
}


/*
	measurement,mode can be temp or humi.
*/
uint8 SHT10_Measurement(uint8* pReceiveBuf,uint8 mode)
{
	uint8 error = 0;  //code that cheakout whether the WriteCommand has ACK

	SHT10_TranStart();
	switch(mode)
	{
		case MEASURE_TEMP : error += SHT10_WriteCommand(MEASURE_TEMP);
		case MEASURE_HUMI : error += SHT10_WriteCommand(MEASURE_HUMI);
	}

	//if(error) HalUartPrint("Error!");
	while(P0 & SDA);
	
	#ifdef UARTDEBUG
	HalUartPrint("Measurement Done");
	#endif
	
	*(pReceiveBuf) = SHT10_ReadByte(ACK);
	*(pReceiveBuf+1) = SHT10_ReadByte(noACK);

	return error;
}


/*
	write status reg
*/
uint8 SHT10_WriteStatusReg(uint8 RegValue)
{
	uint8 error = 0;
	SHT10_TranStart();	
	error += SHT10_WriteCommand(STATUS_REG_W);
	error += SHT10_WriteCommand(RegValue);
	return error;
}

/*
	reset the connection,first transmit 9 clk in SCK while SDA = 1;
	then give the sensor a TranStart
*/
void SHT10_ConnectReset(void)
{
	SDA_OUT;
	SDA_H;SCK_L;
	for(int ClkCount = 0;ClkCount<9;ClkCount++){
		SCK_H;NOP();
		SCK_L;NOP();
	}
	SHT10_TranStart();
}

/*
	Soft reset, write reset command to the sensor and the Data in Reg will be erased
	it should wait for 11 ms to write command again
*/
uint8 SHT10_SoftReset(void)
{
	uint8 error = 0;
	SHT10_ConnectReset();
	error += SHT10_WriteCommand(RESET);
	return error;
}























