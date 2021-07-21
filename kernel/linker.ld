OUTPUT_FORMAT("binary")
ENTRY(nop)

SECTIONS {
  . = 0x00010000;

  .entry : {
    *(.entry*)
  }

  .text : {
    *(.text*)
  }

  .rodata : {
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(COMMON)
    *(.bss*)
  }
}
