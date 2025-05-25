/*
 * bit_banging.c
 *
 *  Created on: May 24, 2025
 *      Author: deep
 */

#include "bit_banging.h"

void cGenerateStopCondition(void) {
	cSetSDADirection(SDA_DIR_OUT);
	SCL_LOW();
	SDA_LOW();
	SCL_HIGH();
	cDelay1();
	SDA_HIGH();
	cDelay1();
	SCL_LOW();
}

//~1us delay
void cDelay1(void) {
	__asm volatile ("NOP");	//no operation

}

void cGenerateStartCondition(void) {
	cSetSDADirection(SDA_DIR_OUT);
	SDA_HIGH();
	SCL_HIGH();
	cDelay1(); //1us
	SDA_LOW();
	cDelay1(); //1us
	SCL_LOW();
	cDelay1(); //1us
}

uint8_t cSendDevAddress(uint8_t rw) {
	cSetSDADirection(SDA_DIR_OUT);
	uint8_t addr = 0xA0;
	addr |= (rw << 0);
	for (int i = 7; i >= 0; i--) {
		if (addr & (1 << i)) {
			SDA_HIGH();
		} else {
			SDA_LOW();
		}
		cDelay1(); //1us
		SCL_HIGH();
		cDelay1(); //1us
		SCL_LOW();
		cDelay1(); //1us
	}
	SDA_HIGH();
	if (cCheckAck())
		return ERR_NACK;
	return OK;
}


uint8_t cWriteRegAddress(uint16_t addr) {
	cSetSDADirection(SDA_DIR_OUT);
	uint8_t lb = (uint8_t)((addr & 0xff00) >> 8);
	uint8_t hb = (uint8_t)(addr & 0x00ff);

	//write lower byte
	for (int i = 7; i >= 0; i--) {
		if (lb & (1 << i)) {
			SDA_HIGH();
		} else {
			SDA_LOW();
		}
		cDelay1(); //1us
		SCL_HIGH();
		cDelay1(); //1us
		SCL_LOW();
		cDelay1(); //1us
	}
	SDA_HIGH();
	if (cCheckAck())
		return ERR_NACK;

	cSetSDADirection(SDA_DIR_OUT);
	//write higher byte
	for (int i = 7; i >= 0; i--) {
		if (hb & (1 << i)) {
			SDA_HIGH();
		} else {
			SDA_LOW();
		}
		cDelay1(); //1us
		SCL_HIGH();
		cDelay1(); //1us
		SCL_LOW();
		cDelay1(); //1us
	}
	SDA_HIGH();
	if(cCheckAck())
		return ERR_NACK;
	return OK;
}

uint8_t cCheckAck(void) {
	cSetSDADirection(SDA_DIR_IN);
	cDelay1(); //1us
	SCL_HIGH();
	cDelay1(); //1us
	SCL_LOW();
	uint8_t Ack_Status = SDA_READ();
	if (Ack_Status != 0) {
		return 1;
	}
	return 0;
}

void cSetSDADirection(uint8_t in_or_out) {
	if (SDA_DIR_IN == in_or_out) {
		//configure SDA as input pin
		*((uint32_t*)(GPIOA_BASEADDR + GPIO_MODER_OFFSET)) &= ~(0x1U << (2 * SDA_PIN3));	//MODER - input mode
	} else {
		//configure SDa as output pin
		*((uint32_t*)(GPIOA_BASEADDR + GPIO_MODER_OFFSET)) |= (0x1U << (2 * SDA_PIN3));	//MODER - output mode
	}
}

uint8_t cSendData(uint8_t data) {
	cSetSDADirection(SDA_DIR_OUT);
	for (int i = 7; i >= 0; i--) {
		if (data & (1 << i)) {
			SDA_HIGH();
		} else {
			SDA_LOW();
		}
		cDelay1(); //1us
		SCL_HIGH();
		cDelay1(); //1us
		SCL_LOW();
		cDelay1(); //1us
	}
	SDA_HIGH();
	if (cCheckAck())
		return ERR_NACK;
	return OK;
}

uint8_t cReadData(uint8_t *data) {
	uint8_t byte = 0;
	cSetSDADirection(SDA_DIR_IN);
	for (int i = 0; i < 8; i++) {
		//cDelay1(); //1us
		SCL_HIGH();
		cDelay1(); //1us
		byte = (byte << 1) | SDA_READ();
		SCL_LOW();
		cDelay1(); //1us
	}
	cSendAckNack(SDA_SEND_NACK);
	return byte;
}

uint8_t cSendAckNack(uint8_t ack_or_nack) {
	cSetSDADirection(SDA_DIR_OUT);
	if (SDA_SEND_ACK == ack_or_nack)
		SDA_LOW();
	else
		SDA_HIGH();
	cDelay1();
	SCL_HIGH();
	cDelay1();
	SCL_LOW();
	cDelay1();

	return OK;
}
