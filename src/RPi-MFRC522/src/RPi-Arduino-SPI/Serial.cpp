/*
 From different classes in https://github.com/arduino/ArduinoCore-avr/tree/master/cores/arduino
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Serial.h"

// Public Methods //////////////////////////////////////////////////////////////

size_t SerialClass::write(const uint8_t *buffer, size_t size)
{
  return fwrite(buffer, size, 1, stdout);
}

size_t SerialClass::write(uint8_t c)
{
  return write(&c, 1);
}

size_t SerialClass::print(const std::string &s)
{
  return write(s.c_str(), s.length());
}

size_t SerialClass::print(const char str[])
{
  return write(str);
}

size_t SerialClass::print(char c)
{
  return write(c);
}

size_t SerialClass::print(unsigned char b, int base)
{
  return print((unsigned long)b, base);
}

size_t SerialClass::print(int n, int base)
{
  return print((long)n, base);
}

size_t SerialClass::print(unsigned int n, int base)
{
  return print((unsigned long)n, base);
}

size_t SerialClass::print(long n, int base)
{
  if (base == 0)
  {
    return write(n);
  }
  else if (base == 10)
  {
    if (n < 0)
    {
      int t = print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  }
  else
  {
    return printNumber(n, base);
  }
}

size_t SerialClass::print(unsigned long n, int base)
{
  if (base == 0)
    return write(n);
  else
    return printNumber(n, base);
}

size_t SerialClass::print(double n, int digits)
{
  return printFloat(n, digits);
}

size_t SerialClass::println(void)
{
  return write("\r\n");
}

size_t SerialClass::println(const std::string &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t SerialClass::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t SerialClass::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t SerialClass::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t SerialClass::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t SerialClass::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t SerialClass::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t SerialClass::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t SerialClass::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t SerialClass::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2)
    base = 10;

  do
  {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return write(str);
}

size_t SerialClass::printFloat(double number, uint8_t digits)
{
  size_t n = 0;

  if (isnan(number))
    return print("nan");
  if (isinf(number))
    return print("inf");
  if (number > 4294967040.0)
    return print("ovf"); // constant determined empirically
  if (number < -4294967040.0)
    return print("ovf"); // constant determined empirically

  // Handle negative numbers
  if (number < 0.0)
  {
    n += print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
  {
    n += print('.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}

SerialClass Serial;