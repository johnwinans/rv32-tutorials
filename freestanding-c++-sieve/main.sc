	.file	"main.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.type	_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0, @function
_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0:
.LFB1015:
	.cfi_startproc
	beq	a0,zero,.L61
	addi	sp,sp,-48
	.cfi_def_cfa_offset 48
	sw	s5,20(sp)
	sw	ra,44(sp)
	sw	s0,40(sp)
	sw	s1,36(sp)
	sw	s2,32(sp)
	sw	s3,28(sp)
	sw	s4,24(sp)
	sw	s6,16(sp)
	sw	s7,12(sp)
	sw	s8,8(sp)
	.cfi_offset 21, -28
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	.cfi_offset 9, -12
	.cfi_offset 18, -16
	.cfi_offset 19, -20
	.cfi_offset 20, -24
	.cfi_offset 22, -32
	.cfi_offset 23, -36
	.cfi_offset 24, -40
	mv	s5,a0
.L19:
	lw	s6,12(s5)
	beq	s6,zero,.L3
.L18:
	lw	s7,12(s6)
	beq	s7,zero,.L4
.L17:
	lw	s8,12(s7)
	beq	s8,zero,.L5
.L16:
	lw	s0,12(s8)
	beq	s0,zero,.L6
.L15:
	lw	s1,12(s0)
	beq	s1,zero,.L7
.L14:
	lw	s4,12(s1)
	beq	s4,zero,.L8
.L13:
	lw	s2,12(s4)
	beq	s2,zero,.L9
.L12:
	lw	s3,12(s2)
	beq	s3,zero,.L10
.L11:
	lw	a0,12(s3)
	call	_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0
	mv	a0,s3
	lw	s3,8(s3)
	li	a1,20
	call	_ZdlPvj
	bne	s3,zero,.L11
.L10:
	lw	s3,8(s2)
	li	a1,20
	mv	a0,s2
	call	_ZdlPvj
	beq	s3,zero,.L9
	mv	s2,s3
	j	.L12
.L7:
	lw	s1,8(s0)
	li	a1,20
	mv	a0,s0
	call	_ZdlPvj
	beq	s1,zero,.L6
	mv	s0,s1
	j	.L15
.L8:
	lw	s2,8(s1)
	li	a1,20
	mv	a0,s1
	call	_ZdlPvj
	beq	s2,zero,.L7
	mv	s1,s2
	j	.L14
.L6:
	lw	s0,8(s8)
	li	a1,20
	mv	a0,s8
	call	_ZdlPvj
	beq	s0,zero,.L5
	mv	s8,s0
	j	.L16
.L9:
	lw	s2,8(s4)
	li	a1,20
	mv	a0,s4
	call	_ZdlPvj
	beq	s2,zero,.L8
	mv	s4,s2
	j	.L13
.L5:
	lw	s0,8(s7)
	li	a1,20
	mv	a0,s7
	call	_ZdlPvj
	beq	s0,zero,.L4
	mv	s7,s0
	j	.L17
.L4:
	lw	s0,8(s6)
	li	a1,20
	mv	a0,s6
	call	_ZdlPvj
	beq	s0,zero,.L3
	mv	s6,s0
	j	.L18
.L3:
	lw	s0,8(s5)
	li	a1,20
	mv	a0,s5
	call	_ZdlPvj
	beq	s0,zero,.L62
	mv	s5,s0
	j	.L19
.L62:
	lw	ra,44(sp)
	.cfi_restore 1
	lw	s0,40(sp)
	.cfi_restore 8
	lw	s1,36(sp)
	.cfi_restore 9
	lw	s2,32(sp)
	.cfi_restore 18
	lw	s3,28(sp)
	.cfi_restore 19
	lw	s4,24(sp)
	.cfi_restore 20
	lw	s5,20(sp)
	.cfi_restore 21
	lw	s6,16(sp)
	.cfi_restore 22
	lw	s7,12(sp)
	.cfi_restore 23
	lw	s8,8(sp)
	.cfi_restore 24
	addi	sp,sp,48
	.cfi_def_cfa_offset 0
	jr	ra
.L61:
	ret
	.cfi_endproc
.LFE1015:
	.size	_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0, .-_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
.LC0:
	.string	"mhartid=0x%8.8x, misa=0x%8.8x\n"
	.align	2
.LC1:
	.string	"rv32"
	.align	2
.LC2:
	.string	"%c"
	.align	2
.LC3:
	.string	"\n"
	.align	2
.LC4:
	.string	"cmain"
	.align	2
.LC5:
	.string	"main.cc"
	.align	2
.LC6:
	.string	"*** %s %d %s(): "
	.align	2
.LC7:
	.string	"constructed the set\n"
	.align	2
.LC8:
	.string	"returning\n"
	.text
	.align	2
	.globl	cmain
	.type	cmain, @function
