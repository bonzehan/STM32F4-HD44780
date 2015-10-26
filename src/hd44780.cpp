#include "hd44780.h"

// Set up the LCD in 4 bit Mode
HD44780::HD44780(GPIO_TypeDef* GPIOx, uint16_t pin_e, uint16_t pin_rs,
          uint16_t pin_db4, uint16_t pin_db5, uint16_t pin_db6, uint16_t pin_db7)
{

    // Set up the pin mapping
    this->lcd_gpio      = GPIOx;
    this->lcd_pin_e     = pin_e;
    this->lcd_pin_rs    = pin_rs;
    this->lcd_pin_db4   = pin_db4;
    this->lcd_pin_db5   = pin_db5;
    this->lcd_pin_db6   = pin_db6;
    this->lcd_pin_db7   = pin_db7;

    // Utility
    this->lcd_pins_all = pin_e | pin_rs | pin_db4 | pin_db5 | pin_db6 | pin_db7;

    this->eight_bit_mode = false;

    initPins();
}

// Set up the LCD in 8 Bit Mode
HD44780::HD44780(GPIO_TypeDef* GPIOx, uint16_t pin_e, uint16_t pin_rs,
          uint16_t pin_db4, uint16_t pin_db5, uint16_t pin_db6, uint16_t pin_db7,
          uint16_t pin_db0, uint16_t pin_db1, uint16_t pin_db2, uint16_t pin_db3)
{

    // Set up the pin mapping
    this->lcd_gpio      = GPIOx;
    this->lcd_pin_e     = pin_e;
    this->lcd_pin_rs    = pin_rs;
    this->lcd_pin_db0   = pin_db0;
    this->lcd_pin_db1   = pin_db1;
    this->lcd_pin_db2   = pin_db2;
    this->lcd_pin_db3   = pin_db3;
    this->lcd_pin_db4   = pin_db4;
    this->lcd_pin_db5   = pin_db5;
    this->lcd_pin_db6   = pin_db6;
    this->lcd_pin_db7   = pin_db7;

    // Utility
    this->lcd_pins_all = pin_e | pin_rs | pin_db4 | pin_db5 | pin_db6 | pin_db7 | pin_db0 | pin_db1 | pin_db2 | pin_db3;

    this->eight_bit_mode = true;

    initPins();
}

void HD44780::initPins()
{
    // Set up the physical hardware pins
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = this->lcd_pins_all;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(this->lcd_gpio, &GPIO_InitStructure);
}


void HD44780::initLCD(bool twoLine, bool showCursor, bool cursorBlink, HD44780::Font font, HD44780::CusorMode cursorMode, bool shiftDisplay)
{
    reset();

    if(this->eight_bit_mode)
    {
        functionSet(EightBit, twoLine, font);
        entryMode(cursorMode, shiftDisplay);
        displayControl(true, showCursor, cursorBlink);
        clearLCD();
    }else{
        // Initial 4 Bit FS
        // This first command is handled as 8 bit
        // the second argument here ensures the Enable pin only gets
        // toggled once for this command despite being in 4 bit mode
        sendCmd(0b00000010, true);

        functionSet(FourBit, twoLine, font);
        entryMode(cursorMode, shiftDisplay);
        displayControl(true, showCursor, cursorBlink);
        clearLCD();
    }
}

void HD44780::reset()
{
    // This resets the display controller to a known state
    // Reference:
    //     http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
    sendCmd(0b00000011, true);
    wait(5);
    sendCmd(0b00000011, true);
    wait(1);
    sendCmd(0b00000011, true);
    wait(1);

}

void HD44780::displayControl(bool display, bool cursor, bool blink)
{
    // 0    0   0   0   1   D(1=Disp. On)   C(1=Cursor On)  B(1=Blink)
    uint8_t cmd =   0b00001000;

    if(display)
        cmd = cmd | 0b00000100;

    if(cursor)
        cmd = cmd | 0b00000010;

    if(blink)
        cmd = cmd | 0b00000001;

    sendCmd(cmd);
}

void HD44780::functionSet(HD44780::DataLength dataLength, bool twoLine, HD44780::Font font)
{
    // 0    0   1   DL(0=4bit) NL(1=2Line)  F(0=5x8 Font)   *   *
    uint8_t cmd =   0b00100000;

    if(dataLength == EightBit)
        cmd = cmd | 0b00010000;

    if(twoLine)
        cmd = cmd | 0b00001000;

    if(font == FiveByTen)
        cmd = cmd | 0b00000100;

    sendCmd(cmd);
}

void HD44780::entryMode(HD44780::CusorMode cursorMode, bool shiftDisplay)
{
    // 0    0   0   0   0   1   I/D(1=Inc cursor)   S(0=no Shift)
    uint8_t cmd =   0b00000100;

    if(cursorMode == Increment)
        cmd = cmd | 0b00000010;

    if(shiftDisplay)
        cmd = cmd | 0b00000001;

    sendCmd(cmd);
}

void HD44780::upperNibble(uint8_t cmd)
{

        if(cmd & 0x80)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db7);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db7);
        if(cmd & 0x40)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db6);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db6);
        if(cmd & 0x20)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db5);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db5);
        if(cmd & 0x10)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db4);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db4);
}

void HD44780::lowerNibble(uint8_t cmd)
{
    // If we are in eight bit mode then just map the lower nibble to the
    // lower data pins
    if(this->eight_bit_mode)
    {
        if(cmd & 0x8)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db3);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db3);
        if(cmd & 0x4)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db2);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db2);
        if(cmd & 0x2)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db1);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db1);
        if(cmd & 0x1)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db0);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db0);
    }else{
        if(cmd & 0x8)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db7);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db7);
        if(cmd & 0x4)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db6);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db6);
        if(cmd & 0x2)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db5);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db5);
        if(cmd & 0x1)
            GPIO_SetBits(this->lcd_gpio, this->lcd_pin_db4);
        else
            GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_db4);
    }
}

void HD44780::pulseEnable()
{
    wait(1);
    GPIO_SetBits(this->lcd_gpio, this->lcd_pin_e);
    wait(1);
    GPIO_ResetBits(this->lcd_gpio, this->lcd_pin_e);
    wait(1);
}

void HD44780::sendCmd(uint8_t cmd, bool force8bit)
{
    GPIO_ResetBits(this->lcd_gpio, this->lcd_pins_all);

    upperNibble(cmd);

    // In 4 bit mode we need to send the data in two passes
    if(!this->eight_bit_mode && !force8bit)
    {
        pulseEnable();
    }

    lowerNibble(cmd);

    pulseEnable();
}

void HD44780::printChar(uint8_t c)
{
    GPIO_ResetBits(this->lcd_gpio, this->lcd_pins_all);
    GPIO_SetBits(this->lcd_gpio, this->lcd_pin_rs);
    upperNibble(c);

    // In 4 bit mode we need to send the data in two passes
    if(!this->eight_bit_mode)
        pulseEnable();

    lowerNibble(c);
    pulseEnable();
}

void HD44780::printString(const char *str) {
	uint8_t i=0;
	while(str[i] != '\0') {
		printChar(str[i]);
		i++;
	}
}

void HD44780::clearLCD()
{
    // 0    0   0   0   0   0   0   1
    sendCmd(0b00000001);
    wait(3);
}

void HD44780::cursorPosition(uint8_t pos)
{
    // 1    A   A   A   A   A   A   A
    uint8_t cmd =   0b10000000;
    sendCmd(pos | cmd);
}

void HD44780::wait(uint32_t tCount)
{
    uint32_t totalDelay = tCount * 2516;
	for(; totalDelay != 0; totalDelay--);
}
