__asm__(".code16gcc\n");
#include "io.h"

/*
 * c bootloader
 */

//#define POINTER_TEST


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


  Io io;

extern int _start_ctors;
extern int _end_ctors;

extern "C" void WinMain(void)
{

  BOCHS_MB  
  #if 1
  //__asm__ ("mov  %cs, %ax\n");
  //__asm__ ("mov  %ax, %ds\n");
  //__asm__ ("mov  %ax, %ss\n");

#if 0
  int ctor_addr_start = _start_ctors;
  int ctor_addr_end = _end_ctors;
    typedef void (*FuncPtr)();
    FuncPtr fp = (FuncPtr)(ctor_addr_start);
    fp();
#endif

/* Test for GCC > 3.2.0 
 * ref: http://gcc.gnu.org/onlinedocs/gcc-3.4.6/cpp/Common-Predefined-Macros.html
  #if __GNUC__ > 3 || \
  (__GNUC__ == 3 && (__GNUC_MINOR__ > 2 || \
  (__GNUC_MINOR__ == 2 && \
  __GNUC_PATCHLEVEL__ > 0))
*/

  #if __GNUC__ >= 4 && __GNUC_MINOR__ <= 4
  extern void _GLOBAL__I_io();
  _GLOBAL__I_io();
  #else // for 4.7
  extern void _GLOBAL__sub_I_io();
  _GLOBAL__sub_I_io();
  #endif
    #if 0
  for (int i = ctor_addr_start; i < ctor_addr_end ; ++i)
  {
  }

#endif
  //__asm__ ("mov  $0xfff0, %sp\n");
  {
  const char *ver=__VERSION__;
  io.print("hello cpp class\r\n");
  io.print("g++ version: ");
  io.print(ver);
  io.print("\r\n");
  }
  #if 0
  unsigned char *vb = (unsigned char *)0xb8000;
  *vb = 'A';
  *(unsigned char *)0xb8001 = 0xc;
  *(unsigned char *)0xb8002 = 'B';
  *(unsigned char *)0xb8003 = 0x9;
  *(unsigned char *)0xb8004 = '@';
  *(unsigned char *)0xb8005 = 0xc;
  #endif
  //while(1);
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n"); //   回到 DOS

  #endif
}



void *__dso_handle;
extern "C"
{
int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle)
{
  io.print("__cxa_atexit\r\n");
  return 0;
}
void __cxa_finalize(void *d)
{
  io.print("hello dtor\r\n");
}
}
