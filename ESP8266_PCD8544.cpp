#include "ESP8266_PCD8544.h"
#include "nanofont.h"

ESP8266_PDC8544::ESP8266_PDC8544(uint8_t pin_reset, uint8_t pin_sce, uint8_t pin_dc, uint8_t pin_sdin, uint8_t pin_sclk) {

    this->pin_reset = pin_reset;
    this->pin_sce = pin_sce;
    this->pin_dc = pin_dc;
    this->pin_sdin = pin_sdin;
    this->pin_sclk = pin_sclk;

    memset(this->display_buffer, 0x00, LCD_WIDTH * LCD_HEIGHT / 8);

    this->text_cursor[0] = 0;
    this->text_cursor[1] = 0;

}

void ESP8266_PDC8544::initialize(uint8_t contrast, uint8_t bias) {

    pinMode(this->pin_reset, OUTPUT);
    pinMode(this->pin_sce, OUTPUT);
    pinMode(this->pin_dc, OUTPUT);
    pinMode(this->pin_sdin, OUTPUT);
    pinMode(this->pin_sclk, OUTPUT);

    //Reset the LCD to a known state
    digitalWrite(this->pin_reset, LOW);
    digitalWrite(this->pin_reset, HIGH);

    //Tell LCD that extended commands follow
    this->command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);

    // LCD bias mode
    this->command(PCD8544_SETBIAS | bias);

    //Set LCD Vop (Contrast): Tddry 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    this->command(PCD8544_SETVOP | (contrast > 0x7f ? 0x7f : contrast));

    //Set Temp coefficent
    //this->command(PCD8544_SETTEMP);

    // We must send 0x20 before modifying the display control mode
    this->command(PCD8544_FUNCTIONSET);

    // Set display control, normal mode. 0x0D for inverse
    this->command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

}

void ESP8266_PDC8544::bitmap(uint8_t bitmap[]) {

    memcpy(display_buffer, bitmap, LCD_BUFFER_SIZE);

}

void ESP8266_PDC8544::clear() {

    memset(display_buffer, 0, LCD_BUFFER_SIZE);

    this->cursor(0, 0);

    for (uint16_t i = 0; i < LCD_BUFFER_SIZE; i++) {
        this->data(0x00);
    }

}

void ESP8266_PDC8544::set_pixel(uint8_t x, uint8_t y, uint8_t color) {

    if ((x < 0) || (x >= LCD_WIDTH) || (y < 0) || (y >= LCD_HEIGHT)) {
      return;
    }

    if (color) {
        BIT_SET(display_buffer, x, y);
    } else {
        BIT_CLEAR(display_buffer, x, y);
    }

}

uint8_t ESP8266_PDC8544::get_pixel(uint8_t x, uint8_t y) {
    return BIT_READ(display_buffer, x, y);
}

/* algorithm taken from adafruit gfx library */
void ESP8266_PDC8544::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {

    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        XOR_SWAP(x0, y0);
        XOR_SWAP(x1, y1);
    }

    if (x0 > x1) {
        XOR_SWAP(x0, x1);
        XOR_SWAP(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            this->set_pixel(y0, x0, color);
        } else {
            this->set_pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }

}

/* algorithm taken from adafruit gfx library */
void ESP8266_PDC8544::circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  this->set_pixel(x0, y0+r, color);
  this->set_pixel(x0, y0-r, color);
  this->set_pixel(x0+r, y0, color);
  this->set_pixel(x0-r, y0, color);

  while (x < y) {

      if (f >= 0) {
          y--;
          ddF_y += 2;
          f += ddF_y;
      }

      x++;
      ddF_x += 2;
      f += ddF_x;

      this->set_pixel(x0 + x, y0 + y, color);
      this->set_pixel(x0 - x, y0 + y, color);
      this->set_pixel(x0 + x, y0 - y, color);
      this->set_pixel(x0 - x, y0 - y, color);
      this->set_pixel(x0 + y, y0 + x, color);
      this->set_pixel(x0 - y, y0 + x, color);
      this->set_pixel(x0 + y, y0 - x, color);
      this->set_pixel(x0 - y, y0 - x, color);

  }

}

void ESP8266_PDC8544::character(uint8_t x, uint8_t y, char character) {

    if (character < 0x20 || character > 0x7F) {
        character = 0x20;
    }

    uint16_t bitmap = aGlyphs3x4[character - 0x20];
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            this->set_pixel(x+(3-j), y+(3-i), bitRead(bitmap, j + 4*i));
        }
    }

};

void ESP8266_PDC8544::set_text_cursor(uint8_t x, uint8_t y) {
  this->text_cursor[0] = x;
  this->text_cursor[1] = y;
}

void ESP8266_PDC8544::wrap_text_cursor() {

  this->text_cursor[0] = 0;
  this->text_cursor[1] += 4;

  // wrap screen
  if (this->text_cursor[1] > LCD_HEIGHT -1) {
      this->text_cursor[1] = 0;
  }

}


void ESP8266_PDC8544::text(uint8_t x, uint8_t y, const char * text) {
  this->set_text_cursor(x, y);
  this->text(text);
}

void ESP8266_PDC8544::text(const char * text) {

  for (uint16_t i = 0; text[i] != 0; i++) {

    this->character(this->text_cursor[0], this->text_cursor[1], text[i]);
    this->text_cursor[0] += 4;

    // wrap lines
    if (this->text_cursor[0]> LCD_WIDTH-1) {
      this->wrap_text_cursor();
    }

  }

}

void ESP8266_PDC8544::text(String * text) {
  this->text(text->c_str());
}

void ESP8266_PDC8544::text(uint8_t x, uint8_t y, String * text) {
  this->text(x, y, text->c_str());
}

void ESP8266_PDC8544::update() {

    this->cursor(0, 0);

    for (uint16_t i = 0; i < LCD_BUFFER_SIZE; i++) {
        this->data(display_buffer[i]);
    }

}

void ESP8266_PDC8544::write(uint8_t dc, uint8_t data) {

    digitalWrite(this->pin_dc, dc);

    digitalWrite(this->pin_sce, LOW);
    shiftOut(this->pin_sdin, this->pin_sclk, MSBFIRST, data);
    digitalWrite(this->pin_sce, HIGH);

}

void ESP8266_PDC8544::command(uint8_t command) {
    this->write(PCD8544_COMMAND, command);
}

void ESP8266_PDC8544::data(uint8_t data) {
    this->write(PCD8544_DATA, data);
}

void ESP8266_PDC8544::cursor(uint8_t x, uint8_t y) {

    this->command(PCD8544_SETXADDR | x);
    this->command(PCD8544_SETYADDR | y);

};