cmain:
.LFB961:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA961
	addi	sp,sp,-64
	.cfi_def_cfa_offset 64
	sw	ra,60(sp)
	sw	s0,56(sp)
	sw	s1,52(sp)
	sw	s2,48(sp)
	sw	s3,44(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	.cfi_offset 9, -12
	.cfi_offset 18, -16
	.cfi_offset 19, -20
 #APP
# 19 "main.cc" 1
	csrrs a1,mhartid,x0
# 0 "" 2
# 21 "main.cc" 1
	csrrs s1,misa,x0
# 0 "" 2
 #NO_APP
	lui	a0,%hi(.LC0)
	mv	a2,s1
	addi	a0,a0,%lo(.LC0)
.LEHB0:
	call	printf
	lui	a0,%hi(.LC1)
	addi	a0,a0,%lo(.LC1)
	call	printf
	li	s0,97
	lui	s3,%hi(.LC2)
	li	s2,123
	j	.L65
.L64:
	addi	s0,s0,1
	srli	s1,s1,1
	beq	s0,s2,.L80
.L65:
	andi	a5,s1,1
	beq	a5,zero,.L64
	mv	a1,s0
	addi	a0,s3,%lo(.LC2)
	addi	s0,s0,1
	call	printf
	srli	s1,s1,1
	bne	s0,s2,.L65
.L80:
	lui	a0,%hi(.LC3)
	addi	a0,a0,%lo(.LC3)
	call	printf
.LEHE0:
	lui	s3,%hi(.LC4)
	lui	s2,%hi(.LC5)
	lui	s1,%hi(.LC6)
	addi	a5,sp,12
	addi	a3,s3,%lo(.LC4)
	li	a2,36
	addi	a1,s2,%lo(.LC5)
	addi	a0,s1,%lo(.LC6)
	sw	zero,12(sp)
	sw	zero,16(sp)
	sw	a5,20(sp)
	sw	a5,24(sp)
	sw	zero,28(sp)
.LEHB1:
	call	printf
	lui	a0,%hi(.LC7)
	addi	a0,a0,%lo(.LC7)
	call	printf
	lui	s0,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(s0)
	lw	a0,8(a5)
	call	fflush
	li	a1,1000
	addi	a0,sp,8
	call	_Z5sieveRSt3setIiSt4lessIiESaIiEEi
	addi	a0,sp,8
	call	_Z12print_primesRKSt3setIiSt4lessIiESaIiEE
	addi	a3,s3,%lo(.LC4)
	li	a2,41
	addi	a1,s2,%lo(.LC5)
	addi	a0,s1,%lo(.LC6)
	call	printf
	lui	a0,%hi(.LC8)
	addi	a0,a0,%lo(.LC8)
	call	printf
	lw	a5,%lo(_impure_ptr)(s0)
	lw	a0,8(a5)
	call	fflush
.LEHE1:
	lw	s0,16(sp)
	beq	s0,zero,.L72
.L67:
	lw	a0,12(s0)
	call	_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0
	mv	a0,s0
	lw	s0,8(s0)
	li	a1,20
	call	_ZdlPvj
	bne	s0,zero,.L67
.L72:
	lw	ra,60(sp)
	.cfi_remember_state
	.cfi_restore 1
	lw	s0,56(sp)
	.cfi_restore 8
	lw	s1,52(sp)
	.cfi_restore 9
	lw	s2,48(sp)
	.cfi_restore 18
	lw	s3,44(sp)
	.cfi_restore 19
	li	a0,0
	addi	sp,sp,64
	.cfi_def_cfa_offset 0
	jr	ra
.L69:
	.cfi_restore_state
	mv	s0,a0
	lw	a0,16(sp)
	call	_ZNSt8_Rb_treeIiiSt9_IdentityIiESt4lessIiESaIiEE8_M_eraseEPSt13_Rb_tree_nodeIiE.isra.0
	mv	a0,s0
.LEHB2:
	call	_Unwind_Resume
.LEHE2:
	.cfi_endproc
.LFE961:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"aw",@progbits
.LLSDA961:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x27
	.4byte	.LEHB0-.LFB961
	.4byte	.LEHE0-.LEHB0
	.4byte	0
	.byte	0
	.4byte	.LEHB1-.LFB961
	.4byte	.LEHE1-.LEHB1
	.4byte	.L69-.LFB961
	.byte	0
	.4byte	.LEHB2-.LFB961
	.4byte	.LEHE2-.LEHB2
	.4byte	0
	.byte	0
	.text
	.size	cmain, .-cmain
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.sdata.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align	2
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 4
DW.ref.__gxx_personality_v0:
	.word	__gxx_personality_v0
	.ident	"GCC: (GNU) 11.1.0"
