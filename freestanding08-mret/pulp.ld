/*======================================================================*/
/* Default maven linker script                                          */
/*======================================================================*/
/* This is the default linker script for maven. It is based off of the
   mips idt32.ld linker script. I have added many more comments and
   tried to clean things up a bit. For more information about standard
   MIPS sections see Section 9.5 of "See MIPS Run Linux" by Dominic
   Sweetman. For more generic information about the init, fini, ctors,
   and dtors sections see the paper titled "ELF From the Programmers
   Perspective" by Hongiu Lu. */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH( "riscv" )

ENTRY( _start )

/* The GROUP command is special since the listed archives will be
   searched repeatedly until there are no new undefined references. We
   need this since -lc depends on -lgloss and -lgloss depends on -lc. I
   thought gcc would automatically include -lgcc when needed, but
   idt32.ld includes it explicitly here and I was seeing link errors
   without it. */

/*GROUP( -lc -lgloss -lgcc )*/



/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/
/* This is where we specify how the input sections map to output
   sections. The .= commands set the location counter, and the
   sections are inserted in increasing address order according to the
   location counter. The following statement will take all of the .bar
   input sections and reloate them into the .foo output section which
   starts at address 0x1000.

    . = 0.x1000;
    .foo : { *(.bar) }

   If we wrap an input specification with a KEEP command then it
   prevents it from being eliminted during "link-time garbage
   collection". I'm not sure what this is, so I just followed what was
   done in idt32.ld.

   We can also set a global external symbol to a specific address in the
   output binary with this syntax:

    _etext = .;
    PROVIDE( etext = . );

   This will set the global symbol _ftext to the current location. If we
   wrap this in a PROVIDE commad, the symbol will only be set if it is
   not defined. We do this with symbols which don't begin with an
   underscore since technically in ansi C someone might have a function
   with the same name (eg. etext).

   If we need to label the beginning of a section we need to make sure
   that the linker doesn't insert an orphan section inbetween where we
   set the symbol and the actual begining of the section. We can do that
   by assigning the location dot to itself.

    . = .
    _ftext = .;
    .text :
    { }

   */

SECTIONS
{

  /*--------------------------------------------------------------------*/
  /* Code and read-only segment                                         */
  /*--------------------------------------------------------------------*/

  /* Begining of code and text segment */
  . = 0x80000000;

    PROVIDE(__ram_origin__ = .);
    PROVIDE(__ram_end__ = 128M);
    PROVIDE(__stack_top__ = __ram_end__);

	. = ALIGN(4);

	.text : 
	{
		KEEP(*(SORT_NONE(.crt0)))
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
	}
/*
  .init : 
  {
    KEEP( *(.init) )
  }
*/
	. = ALIGN(4);

  .fini : 
  {
    KEEP( *(.fini) )
  }

	. = ALIGN(16);

  .rodata : 
  {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
  }

    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);


  /*--------------------------------------------------------------------*/
  /* Global constructor/destructor segement                             */
  /*--------------------------------------------------------------------*/

	. = ALIGN(16);

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  }

	. = ALIGN(16);
  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array ))
    PROVIDE_HIDDEN (__init_array_end = .);
  }

	. = ALIGN(16);
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array ))
    PROVIDE_HIDDEN (__fini_array_end = .);
  }




  /*--------------------------------------------------------------------*/
  /* Other misc gcc segments (this was in idt32.ld)                     */
  /*--------------------------------------------------------------------*/
  /* I am not quite sure about these sections but it seems they are for
     C++ exception handling. I think .jcr is for "Java Class
     Registration" but it seems to end up in C++ binaries as well. */
/*
  .eh_frame_hdr     : { *(.eh_frame_hdr)     }
  .eh_frame         : { KEEP( *(.eh_frame) ) }
  .gcc_except_table : { *(.gcc_except_table) }
  .jcr              : { KEEP (*(.jcr))       } 
*/

  /*--------------------------------------------------------------------*/
  /* Initialized data segment                                           */
  /*--------------------------------------------------------------------*/

  /* Start of initialized data segment */
  . = ALIGN(4096);
   _fdata = .;

  /* data: Writable data */
  .data : 
  {
	__DATA_BEGIN__ = .;

    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d.*)
  }

  /* End of initialized data segment */
/*
  PROVIDE( edata = . );
  _edata = .;
*/

  /* Have _gp point to middle of sdata/sbss to maximize displacement range */
  . = ALIGN(16);
  __global_pointer = . + 0x800;

  /* Writable small data segment */
  .sdata : 
  {
    *(.sdata)
    *(.sdata.*)
    *(.srodata.*)
    *(.gnu.linkonce.s.*)
  }

	_edata = .; 
	PROVIDE (edata = .);
	. = .;
	__bss_start = .;


  /*--------------------------------------------------------------------*/
  /* Uninitialized data segment                                         */
  /*--------------------------------------------------------------------*/

  /* Start of uninitialized data segment */
  . = ALIGN(16);
  _fbss = .;

  /* Writable uninitialized small data segment */
  .sbss : 
  {
    *(.sbss)
    *(.sbss.*)
    *(.gnu.linkonce.sb.*)
  }

  /* bss: Uninitialized writeable data section */
  . = .;
  _bss_start = .;
  .bss : 
  {
    *(.bss)
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
  }

/*
  . = ALIGN(32 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(32 / 8);
*/

  __BSS_END__ = .;
/*
	__global_pointer$ = MIN(__SDATA_BEGIN__ + 0x800,
                    MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800));
*/
	_end = .; 
	PROVIDE (end = .);


  /* End of uninitialized data segment (used by syscalls.c for heap) */
/*
  PROVIDE( end = . );
  _end = ALIGN(8);
*/
}

