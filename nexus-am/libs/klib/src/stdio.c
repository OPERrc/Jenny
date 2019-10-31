#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char *itoa(int value, char *str, int radix) {
  assert(radix > 16);
  char index[] = "0123456789abcdef";
  int len = 0;
  while (value > 0) {
    str[len] = index[value % radix];
    value /= radix;
    len++;
  }
  str[len] = '\0';
  for (int i = 0; i <= len / 2; i++){
    char tmp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = tmp;
  }
  return str;
}

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  int d;
  char *str = NULL;

  va_start(ap, fmt);
  int flag = 0;
  while (*fmt) {
    switch (*fmt++) {
      case '%': 
        flag = 1;
        *out++ = *fmt;
        break;
      case 's':
        if (flag) {
          str = va_arg(ap, char *);
          out = strcat(out, str);
          flag = 0;
          break;
        }
        else break;
      case 'd':
        if (flag) {
          d = va_arg(ap, int);
          out = strcat(out, itoa(d, str, 10));
          flag = 0;
          break;
        }
        else break;
      default: *out++ = *fmt;
    }
  }
  va_end(ap);

  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
