#ifndef HD44780_H_INCLUDED
#define HD44780_H_INCLUDED

#include "stm32f4xx_conf.h"

class HD44780
{
public:
    HD44780(GPIO_TypeDef* GPIOx,
          uint16_t pin_e,
          uint16_t pin_rs,
          uint16_t pin_db4,
          uint16_t pin_db5,
          uint16_t pin_db6,
          uint16_t pin_db7);
    HD44780(GPIO_TypeDef* GPIOx,
          uint16_t pin_e,
          uint16_t pin_rs,
          uint16_t pin_db4,
          uint16_t pin_db5,
          uint16_t pin_db6,
          uint16_t pin_db7,
          uint16_t pin_db0,
          uint16_t pin_db1,
          uint16_t pin_db2,
          uint16_t pin_db3);
    ~HD44780();

    enum DataLength{
        EightBit,
        FourBit
    };

    enum Font{
        FiveByEight,
        FiveByTen
    };

    enum CusorMode{
        Increment,
        Decrement
    };


    void initLCD(bool twoLine = true,
                 bool showCursor = true,
                 bool cursorBlink = true,
                 HD44780::Font font = FiveByEight,
                 HD44780::CusorMode cursorMode = Increment,
                 bool shiftDisplay = false);

    void printChar(uint8_t c);
    void printString(const char *str);
    void clearLCD();
    void displayControl(bool display, bool cursor, bool blink);
    void entryMode(HD44780::CusorMode cursorMode, bool shiftDisplay);
    void cursorPosition(uint8_t position);

private:
    uint16_t lcd_pin_e;
    uint16_t lcd_pin_rs;
    uint16_t lcd_pin_db0;
    uint16_t lcd_pin_db1;
    uint16_t lcd_pin_db2;
    uint16_t lcd_pin_db3;
    uint16_t lcd_pin_db4;
    uint16_t lcd_pin_db5;
    uint16_t lcd_pin_db6;
    uint16_t lcd_pin_db7;
    uint16_t lcd_pins_all;
    GPIO_TypeDef* lcd_gpio;

    bool eight_bit_mode;

    void sendCmd(uint8_t cmd, bool force8bit = false);

    void wait(uint32_t tCount);
    void upperNibble(uint8_t cmd);
    void lowerNibble(uint8_t cmd);
    void pulseEnable();

    void initPins();
    void reset();
    void functionSet(HD44780::DataLength dataLength, bool twoLine, HD44780::Font font);


};


#endif /* HD44780_H_INCLUDED */
