#ifndef __RISCV32_RTL_H__
#define __RISCV32_RTL_H__

#include "rtl/rtl.h"

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  if (r != 0) { rtl_mv(dest, &reg_l(r)); }
  else { rtl_li(dest, 0); }
}

static inline void rtl_sr(int r, const rtlreg_t *src1, int width) {
  if(*src1 ==  0x6d305b1b)printf("cpu.pc = %d\n",cpu.pc);
  assert((*src1 !=  0x6d305b1b));
  if (r != 0) { rtl_mv(&reg_l(r), src1); }
}

#endif
