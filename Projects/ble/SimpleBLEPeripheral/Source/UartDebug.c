#include "UartDebug.h"
#include "hal_uart.h"
#include "OnBoard.h"

void HalUartInit(void)
{
	halUARTCfg_t uartConfig;
	/*Uart Config*/
	uartConfig.configured = TRUE;
	uartConfig.callBackFunc = NULL;
	uartConfig.baudRate=HAL_UART_BR_115200;
	uartConfig.flowControl = HAL_UART_FLOW_OFF;
	uartConfig.flowControlThreshold = MT_UART_THRESHOLD;
	uartConfig.tx.maxBufSize = MT_UART_TX_BUFF_MAX;
	uartConfig.rx.maxBufSize = MT_UART_RX_BUFF_MAX;
	uartConfig.intEnable = TRUE;
	uartConfig.idleTimeout = MT_UART_IDLE_TIMEOUT;
	/*Open Uart Port*/
	HalUARTOpen(HAL_UART_PORT_0, &uartConfig);
}

void HalUartPrint(uint8 * str)
{
	HalUARTWrite(HAL_UART_PORT_0, str, osal_strlen(str));
	HalUARTWrite(HAL_UART_PORT_0, "\r\n", 2);
}











