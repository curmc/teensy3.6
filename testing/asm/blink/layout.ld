/* MEMORY - Tells the linker how the storage address space should be broken up */
MEMORY {
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  RAM (rwx)  : ORIGIN = 0x1FFF8000, LENGTH = 64K
}

/* SECTIONS - Tells Linker where and what order to place the various parts of the program */
SECTIONS {
  /* . denotes start of the address space */
  . = 0x00000000;
  /* matches all the text to its place in the flash */
  .text : { 
    KEEP(*(.vectors))
    . = 0x400;
    KEEP(*(.flashconfig*))
    *(.startup)
    *(.text)
  } > FLASH

  _estack = ORIGIN(RAM) + LENGTH(RAM); /* stack pointer */
}

