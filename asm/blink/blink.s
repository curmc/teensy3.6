.syntax unified

.section "vectors"
// Interupt vector definitions
.long _estack // 0 ARM: Initial stack pointer
.long _startup // 1 ARM: Initial program counter
.long _halt
.long _halt
.long _halt
.long _halt
.long _halt

.section ".flashconfig"
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFE

.section ".startup","x",%progbits
.thumb_func
.global _startup

_startup:
    mov     r0,#0
    mov     r1,#0
    mov     r2,#0
    mov     r3,#0
    mov     r4,#0
    mov     r5,#0
    mov     r6,#0
    mov     r7,#0
    mov     r8,#0
    mov     r9,#0
    mov     r10,#0
    mov     r11,#0
    mov     r12,#0
    cpsid i // Disable interrupts

    // Unlock watchdog - page 478
    ldr r6, = 0x4005200E // address from page 473
    ldr r0, = 0xC520
    strh r0, [r6]
    ldr r0, = 0xD928
    strh r0, [r6]

    // Disable watchdog - page 468
    ldr r6, = 0x40052000 // address from page 473
    ldr r0, = 0x01D2
    strh r0, [r6]

    cpsie i // Enable interrupts

    // Enable system clock on all GPIO ports - page 254
    ldr r6, = 0x40048038
    ldr r0, = 0x00043F82 // 0b1000011111110000010
    str r0, [r6]

    // Configure the led pin
    ldr r6, = 0x4004B014 // PORTC_PCR5 - page 223/227
    ldr r0, = 0x00000143 // Enables GPIO | DSE | PULL_ENABLE | PULL_SELECT - page 227
    str r0, [r6]

    // Set the led pin to output
    ldr r6, = 0x400FF094 // GPIOC_PDDR - page 1334,1337
    ldr r0, = 0x20 // pin 5 on port c
    str r0, [r6]

loop:
    bl led_on
    bl delay
    bl led_off
    bl delay
    b loop

    // Function to turn the led off
    .thumb_func
    .global led_off
led_off:
    ldr r6, = 0x400FF080 // GPIOC_PDOR - page 1334,1335
    ldr r0, = 0x0
    str r0, [r6]
    mov pc, r14

    // Function to turn the led on
    .thumb_func
    .global led_on
led_on:
    ldr r6, = 0x400FF080 // GPIOC_PDOR - page 1334,1335
    ldr r0, = 0x20
    str r0, [r6]
    mov pc, r14

    // Uncalibrated busy wait
    .thumb_func
    .global delay
delay:
    ldr r1, = 0x2625A0
delay_loop:
    sub r1, r1, #1
    cmp r1, #0
    bne delay_loop
    mov pc, r14

_halt: b _halt
    .end
