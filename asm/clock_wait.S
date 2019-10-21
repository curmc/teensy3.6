    AREA |.text, CODE, READONLY, ALIGN=2
    THUMB
    EXPORT setup

setup
  MOV.w     r0, #1000 ; 1000 microseconds
  b.w       sleep


sleep
  PUSH      {r3, r4, r5, lr}
  MOV       r4, r0
  BL        
  MOV       r5, r0
  CBNZ      r4, sleep+0x16
  POP       {r3, r4, r5, pc}
  SUBS      r4, #1
  BEQ.n     delay+0x28
  add.w     r5, r5, #1000
  bl        micros
  subs      r0, r0, r5
  CMP.w     r0, #1000
  BCS.n     delay+0xe
  BL        yield

micros:
    cpsid   i
    ldr     r3, [pc, #48]   ; (4e0 <micros+0x34>)
    ldr     r2, [pc, #48]   ; (4e4 <micros+0x38>)
    ldr     r1, [pc, #52]   ; (4e8 <micros+0x3c>)
    ldr     r3, [r3, #0]
    ldr     r2, [r2, #0]
    ldr     r1, [r1, #0]
    cpsie   i
    lsls    r1, r1, #5
    bpl.n   micros+0x1a
    cmp     r3, #50 ; 0x32 
    it      hi   
    addhi   r2, #1
    rsb     r3, r3, #179200 ; 0x2bc00
    movs    r0, #180        ; 0xb4 
    addw    r3, r3, #799    ; 0x31f
    udiv    r3, r3, r0
    mov.w   r0, #1000       ; 0x3e8
    mla     r0, r0, r2, r3
    bx      lr   
    nop
    .word   0xe000e018
    .word   0x1fff07f0
    .word   0xe000ed04

