#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define make_my_itoa(radix) \
  flag = 0; \
  d = va_arg(ap, int); \
  my_itoa(d, tmp, radix); \
  count += strlen(tmp); \
  tmp += strlen(tmp);

int my_itoa(int value, char *out, int radix) {
  static char index[] = "0123456789abcdef";
  int len = 0;
  char numbuf[32];
  if (value == 0) {
    numbuf[len] = '0';
    len++;
  }
  else
  while (value > 0) {
    numbuf[len] = index[value % radix];
    value /= radix;
    len++;
  }
  // numbuf[len] = '\0';
  for (int i = len - 1; i >= 0; i--) {
    *out++ = numbuf[i];
  }
  *out++ = '\0';
  return len;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[512];
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
  *out = '\0';
  char a[512];
  char *str = a;
  char *tmp = out;
  int d, count = 0;
  char ch;
  //char align = ' ';

  int flag = 0;
  while (*fmt) {
    //_putc(*fmt);
    if (*fmt != '%' && !flag) {
      *tmp++ = *fmt;
      fmt++;
      count++;
      continue;
    }

    flag = 1;
    switch (*fmt) {
      case 'c':
        ch = (unsigned char)va_arg(ap, int);
          *tmp++ = ch;
        flag = 0;
        count++;
        break;

      case 's':
        str = va_arg(ap, char *);
        while (*str) {
          *tmp++ = *str;
          str++;
          count++;
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

      case 'p':
        //*tmp++ = '0';
        //*tmp++ = 'x'; 
        //count += 2;
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
        printf("%c unimplemented!\n", *fmt);
        //my_TODO();
        break;
    }
    fmt++;
  }
  *tmp = '\0';
  //count++;

  return tmp - out;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int count = vsprintf(out, fmt, ap);
  va_end(ap);

  return count;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  my_TODO();
  return 0;
}

#endif
