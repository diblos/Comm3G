/******************************************************************************
 * AVR32 AT32UC3A3256 GNU LD script file.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  AVR32 AT32UC3A3256
 *
 * - author              Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 * SDRAM LINKER
	VERSION		Date		By		Description
	1.0			-			-		First Release
	1.1			20120505	CWAW	Rearrange SDRAM region, enlarge stack to 2M, INTRAM to 2M, and TEXT reduced to 16M			
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-avr32", "elf32-avr32",
	      "elf32-avr32")
OUTPUT_ARCH(avr32:uc)
ENTRY(_start)
SEARCH_DIR("/home/mingwbuild/mingwavr32/avr32/lib");
/* Do we need any of these for elf?
   __DYNAMIC = 0;    */
MEMORY
  {    
    /*
	SDRAM (rxai!w) : ORIGIN = 0xD0000000, LENGTH = 0x02000000
    INTRAM (wxa!ri) : ORIGIN = 0xD0100000, LENGTH = 64K
	*/
	SDRAM (rxai!w) : ORIGIN = 0xD0000000, LENGTH =  0x01000000
    INTRAM (wxa!ri) : ORIGIN = 0xD1000000, LENGTH = 0x00400000/*2M*/
  }
SECTIONS
{      
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = 0xD0000000); . = 0xD0000000;
  .interp         : { *(.interp) } >SDRAM AT>SDRAM
  .reset : {  *(.reset) } >SDRAM AT>SDRAM
  .hash           : { *(.hash) } >SDRAM AT>SDRAM
  .dynsym         : { *(.dynsym) } >SDRAM AT>SDRAM
  .dynstr         : { *(.dynstr) } >SDRAM AT>SDRAM
  .gnu.version    : { *(.gnu.version) } >SDRAM AT>SDRAM
  .gnu.version_d  : { *(.gnu.version_d) } >SDRAM AT>SDRAM
  .gnu.version_r  : { *(.gnu.version_r) } >SDRAM AT>SDRAM
  .rel.init       : { *(.rel.init) } >SDRAM AT>SDRAM
  .rela.init      : { *(.rela.init) } >SDRAM AT>SDRAM
  .rel.text       : { *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*) } >SDRAM AT>SDRAM
  .rela.text      : { *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) } >SDRAM AT>SDRAM
  .rel.fini       : { *(.rel.fini) } >SDRAM AT>SDRAM
  .rela.fini      : { *(.rela.fini) } >SDRAM AT>SDRAM
  .rel.rodata     : { *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*) } >SDRAM AT>SDRAM
  .rela.rodata    : { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) } >SDRAM AT>SDRAM
  .rel.data.rel.ro   : { *(.rel.data.rel.ro*) } >SDRAM AT>SDRAM
  .rela.data.rel.ro   : { *(.rel.data.rel.ro*) } >SDRAM AT>SDRAM
  .rel.data       : { *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*) } >SDRAM AT>SDRAM
  .rela.data      : { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) } >SDRAM AT>SDRAM
  .rel.tdata	  : { *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*) } >SDRAM AT>SDRAM
  .rela.tdata	  : { *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*) } >SDRAM AT>SDRAM
  .rel.tbss	  : { *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*) } >SDRAM AT>SDRAM
  .rela.tbss	  : { *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*) } >SDRAM AT>SDRAM
  .rel.ctors      : { *(.rel.ctors) } >SDRAM AT>SDRAM
  .rela.ctors     : { *(.rela.ctors) } >SDRAM AT>SDRAM
  .rel.dtors      : { *(.rel.dtors) } >SDRAM AT>SDRAM
  .rela.dtors     : { *(.rela.dtors) } >SDRAM AT>SDRAM
  .rel.got        : { *(.rel.got) } >SDRAM AT>SDRAM
  .rela.got       : { *(.rela.got) } >SDRAM AT>SDRAM
  .rel.bss        : { *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*) } >SDRAM AT>SDRAM
  .rela.bss       : { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) } >SDRAM AT>SDRAM
  .rel.plt        : { *(.rel.plt) } >SDRAM AT>SDRAM
  .rela.plt       : { *(.rela.plt) } >SDRAM AT>SDRAM
  .init           :
  {
    KEEP (*(.init))
  } >SDRAM AT>SDRAM =0xd703d703
  .plt            : { *(.plt) } >SDRAM AT>SDRAM
  .text           :
  {
    *(.text .stub .text.* .gnu.linkonce.t.*)
    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  } >SDRAM AT>SDRAM =0xd703d703
  .fini           :
  {
    KEEP (*(.fini))
  } >SDRAM AT>SDRAM =0xd703d703
  
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } >SDRAM AT>SDRAM
  .rodata1        : { *(.rodata1) } >SDRAM AT>SDRAM
  .eh_frame_hdr : { *(.eh_frame_hdr) } >SDRAM AT>SDRAM
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) } >SDRAM AT>SDRAM
  .gcc_except_table   : ONLY_IF_RO { KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) } >SDRAM AT>SDRAM
  .dalign	: { . = ALIGN(8); PROVIDE(_data_lma = .); } >SDRAM AT>SDRAM
  PROVIDE (_data = ORIGIN(INTRAM));
  . = ORIGIN(INTRAM);
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) } >INTRAM AT>SDRAM
  .gcc_except_table   : ONLY_IF_RW { KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) } >INTRAM AT>SDRAM
  /* Thread Local Storage sections  */
  .tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) } >INTRAM AT>SDRAM
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } >INTRAM AT>SDRAM
  /* Ensure the __preinit_array_start label is properly aligned.  We
     could instead move the label definition inside the section, but
     the linker would then create the section even if it turns out to
     be empty, which isn't pretty.  */
  PROVIDE (__preinit_array_start = ALIGN(32 / 8));
  .preinit_array     : { KEEP (*(.preinit_array)) } >INTRAM AT>SDRAM
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : { KEEP (*(.init_array)) } >INTRAM AT>SDRAM
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : { KEEP (*(.fini_array)) } >INTRAM AT>SDRAM
  PROVIDE (__fini_array_end = .);
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
    KEEP (*crtbegin*.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >INTRAM AT>SDRAM
  .dtors          :
  {
    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >INTRAM AT>SDRAM
  .jcr            : { KEEP (*(.jcr)) } >INTRAM AT>SDRAM
  .data.rel.ro : { *(.data.rel.ro.local) *(.data.rel.ro*) } >INTRAM AT>SDRAM
  .dynamic        : { *(.dynamic) } >INTRAM AT>SDRAM
  .got            : { *(.got.plt) *(.got) } >INTRAM AT>SDRAM
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
  } >INTRAM AT>SDRAM
  .data1          : { *(.data1) } >INTRAM AT>SDRAM
  .balign	: { . = ALIGN(8); _edata = .; } >INTRAM AT>SDRAM
  _edata = .;
  PROVIDE (edata = .);
  __bss_start = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(8);
  } >INTRAM AT>SDRAM
  . = ALIGN(8);
  _end = .;
  PROVIDE (end = .);
  __heap_start__ = ALIGN(8);
  . = ORIGIN(INTRAM) + LENGTH(INTRAM) - 0x00200000;
  __heap_end__ = .;
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  .stack         ORIGIN(INTRAM) + LENGTH(INTRAM) - 0x00200000 :
  {
    _stack = .;
    *(.stack)
    . = 0x00200000;
    _estack = .;
  } >INTRAM AT>SDRAM  
  
  /DISCARD/ : { *(.note.GNU-stack) }  
}

