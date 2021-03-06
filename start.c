#include "io.h"
#include "type.h"
#include "clock.h"
#include "protect.h"
#include "process.h"
#include "asm_syscall.h"
#include "syscall.h"
#include "storage.h"
#include "vfs.h"
#include "romfs.h"
#include "k_string.h"
#include "k_stdlib.h"
#include "k_stdio.h"
#include "endian.h"
#include "keyboard.h"
#include "irq.h"
#include "tty.h"
#include "mm.h"
#include "elf.h"

#define INT_M_PORT 0x20
#define INT_S_PORT 0xa0
#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28
#define INT_M_CTLMASK 0x21
#define INT_S_CTLMASK 0xa1



#define TIMER_MODE 0x43
#define RATE_GENERATOR 0x34 // 00110100
#define TIMER0 0x40
#define TIMER_FREQ 1193182L
#define HZ 100

int get_ticks(void);
//void s32_print(const u8 *s, u8 *vb);


//void __attribute__((aligned(16))) function() { }

//__attribute__((aligned(512))) 
u8 gdt_ptr[6];
Descriptor gdt[GDT_SIZE];

u8 idt_ptr[6];
Gate idt[IDT_SIZE];


IrqHandler irq_table[NR_IRQ];




#if 0
u32 io_load_eflags(void)
{
}

void io_store_eflags(u32 eflg)
{
}

u32 load_cr0(void)
{
}

void store_cr0(u32 cr0)
{
}
#endif

#if 0
void c_test()
{
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);

  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*0));
  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*1));
}
#endif

void init_protected_mode_by_c()
{
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  //int i=65535;
#if 0
{
  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);

  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*2));
  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*3));
}
#endif


  p_asm_memcpy(&gdt, (void*)(*((u32*)(&gdt_ptr[2]))), *((u16*)(&gdt_ptr[0])) + 1);

  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);
  *p_gdt_limit = GDT_SIZE * sizeof(Descriptor) - 1;
  *p_gdt_base = (u32)&gdt;

#if 0
  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*4));

  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*5));
#endif

}

// vector no
#define DIVIDE_NO               0x0
#define DEBUG_NO                0x1
#define NMI_NO                  0x2
#define BREAKPOINT_NO           0x3
#define OVERFLOW_NO             0x4
#define BOUNDS_NO               0x5
#define INVAL_OP_NO             0x6
#define COPROC_NOT_NO           0x7
#define DOUBLE_FAULT_NO         0x8
#define COPROC_SEG_NO           0x9
#define INVAL_TSS_NO            0xA
#define SEG_NOT_NO              0xB
#define STACK_FAULT_NO          0xC
#define PROTECTION_NO           0xD
#define PAGE_FAULT_NO           0xE
#define COPROC_ERR_NO           0x10

