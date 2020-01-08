/*
 From different classes in https://github.com/arduino/ArduinoCore-avr/tree/master/cores/arduino
 */

#ifndef Serial_h
#define Serial_h

#include <string>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class SerialClass
{
private:
  size_t printNumber(unsigned long, uint8_t);
  size_t printFloat(double, uint8_t);

public:
  SerialClass() {}

  size_t write(uint8_t);
  size_t write(const char *str)
  {
    if (str == NULL)
      return 0;
    return write((const uint8_t *)str, strlen(str));
  }
  size_t write(const uint8_t *buffer, size_t size);
  size_t write(const char *buffer, size_t size)
  {
    return write((const uint8_t *)buffer, size);
  }

  size_t print(const std::string &);
  size_t print(const char[]);
  size_t print(char);
  size_t print(unsigned char, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(double, int = 2);

  size_t println(const std::string &s);
  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(double, int = 2);
  size_t println(void);
};
extern SerialClass Serial;

#endif