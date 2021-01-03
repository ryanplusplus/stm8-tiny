.module interrupts

.area CODE

.globl _interrupts_save
_interrupts_save:
  push  cc
  pop   a
  sim
  ret

.globl _interrupts_restore
_interrupts_restore:
  ld    a, (3, sp)
  push  a
  pop   cc
  ret
