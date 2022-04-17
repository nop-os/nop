ENTRY(nop)

SECTIONS {
  . = 0x00010000;
  
  .text : {
    *(.entry*)
    *(.text*)
    
    *(.rodata*)
    *(.data*)
    
    *(COMMON)
    *(.bss*)
  }
}
