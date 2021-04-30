#ifndef M24C64_H
#define M24C64_H
#include <arduino.h>
#include <Wire.h>

//M24C64 eeprom address size is 4bits.(--XX XXXX XXXX XXXX)
//
//M24C64 I2C address is 1010 [E2] [E1] [E0]
//1010000 : 0x50

#define ADDR_DEFAULT 0x50

class M24C64
{
public:
    M24C64();
    ~M24C64();

    /*
    * \brief initialize command. must call this function before other function.
    * \param adr I2C device address (default:0x50)
    */
    void init(uint8_t adr = ADDR_DEFAULT);

    /*
    * \brief 1 byte read from EEPROM.
    * \param addr EEPROM address.(0x0000～0x3FFF)
    */
	char read(uint16_t addr);

    /*
    * \brief 1 byte write to EEPROM.
    * \param addr EEPROM address.(0x0000～0x3FFF)
    * \param value write value.
    */
	void write(uint16_t addr, uint8_t value);
private:
    uint8_t base_addr;// I2C device address
};
#endif