int init_idt_by_c()
{
  int init_8259a();

  //s32_print("init_8259a", (u8*)(0xb8000+160*5));
  //init_8259a();

  void init_idt_desc_by_c(u8 vector_no, u8 desc_type, IntHandler handler, u8 privilege);

  u16 *idt_limit = (u16 *)(&idt_ptr[0]);
  u32 *idt_base = (u32 *)(&idt_ptr[2]);
  *idt_limit = IDT_SIZE * sizeof(Gate) - 1;
  *idt_base = (u32)&idt;
//  u8 a=0;

#if 0
  __asm__ volatile ("lidt %0\t\n"
                     :
		     :"m"(idt_ptr)
                   );
#endif
  void divide_error(void);
  void single_step_exception(void);
  void nmi(void);
  void breakpoint_exception(void);
  void overflow(void);
  void bounds_check(void);
  void inval_opcode(void);

  void copr_not_available(void);
  void double_fault(void);
  void copr_seg_overrun(void);
  void inval_tss(void);
  void segment_not_present(void);
  void stack_exception(void);
  void general_protection(void);
  void page_fault(void);
  void copr_error(void);

#if 1
  init_idt_desc_by_c(DIVIDE_NO, DA_386IGate, divide_error, PRIVILEGE_KRNL);
  init_idt_desc_by_c(DEBUG_NO, DA_386IGate, single_step_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(NMI_NO, DA_386IGate, nmi, PRIVILEGE_KRNL);
  init_idt_desc_by_c(BREAKPOINT_NO, DA_386IGate, breakpoint_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(OVERFLOW_NO, DA_386IGate, overflow, PRIVILEGE_KRNL);
  init_idt_desc_by_c(BOUNDS_NO, DA_386IGate, bounds_check, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INVAL_OP_NO, DA_386IGate, inval_opcode, PRIVILEGE_KRNL);

  init_idt_desc_by_c(COPROC_NOT_NO, DA_386IGate, copr_not_available, PRIVILEGE_KRNL);
  init_idt_desc_by_c(DOUBLE_FAULT_NO, DA_386IGate, double_fault, PRIVILEGE_KRNL);
  init_idt_desc_by_c(COPROC_SEG_NO, DA_386IGate, copr_seg_overrun, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INVAL_TSS_NO, DA_386IGate, inval_tss, PRIVILEGE_KRNL);
  init_idt_desc_by_c(SEG_NOT_NO, DA_386IGate, segment_not_present, PRIVILEGE_KRNL);
  init_idt_desc_by_c(STACK_FAULT_NO, DA_386IGate, stack_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(PROTECTION_NO, DA_386IGate, general_protection, PRIVILEGE_KRNL);
  init_idt_desc_by_c(PAGE_FAULT_NO, DA_386IGate, page_fault, PRIVILEGE_KRNL);
  init_idt_desc_by_c(COPROC_ERR_NO, DA_386IGate, copr_error, PRIVILEGE_KRNL);
  #endif

  void hwint00(void);
  void hwint01(void);
  void hwint02(void);
  void hwint03(void);
  void hwint04(void);
  void hwint05(void);
  void hwint06(void);
  void hwint07(void);
  void hwint08(void);
  void hwint09(void);
  void hwint10(void);
  void hwint11(void);
  void hwint12(void);
  void hwint13(void);
  void hwint14(void);
  void hwint15(void);

  init_idt_desc_by_c(INT_VECTOR_IRQ0, DA_386IGate, hwint00, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 1, DA_386IGate, hwint01, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 2, DA_386IGate, hwint02, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 3, DA_386IGate, hwint03, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 4, DA_386IGate, hwint04, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 5, DA_386IGate, hwint05, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 6, DA_386IGate, hwint06, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 7, DA_386IGate, hwint07, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 8, DA_386IGate, hwint08, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 9, DA_386IGate, hwint09, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 10, DA_386IGate, hwint10, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 11, DA_386IGate, hwint11, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 12, DA_386IGate, hwint12, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 13, DA_386IGate, hwint13, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 14, DA_386IGate, hwint14, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 15, DA_386IGate, hwint15, PRIVILEGE_KRNL);

  void sys_call(void);
  init_idt_desc_by_c(INT_VECTOR_SYS_CALL, DA_386IGate, sys_call, PRIVILEGE_USER);
#if 0
  void spurious_handler(void);

  for (int i=0 ; i <= 6; ++i)
    init_idt_desc_by_c(i, DA_386IGate, spurious_handler, PRIVILEGE_KRNL);
#endif

  __asm__ volatile ("lidt idt_ptr");

  return 0;
}

void init_idt_desc_by_c(u8 vector_no, u8 desc_type, IntHandler handler, u8 privilege)
{
  Gate *cur_gate = &idt[vector_no];

  u32 base = (u32)handler;
  cur_gate->offset_low      = base & 0xFFFF;
  cur_gate->selector        = SELECTOR_KERNEL_CS;
  cur_gate->dcount          = 0;
  cur_gate->attr            = desc_type | (privilege << 5);
  cur_gate->offset_high     = (base >> 16) & 0xFFFF;
}

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
  char *err_msg[] = {
                      "#DE: Dvivde Error", 
		      "#DB: RESERVED",
		      "-- : NMI Interrupt", 
		      "#BP: Breakppoint",
		      "#OF: Overflow",
		      "#BR: BOUND Range Exceeded",
		      "#UD: Invalid Opcode (Undefined Opcode)",
		      "#NM: Device Not Available (No Math Coprocessor)",
		      "#DF: Double Fault",
		      "   : Coprocessor Segment Overrun (reserved)",
		      "#TS: Invalid TSS",
		      "#NP: Segment Not Present",
		      "#SS: Stack-Segment Fault",
		      "#GP: General Protection",
		      "#PF: Page Fault",
		      "-- : (Intel reserved. Do not use.)",
		      "#MF: x87 FPU Floating-Point Error (Math Fault)",
		      "#AC: Alignment Check",
		      "#MC: Machine Check",
		      "#XF: SIMD Floating-Point Exceprion"
                    };
  //clear();
  s32_print("exception_handler", (u8*)(0xb8000+160*24));

  u8 str[12]="";
  u8 *str_ptr = str;

  clear_line(0);
  clear_line(1);
  clear_line(2);
  clear_line(3);
  clear_line(23);

  s32_print(err_msg[vec_no], (u8*)(0xb8000+160*23));
  str_ptr = s32_itoa(eflags, str_ptr, 16);
  s32_print("eflags", (u8*)(0xb8000+160*0));
  s32_print(str_ptr, (u8*)(0xb8000+160*1));

  str_ptr = s32_itoa(cs, str_ptr, 16);
  s32_print("cs", (u8*)(0xb8000+160*2));
  s32_print(str_ptr, (u8*)(0xb8000+160*3));
  
  str_ptr = s32_itoa(eip, str_ptr, 16);
  clear_line(4);
  s32_print("eip", (u8*)(0xb8000+160*4));
  clear_line(5);
  s32_print(str_ptr, (u8*)(0xb8000+160*5));

  if (err_code != 0xffffffff)
  {
    str_ptr = s32_itoa(err_code, str_ptr, 16);
    clear_line(6);
    s32_print("err_code", (u8*)(0xb8000+160*6));
    clear_line(7);
    s32_print(str_ptr, (u8*)(0xb8000+160*7));
  }
}


void ptr_test(u8 *ch)
{
  ch[0] = 'X';
  //*ch = 'X';
}

int AA;
char data_str[]="data_string";

#if 0
void startc()
{
  clear();
  u8 *ro_str="ro_string";
  u8 *vb=(u8*)0xb8000;
  int i=65536;
  int c=AA+1;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;

  *vb = 'W';
  #if 0
  while(1);
  #else
  __asm__ ("nop\n\t");
  __asm__ ("nop\n\t");
  #endif
  #if 0
  u8 ch='Q';
  *(vb+15*2) = ch;
  ptr_test(&ch);
  *(vb+17*2) = ch;
  #endif
  //s32_print(ro_str, vb+80*2);
  sp = s32_itoa(i, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160));
  //s32_put_char(sp[0], vb);
  sp = s32_itoa(c, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*2));
  s32_print(data_str, (u8*)(0xb8000+160*3));
  static char *s_str="static_point"; 
  s32_print(s_str, (u8*)(0xb8000+160*4));
  static char s_str_a[]="static_array"; 
  s32_print(s_str_a, (u8*)(0xb8000+160*5));
}
#endif

