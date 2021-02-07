	.file	"complex.cc"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata
.LC0:
	.string	"ctor, this = "
.LC1:
	.string	", real = "
.LC2:
	.string	", img = "
	.section	.text._ZN7complexC2Edd,"axG",@progbits,_ZN7complexC5Edd,comdat
	.align 2
	.weak	_ZN7complexC2Edd
	.type	_ZN7complexC2Edd, @function
_ZN7complexC2Edd:
.LFB972:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -24(%rbp)
	movsd	%xmm0, -32(%rbp)
	movsd	%xmm1, -40(%rbp)
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, (%rdx)
	movq	-24(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rax, 8(%rdx)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rbx
	movq	-24(%rbp), %rax
	movq	(%rax), %r12
	movl	$.LC0, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZNSolsEPKv
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%r12, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rbx, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE972:
	.size	_ZN7complexC2Edd, .-_ZN7complexC2Edd
	.weak	_ZN7complexC1Edd
	.set	_ZN7complexC1Edd,_ZN7complexC2Edd
	.section	.rodata
.LC3:
	.string	"dtor, this = "
	.section	.text._ZN7complexD2Ev,"axG",@progbits,_ZN7complexD5Ev,comdat
	.align 2
	.weak	_ZN7complexD2Ev
	.type	_ZN7complexD2Ev, @function
_ZN7complexD2Ev:
.LFB978:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rbx
	movq	-24(%rbp), %rax
	movq	(%rax), %r12
	movl	$.LC3, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZNSolsEPKv
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%r12, -32(%rbp)
	movsd	-32(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rbx, -32(%rbp)
	movsd	-32(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE978:
	.size	_ZN7complexD2Ev, .-_ZN7complexD2Ev
	.weak	_ZN7complexD1Ev
	.set	_ZN7complexD1Ev,_ZN7complexD2Ev
	.section	.text._ZNK7complex8get_realEv,"axG",@progbits,_ZNK7complex8get_realEv,comdat
	.align 2
	.weak	_ZNK7complex8get_realEv
	.type	_ZNK7complex8get_realEv, @function
_ZNK7complex8get_realEv:
.LFB980:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movsd	-16(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE980:
	.size	_ZNK7complex8get_realEv, .-_ZNK7complex8get_realEv
	.section	.text._ZNK7complex7get_imgEv,"axG",@progbits,_ZNK7complex7get_imgEv,comdat
	.align 2
	.weak	_ZNK7complex7get_imgEv
	.type	_ZNK7complex7get_imgEv, @function
_ZNK7complex7get_imgEv:
.LFB981:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	movsd	-16(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE981:
	.size	_ZNK7complex7get_imgEv, .-_ZNK7complex7get_imgEv
	.section	.rodata
.LC4:
	.string	"("
.LC5:
	.string	","
.LC6:
	.string	")"
	.text
	.globl	_ZlsRSoR7complex
	.type	_ZlsRSoR7complex, @function
_ZlsRSoR7complex:
.LFB982:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNK7complex7get_imgEv
	movsd	%xmm0, -40(%rbp)
	movq	-40(%rbp), %rbx
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNK7complex8get_realEv
	movsd	%xmm0, -40(%rbp)
	movq	-40(%rbp), %r12
	movq	-24(%rbp), %rax
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%r12, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$.LC5, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rbx, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	movq	%rax, %rdi
	call	_ZNSolsEd
	movl	$.LC6, %esi
	movq	%rax, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE982:
	.size	_ZlsRSoR7complex, .-_ZlsRSoR7complex
	.globl	main
	.type	main, @function
main:
.LFB983:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA983
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movabsq	$4616189618054758400, %rdx
	movabsq	$4617315517961601024, %rax
	leaq	-32(%rbp), %rcx
	movq	%rdx, -40(%rbp)
	movsd	-40(%rbp), %xmm1
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	movq	%rcx, %rdi
.LEHB0:
	call	_ZN7complexC1Edd
.LEHE0:
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$_ZSt4cout, %edi
.LEHB1:
	call	_ZlsRSoR7complex
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
.LEHE1:
	movl	$0, %ebx
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
.LEHB2:
	call	_ZN7complexD1Ev
.LEHE2:
	movl	%ebx, %eax
	jmp	.L14
.L13:
	movq	%rax, %rbx
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN7complexD1Ev
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB3:
	call	_Unwind_Resume
.LEHE3:
.L14:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE983:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA983:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE983-.LLSDACSB983
.LLSDACSB983:
	.uleb128 .LEHB0-.LFB983
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB983
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L13-.LFB983
	.uleb128 0
	.uleb128 .LEHB2-.LFB983
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LFB983
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
.LLSDACSE983:
	.text
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB994:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L15
	cmpl	$65535, -8(%rbp)
	jne	.L15
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE994:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__ZlsRSoR7complex, @function
_GLOBAL__sub_I__ZlsRSoR7complex:
.LFB995:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE995:
	.size	_GLOBAL__sub_I__ZlsRSoR7complex, .-_GLOBAL__sub_I__ZlsRSoR7complex
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__ZlsRSoR7complex
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
