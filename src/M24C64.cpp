/*
  Title : M24C64 I2C command library
  Copyright : Copyright (c) 2021 Shingo FUKUYA
  URL : https://github.com/volty98/
  License : Released under the MIT license
*/
#include "M24C64.h"
#include <arduino.h>
#include <Wire.h>

M24C64::M24C64()
{
    return;
}
M24C64::~M24C64()
{
    return;
}
void M24C64::init(uint8_t adr)
{
	this->base_addr = adr;
	Wire.begin();
	return;
}
char M24C64::read(uint16_t addr)
{
	char rd;
	
	//set address
	Wire.beginTransmission(this->base_addr);
	Wire.write((addr >> 8) & 0xFF);
	Wire.write(addr & 0xFF);
	Wire.endTransmission(false);
	//read 1 byte
	Wire.requestFrom(this->base_addr, 1, false);
	while(Wire.available())
	{
		rd = Wire.read();
	}
	Wire.endTransmission();
	return rd;
}
void M24C64::write(uint16_t addr, uint8_t value)
{
	//set address
	Wire.beginTransmission(this->base_addr);
	Wire.write(addr >> 8 & 0xFF);
	Wire.write(addr & 0xFF);
	//write 1 byte
	Wire.write(value);
	Wire.endTransmission();
	return;
}