int init_8259a()
{
  void spurious_irq(int irq);

  // master 8259 icw1
  io_out8(INT_M_PORT, 0x11);

  // slave 8259 icw1
  io_out8(INT_S_PORT, 0x11);

  // master 8259 icw2
  io_out8(INT_M_CTLMASK, INT_VECTOR_IRQ0);

  /* Slave  8259, ICW2. 設置 '從8259' 的中斷入口地址為 0x28 */
  io_out8(INT_S_CTLMASK, INT_VECTOR_IRQ8);

  /* Master 8259, ICW3. IR2 對應 '從8259'. */
  io_out8(INT_M_CTLMASK, 0x4);

  /* Slave  8259, ICW3. 對應 '主8259' 的 IR2. */
  io_out8(INT_S_CTLMASK, 0x2);

  /* Master 8259, ICW4. */
  io_out8(INT_M_CTLMASK, 0x1);

  /* Slave  8259, ICW4. */
  io_out8(INT_S_CTLMASK, 0x1);

  /* Master 8259, OCW1.  */
  io_out8(INT_M_CTLMASK, 0xFF);
  //io_out8(INT_M_CTLMASK, 0xfd); // keyboard irq 1
  //io_out8(INT_M_CTLMASK, 0xfe); // timer irq 0

  /* Slave  8259, OCW1.  */
  io_out8(INT_S_CTLMASK, 0xFF);

  for (int i = 0 ; i < NR_IRQ ; ++i)
  {
    irq_table[i] = spurious_irq;
  }
  return 0;
}



int init_tss(void)
{
  p_asm_memset(&tss, 0, sizeof(tss));
  tss.ss0 = SELECTOR_KERNEL_DS;
  init_descriptor(&gdt[INDEX_TSS], linear2phy(seg2base(SELECTOR_KERNEL_DS), (u32)&tss), sizeof(tss)-1, DA_386TSS);
  tss.iobase = sizeof(tss); // ???

  for (int i = 0 ; i < NR_TASKS + NR_PROCS ; ++i)
  {
    init_descriptor(&gdt[INDEX_LDT_FIRST+i], linear2phy(seg2base(SELECTOR_KERNEL_DS), (u32)proc_table[i].ldt), LDT_SIZE * sizeof(Descriptor) - 1, DA_LDT);
  }

  return 0;  
}

