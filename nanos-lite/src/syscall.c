#include "common.h"
#include "syscall.h"

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

void sys_yield(_Context *c) {
  _yield();
  c->GPRx = 0;
}

void sys_exit(_Context *c) {
  _halt(c->GPR2);
}

void sys_write(_Context *c) {
  /*if (c->GPR2 != 1 && c->GPR2 != 2)
    c->GPRx = -1;
  else {
    const char *buf = (char *)c->GPR3;
    // Log("%s\n", buf);
    uintptr_t len = 0;
    while(len < c->GPR4 && buf) {
      _putc(*buf++);
      len++;
    }
    c->GPRx = len;
  }*/
  c->GPRx = fs_write(c->GPR2, (void *)(c->GPR3), c->GPR4);
}

void sys_brk(_Context *c) {
  extern char _end;
  extern char _heap_start;
  printf("%p\n", &_end);
  printf("%p\n", &_heap_start);
  // _write(1, buf, 20);
  _end = c->GPR2;
  printf("%p\n", &_end);
  c->GPRx = 0;
}

void sys_open(_Context *c) {
  c->GPRx = fs_open((char *)(c->GPR2), c->GPR3, c->GPR4);
}

void sys_read(_Context *c) {
  c->GPRx = fs_read(c->GPR2, (void *)(c->GPR3), c->GPR4);
}

void sys_lseek(_Context *c) {
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}

void sys_close(_Context *c) {
  c->GPRx = fs_close(c->GPR2);
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    case SYS_brk:
      sys_brk(c);
      break;
    case SYS_open:
      sys_open(c);
      break;
    case SYS_close:
      sys_close(c);
      break;
    case SYS_lseek:
      sys_lseek(c);
      break;
    case SYS_read:
      sys_read(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
