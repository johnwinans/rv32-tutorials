/* Script for -z combreloc severely hacked by JRW */
/* Copyright (C) 2014-2020 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

MEMORY
{
   RAM (rwx)  : ORIGIN = 0x80000000, LENGTH = 128M
}


ENTRY(_start)

SECTIONS
{
	. = ORIGIN(RAM);

	PROVIDE(__ram_origin__ = ORIGIN(RAM));
	PROVIDE(__ram_end__ = ORIGIN(RAM) + LENGTH(RAM));
	PROVIDE(__stack_top__ = 0x80800000);

	.init :
	{
		KEEP (*(SORT_NONE(.init)))
	} >RAM 

	.text :
	{
		*(.text )
	} >RAM 

	.fini :
	{
		KEEP (*(SORT_NONE(.fini)))
	} >RAM 
	PROVIDE (__etext = .);
	PROVIDE (_etext = .);
	PROVIDE (etext = .);

	.rodata : { *(.rodata .rodata.* .gnu.linkonce.r.*) } >RAM 
	.rodata1 : { *(.rodata1) } >RAM 
	.sdata2 :
	{
		*(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
	} >RAM 

	.sbss2 : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) } >RAM 

  /* Thread Local Storage sections  */
  .tdata	  :
   {
     PROVIDE_HIDDEN (__tdata_start = .);
     *(.tdata .tdata.* .gnu.linkonce.td.*)
   } >RAM 
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } >RAM 
  .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >RAM 
  .init_array    :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >RAM 
  .fini_array    :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >RAM 


  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >RAM 

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >RAM 

  .data           :
  {
	. = ALIGN(4096);	/* JRW */

    __DATA_BEGIN__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  } >RAM 

  .data1          : { *(.data1) } >RAM 
  .got            : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) } >RAM 

  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata          :
  {
    __SDATA_BEGIN__ = .;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } >RAM 
  _edata = .; PROVIDE (edata = .);
  . = .;
  __bss_start = .;
  .sbss           :
  {
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >RAM 

  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we do not
      pad the .data section.  */
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  } >RAM 
  . = ALIGN(32 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(32 / 8);
  __BSS_END__ = .;
    __global_pointer$ = MIN(__SDATA_BEGIN__ + 0x800,
		            MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800));
  _end = .; PROVIDE (end = .);

}
