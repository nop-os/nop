OUTPUT_FORMAT("binary")
ENTRY(nex_start)

SECTIONS {
  . = 0xC0000000;

  .header : {
    *(.header*)
  }

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
