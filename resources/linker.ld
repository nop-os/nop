OUTPUT_FORMAT("binary")
ENTRY(nop_cs_16)

SECTIONS {
  . = 0x8000;

  .text BLOCK(4K) : ALIGN(4K) {
    *(.text)
  }

  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata)
  }

  .data BLOCK(4K) : ALIGN(4K) {
    *(.data)
  }

  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss)
  }
}
