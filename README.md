# stm32f4-hd44780
A C++ Class for the STM32F4xx series of MCUs to work with LCDs using the Hitachi HD44780 controller, the code has been tested and developed using the STM32F4-Discovery board.

## Usage
### Constructor
The constructor can take a list of 6 (for 4 bit operation) or 10 GPIO pins (for 8 bit operation).
```C++
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
```

### Initialise LCD
By default the init function will set up the display with these parameters:
* Cursor: On
* Cursor Blink: On
* Font: 5x8
* Cursor Mode: Increment
* Shift Display: No
```C++
void initLCD(bool twoLine = true,
                bool showCursor = true,
                bool cursorBlink = true,
                HD44780::Font font = FiveByEight,
                HD44780::CusorMode cursorMode = Increment,
                bool shiftDisplay = false);
```

### Display Control
```C++
void displayControl(bool display, bool cursor, bool blink);
```

### Entry Mode
```C++
void entryMode(HD44780::CusorMode cursorMode, bool shiftDisplay);
```

### Cursor Position
Sets the DDRAM position
```C++
void cursorPosition(uint8_t position);
```

## Sample code
```C++
#define LCD_PORT    GPIOB
#define LCD_PIN_RS  GPIO_Pin_0
#define LCD_PIN_E   GPIO_Pin_1
#define LCD_PIN_DB7 GPIO_Pin_2
#define LCD_PIN_DB6 GPIO_Pin_3
#define LCD_PIN_DB5 GPIO_Pin_4
#define LCD_PIN_DB4 GPIO_Pin_5
#define LCD_PIN_DB3 GPIO_Pin_6
#define LCD_PIN_DB2 GPIO_Pin_7
#define LCD_PIN_DB1 GPIO_Pin_8
#define LCD_PIN_DB0 GPIO_Pin_9

// Set up an 8 bit LCD
HD44780 *hd44780 = new HD44780(LCD_PORT, LCD_PIN_E, LCD_PIN_RS, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7,
                                LCD_PIN_DB0, LCD_PIN_DB1, LCD_PIN_DB2, LCD_PIN_DB3);

// Alternatively use 4 bit
HD44780 *hd44780 = new HD44780(LCD_PORT, LCD_PIN_E, LCD_PIN_RS, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7);

// Initialise the LCD
hd44780->initLCD();

// Turn on the display but turn off the cursor and cursor blink
hd44780->displayControl(true, false, false);

// Clear and home
hd44780->clearLCD();

// Print a string
hd44780->printString("Testing\0");

// Print a character
hd44780->printChar('1');
```

## Prerequisites
* STM32F4xx Standard Peripheral Library (SPL)
