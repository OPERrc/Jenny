#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define make_my_itoa(radix) \
  flag = 0; \
  d = va_arg(ap, int); \
  str = my_itoa(d, str, radix); \
  while (*str) { \
    *tmp++ = *str; \
    str++; \
  }

char *my_itoa(int value, char *str, int radix) {
  char index[] = "0123456789abcdef";
  int len = 0;
  if (value == 0) {
    str[len] = '0';
    len++;
  }
  else
  while (value > 0) {
    str[len] = index[value % radix];
    value /= radix;
    len++;
  }
  str[len] = '\0';
  for (int i = 0; i < len / 2; i++) {
    char tmp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = tmp;
  }
  return str;
}

int printf(const char *fmt, ...) {
  va_list(ap);
  va_start(ap, fmt);
  char buf[1000];
  char *str = buf;
  vsprintf(str, fmt, ap);
  va_end(ap);

  int len = 0;
  while (*str) {
    _putc(*str);
    str++;
    len++;
  }
  return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char a[1000];
  char *str = a;
  char *tmp = out;
  int d;
  char ch;
  //char align = ' ';

  int flag = 0;
  while (*fmt) {
    //_putc(*fmt);
    if (*fmt != '%' && !flag) {
      *tmp++ = *fmt;
      fmt++;
      continue;
    }

    flag = 1;
    switch (*fmt) {
      case 'c':
        ch = (unsigned char)va_arg(ap, int);
          *tmp++ = ch;
        flag = 0;
        break;

      case 's':
        str = va_arg(ap, char *);
        while (*str) {
          *tmp++ = *str;
          str++;
        }
        flag = 0;
        break;
      
      case 'o':
        make_my_itoa(8);
        break;

      case 'd':
        make_my_itoa(10);
        break;

      case 'x':
        make_my_itoa(16);
        break;

      /*
      case '0': *tmp++ = *str;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':*/

      case '%': break;

      default:
        printf("%c uninplemented!\n", *fmt);
        //TODO();
        break;
    }
    fmt++;
  }
  *tmp = '\0';

  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list(ap);
  va_start(ap, fmt);
  vsprintf(out, fmt, ap);
  va_end(ap);

  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  my_TODO();
  return 0;
}

#endif
