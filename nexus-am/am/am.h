/*
 * The Nexus Abstract Machine Architecture (AM)
 * A portable abstraction of a bare-metal computer
 */

#ifndef __AM_H__
#define __AM_H__

#include <stdint.h>
#include <stddef.h>
#include ARCH_H // "arch/x86-qemu.h", "arch/native.h", ...

#ifdef __cplusplus
extern "C" {
#endif

// ===================== Constants and Structs =======================
/* 统一的事件编号 */
enum {
  _EVENT_NULL = 0,  // 0
  _EVENT_ERROR,     // 1  
  _EVENT_IRQ_TIMER, // 2
  _EVENT_IRQ_IODEV, // 3
  _EVENT_PAGEFAULT, // 4
  _EVENT_YIELD,     // 5
  _EVENT_SYSCALL,   // 6
};

enum {
  _PROT_NONE  = 1, // no access
  _PROT_READ  = 2, // can read
  _PROT_WRITE = 4, // can write
  _PROT_EXEC  = 8, // can execute
};

// Memory area for [@start, @end)
typedef struct _Area {
  void *start, *end;
} _Area; 

// An event of type @event, caused by @cause of pointer @ref
typedef struct _Event {
  int event;            /* 事件编号 */
  uintptr_t cause, ref; /* 描述事件的补充信息 */
  const char *msg;      /* 事件信息字符串 */
} _Event;

// Arch-dependent processor context
typedef struct _Context _Context; /* 不同架构上下文信息不同 */

// A protected address space with user memory @area
// and arch-dependent @ptr
typedef struct _AddressSpace {
  size_t pgsize;
  _Area area;
  void *ptr;
} _AddressSpace;

// ====================== Turing Machine (TRM) =======================

extern _Area _heap;
void _putc(char ch);
void _halt(int code) __attribute__((__noreturn__));

// ======================= I/O Extension (IOE) =======================

int _ioe_init();
size_t _io_read(uint32_t dev, uintptr_t reg, void *buf, size_t size);
size_t _io_write(uint32_t dev, uintptr_t reg, void *buf, size_t size);

// ====================== Context Extension (CTE) ====================

/* _cte_init函数除了对CTE进行初始化，还接收一个来自操作系统的时间处理回调函数指针。
 * 当发生事件时，CTE会把时间和相关的上下文作为参数，来调用这个回调参数,交给操作系统
 * 进行后续处理。
 */
int _cte_init(_Context *(*handler)(_Event ev, _Context *ctx));
void _yield();    /* 进行自陷操作，会触发编号为_EVENT_YIELD事件 */
int _intr_read();
void _intr_write(int enable);
_Context *_kcontext(_Area kstack, void (*entry)(void *), void *arg);

// ================= Virtual Memory Extension (VME) ==================

int _vme_init(void *(*pgalloc)(size_t size), void (*pgfree)(void *));
int _protect(_AddressSpace *as);
void _unprotect(_AddressSpace *as);
int _map(_AddressSpace *as, void *va, void *pa, int prot);
_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack,
                                 void *entry, void *args);

// ================= Multi-Processor Extension (MPE) =================

int _mpe_init(void (*entry)());
int _ncpu();
int _cpu();
intptr_t _atomic_xchg(volatile intptr_t *addr, intptr_t newval);

#ifdef __cplusplus
}
#endif

#endif
