#ifndef _esp8266_pdc8544_h_
#define _esp8266_pdc8544_h

#include <Arduino.h>

/* lcd parameters */
#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_BUFFER_SIZE LCD_WIDTH * LCD_HEIGHT / 8

/* pcd8544 commands and symbols */
#define PCD8544_COMMAND 0x00
#define PCD8544_DATA 0x01
#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01
#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

/* mapping macros: map x, y screen position to i, j, k bitwise buffer position (kth bit on i+width*j byte)*/
#define MAP_i(x, y) (x)
#define MAP_j(x, y) (y/8)
#define MAP_k(x, y) (y%8)
#define MAP(i, j) (i + LCD_WIDTH*j)

/* * read/set/clear bit k in buffer position i j * */
#define BIT_READ(buffer, x, y) bitRead(buffer[MAP(MAP_i(x, y), MAP_j(x,y))], MAP_k(x,y))
#define BIT_SET(buffer, x, y) bitSet(buffer[MAP(MAP_i(x, y), MAP_j(x,y))], MAP_k(x,y))
#define BIT_CLEAR(buffer, x, y) bitClear(buffer[MAP(MAP_i(x, y), MAP_j(x,y))], MAP_k(x,y))

#define XOR_SWAP(a, b) ((&(a) == &(b)) ? (a) : ((a)^=(b),(b)^=(a),(a)^=(b)))

#define WHITE 0
#define BLACK 1

class ESP8266_PDC8544 {

  public:
  ESP8266_PDC8544(uint8_t pin_reset, uint8_t pin_sce, uint8_t pin_dc, uint8_t pin_sdin, uint8_t pin_sclk);
  void initialize(uint8_t contrast = 50, uint8_t bias = 0x04);
  void clear();
  void bitmap(uint8_t bitmap[]);
  uint8_t get_pixel(uint8_t x, uint8_t y);
  void set_pixel(uint8_t x, uint8_t y, uint8_t color = BLACK);
  void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color = BLACK);
  void circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color = BLACK);
  void update();
  void set_text_cursor(uint8_t x, uint8_t y);
  void wrap_text_cursor();
  void text(const char * text);
  void text(uint8_t x, uint8_t y, const char * text);
  void text(String * text);
  void text(uint8_t x, uint8_t y, String * text);

  private:
  uint8_t pin_reset;
  uint8_t pin_sce;
  uint8_t pin_dc;
  uint8_t pin_sdin;
  uint8_t pin_sclk;
  uint8_t display_buffer[LCD_WIDTH * LCD_HEIGHT / 8];
  uint8_t text_cursor[2];

  void write(uint8_t dc, uint8_t data);
  void command(uint8_t command);
  void data(uint8_t data);
  void cursor(uint8_t x, uint8_t y);
  void character(uint8_t x, uint8_t y, char character);

};

#endif