void spurious_irq(int irq)
{
  clear();
  s32_print("spurious_irq", (u8*)(0xb8000+160*6));

  u8 str[12]="";
  u8 *str_ptr = str;
  str_ptr = s32_itoa(irq, str_ptr, 16);
  s32_print(str_ptr, (u8*)(0xb8000+160*7));
}

// when HZ is 100 (10 ms), milli_sec needs more than 10
void milli_delay(int milli_sec)
{
  u32 t = get_ticks();
  u32 t2;

#if 0
  while(((get_ticks() - t ) * 1000 / HZ) < milli_sec);
#else
  //BOCHS_MB
  while(1)
  {
    t2 = get_ticks();
#if 1
// if don't run the line, proc_a will get right behave, but I don't know why
    s32_print_int(t2, (u8*)(0xb8000+160*20), 10);
#endif
    if (((t2 - t) * 1000/HZ ) >= milli_sec)
      break;

  }
#endif
}

void loop_delay(int time)
{
  int i, j, k;
  for (k = 0; k < time; k++) 
  {
    for (i = 0; i < 10; i++) 
    {
      for (j = 0; j < 10000; j++) 
      {
      }
    }
  }
}


// 8254
int init_timer(void)
{
  io_out8(TIMER_MODE, RATE_GENERATOR);
  io_out8(TIMER0, (u8)(TIMER_FREQ/HZ) );
  io_out8(TIMER0, (u8)(TIMER_FREQ/HZ) >> 8 );
  return 0;
}

//#define TEST_ALLOC_MEM
//#define TEST_ROMFS
void kernel_main(void)
{
#ifdef TEST_ALLOC_MEM
  char *mem_p[64];
  for (int i=0 ; i < 64 ; ++i)
  {
    mem_p[i] = (u8*)alloc_mem();
  }
  int con=1;
  for (int i=0 ; i < 64 ; ++i)
  {
    free_mem(mem_p[i]);
  }
#endif
#ifdef TEST_ROMFS
  //INode *inode = fs_type[ROMFS]->namei(fs_type[ROMFS], "echo.bin"); // get super block infomation
  INode *inode = fs_type[ROMFS]->namei(fs_type[ROMFS], "echo"); // get super block infomation

  //s32_printf("inode->dsize: %d\r\n", inode->dsize);

  int addr = fs_type[ROMFS]->get_daddr(inode);
  //u32 romfs_get_daddr(INode *inode);
  //int addr = romfs_get_daddr(inode);


  //u8 buf[512];
  //u8 *buf = (u8*)0x1000;
  u8 *buf = (u8*)alloc_mem();

  //fs_type[ROMFS]->device->dout(fs_type[ROMFS]->device, buf, fs_type[ROMFS]->get_daddr(inode), inode->dsize);
  fs_type[ROMFS]->device->dout(fs_type[ROMFS]->device, buf, addr, inode->dsize);

  Elf32Ehdr *elf_header = (Elf32Ehdr*)buf;
  Elf32Phdr *elf_pheader = (Elf32Phdr*)((u8 *)buf + elf_header->e_phoff);
  u32 entry = elf_header->e_entry;

  for (int i=0 ; i < elf_header->e_phnum; ++i)
  {
    if (CHECK_PT_TYPE_LOAD(elf_pheader))
    {
      p_asm_memcpy((u8*)elf_pheader->p_vaddr, buf+(elf_pheader->p_offset), elf_pheader->p_filesz);
    }
    ++elf_pheader;
  }

  //p_dump_u8(buf, inode->dsize);
  exec((u8*)entry);
  free_mem(buf);
  while(1);
#endif

  void setup_paging(void);
  //setup_paging();
 
#if 1
  put_irq_handler(CLOCK_IRQ, clock_handler);
  enable_irq(CLOCK_IRQ);
#endif
  ready_process = proc_table;



  init_proc();
  set_cursor(0);
  //set_video_start_addr(80);

  //BOCHS_MB
  void restart(void);
  restart(); // need run restart(), because restart() iret will enable interrupt
}

typedef int (*InitFunc)(void);



static InitFunc init[]={
                         init_8259a,
			 init_idt_by_c,
			 init_tss,
			 init_timer,
                         ramdisk_driver_init,
                         init_keyboard,
                         romfs_init,
                         mm_init,
                         0
                       };


void load_init_boot(InitFunc *init_func)
{
  for (int i = 0 ; init_func[i] ; ++i)
  {
    init_func[i]();
  }
  u8 buf[512];
  storage[RAMDISK]->dout(storage[RAMDISK], buf, 0, sizeof(buf));
  p_dump_u8(buf, 32);

  //romfs_init();
  //test_romfs();
  //test_vga();

}

void plat_boot(void)
{
  load_init_boot(init);
}
