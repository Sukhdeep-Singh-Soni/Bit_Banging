/*
 * bit_banging.h
 *
 *  Created on: May 24, 2025
 *      Author: deep
 */

#ifndef BIT_BANGING_H_
#define BIT_BANGING_H_

#include <stdint.h>

#define RCC_BASEADDR		(0x40023800UL)
#define RCC_AHB1ENR_OFFSET	(0x30UL)

#define GPIOA_BASEADDR		(0x40020000UL)
#define GPIO_MODER_OFFSET	(0x00UL)
#define GPIO_ODR_OFFSET		(0x14UL)
#define GPIO_IDR_OFFSET		(0x10UL)
#define GPIO_OPTYPE_OFFSET	(0x04UL)
#define GPIO_PIN5			(0x05U)

#define SDA_PIN				(6)
#define SCL_PIN				(7)
#define SDA_HIGH()			(*((uint32_t*)(GPIOA_BASEADDR + GPIO_ODR_OFFSET)) |= (1 << SDA_PIN))
#define SDA_LOW()			(*((uint32_t*)(GPIOA_BASEADDR + GPIO_ODR_OFFSET)) &= ~(1 << SDA_PIN))
#define SCL_HIGH()			(*((uint32_t*)(GPIOA_BASEADDR + GPIO_ODR_OFFSET)) |= (1 << SCL_PIN))
#define SCL_LOW()			(*((uint32_t*)(GPIOA_BASEADDR + GPIO_ODR_OFFSET)) &= ~(1 << SCL_PIN))
#define SDA_READ()			((*((uint32_t*)(GPIOA_BASEADDR + GPIO_IDR_OFFSET)) & (1 << SDA_PIN)) >> SDA_PIN)

/* every function is prefixed with 'c' like cGPioInit() */
void cRccConfig(void);
void cGpioInit(void);
void cGpioTogglePin(uint32_t u32GpioBaseAddr, uint8_t u8PinNumber);
void cDelay(void);
uint8_t cSendDevAddress(uint8_t rw);
void cGenerateStartCondition(void);
void cGenerateStopCondition(void);
void cDelay1(void);
void cDelay2(void);
void cSetSDADirection(uint8_t in_or_out);
uint8_t cCheckAck(void);
uint8_t cWriteRegAddress(uint16_t addr);
uint8_t cSendData(uint8_t data);
uint8_t cSendAckNack(uint8_t ack_or_nack);
uint8_t cReadData(uint8_t *data);

#define SCL_PIN2	(2)
#define SDA_PIN3	(3)
#define SDA_DIR_IN		0
#define SDA_DIR_OUT		1

#define DATA_RD			1
#define DATA_WR			0

#define SDA_SEND_ACK	0
#define SDA_SEND_NACK	1

typedef enum {
	OK	=	0,
	ERR_NACK,
}eReturnStatus;


#endif /* BIT_BANGING_H_ */
