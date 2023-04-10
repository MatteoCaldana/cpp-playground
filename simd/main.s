	.file	"main.cpp"
	.text
	.p2align 4
	.type	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0, @function
_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0:
.LFB12145:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	(%rdi), %rax
	movq	-24(%rax), %rax
	movq	240(%rdi,%rax), %r12
	testq	%r12, %r12
	je	.L7
	cmpb	$0, 56(%r12)
	movq	%rdi, %rbp
	je	.L3
	movsbl	67(%r12), %esi
.L4:
	movq	%rbp, %rdi
	call	_ZNSo3putEc
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rax, %rdi
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	_ZNSo5flushEv
.L3:
	.cfi_restore_state
	movq	%r12, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%r12), %rax
	movl	$10, %esi
	movq	%r12, %rdi
	call	*48(%rax)
	movsbl	%al, %esi
	jmp	.L4
.L7:
	call	_ZSt16__throw_bad_castv
	.cfi_endproc
.LFE12145:
	.size	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0, .-_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
	.section	.rodata._Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"cannot create std::vector larger than max_size()"
	.section	.text._Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE,"axG",@progbits,_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE,comdat
	.p2align 4
	.weak	_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE
	.type	_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE, @function
_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE:
.LFB11155:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	(%rsi), %r8
	movq	8(%rsi), %rcx
	subq	%r8, %rcx
	sarq	$2, %rcx
	movq	%rcx, %r13
	movq	%rcx, %rax
	shrq	$3, %r13
	shrq	$61, %rax
	jne	.L22
	vpxor	%xmm0, %xmm0, %xmm0
	movq	%r13, %rbx
	movq	$0, 16(%rdi)
	movq	%rdi, %r12
	vmovdqu	%xmm0, (%rdi)
	salq	$5, %rbx
	testq	%r13, %r13
	je	.L10
	movq	%rsi, %rbp
	movq	%rbx, %rdi
	movl	$32, %esi
	call	_ZnwmSt11align_val_t
	movq	0(%rbp), %r8
	movq	8(%rbp), %rcx
	vmovq	%rax, %xmm3
	leaq	(%rax,%rbx), %rdx
	movq	%rax, %rsi
	vpunpcklqdq	%xmm3, %xmm3, %xmm0
	subq	%r8, %rcx
	movq	%rdx, 16(%r12)
	leaq	32(%rax), %rax
	vmovdqu	%xmm0, (%r12)
	vpxor	%xmm0, %xmm0, %xmm0
	sarq	$2, %rcx
	vmovdqa	%xmm0, -32(%rax)
	vmovdqa	%xmm0, -16(%rax)
	cmpq	$1, %r13
	je	.L11
	cmpq	%rax, %rdx
	je	.L12
	.p2align 4,,10
	.p2align 3
.L13:
	vmovdqa	(%rsi), %xmm1
	addq	$32, %rax
	vmovdqa	%xmm1, -32(%rax)
	vmovdqa	16(%rsi), %xmm2
	vmovdqa	%xmm2, -16(%rax)
	cmpq	%rax, %rdx
	jne	.L13
.L12:
	movq	%rdx, 8(%r12)
	xorl	%eax, %eax
	testq	%rcx, %rcx
	je	.L8
	.p2align 4,,10
	.p2align 3
.L14:
	movq	%rax, %rdx
	vmovss	(%r8,%rax,4), %xmm0
	movq	%rax, %rdi
	addq	$1, %rax
	shrq	$3, %rdx
	andl	$7, %edi
	salq	$5, %rdx
	addq	%rsi, %rdx
	vmovss	%xmm0, (%rdx,%rdi,4)
	cmpq	%rcx, %rax
	jne	.L14
.L8:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	.cfi_restore_state
	movq	$0, (%rdi)
	xorl	%esi, %esi
	xorl	%edx, %edx
	movq	$0, 16(%rdi)
	jmp	.L12
	.p2align 4,,10
	.p2align 3
.L11:
	movq	%rax, %rdx
	jmp	.L12
.L22:
	movl	$.LC0, %edi
	call	_ZSt20__throw_length_errorPKc
	.cfi_endproc
.LFE11155:
	.size	_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE, .-_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC7:
	.string	"ms\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB8:
	.section	.text.startup,"ax",@progbits
.LHOTB8:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11149:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA11149
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	vpxor	%xmm0, %xmm0, %xmm0
	movl	$400000000, %edi
	pushq	-8(%r10)
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x70,0x6
	.cfi_escape 0x10,0xc,0x2,0x76,0x78
	pushq	%rbx
	subq	$120, %rsp
	.cfi_escape 0x10,0x3,0x2,0x76,0x68
	movq	$0, -128(%rbp)
	vmovdqa	%xmm0, -144(%rbp)
.LEHB0:
	call	_Znwm
.LEHE0:
	leaq	400000000(%rax), %rbx
	leaq	4(%rax), %rcx
	movq	%rax, -144(%rbp)
	movq	%rax, %r12
	movq	%rbx, -128(%rbp)
	cmpq	%rbx, %rcx
	je	.L26
	movq	%rcx, %rdi
	movl	$399999996, %edx
	xorl	%esi, %esi
	call	memset
	movq	%rax, %rcx
.L26:
	movq	%rbx, -136(%rbp)
	vmovss	.LC4(%rip), %xmm3
	vxorps	%xmm2, %xmm2, %xmm2
	movl	$16807, %edx
	movl	$0x37034c00, (%r12)
	vmovss	.LC5(%rip), %xmm1
	movabsq	$8589934597, %rdi
	.p2align 4,,10
	.p2align 3
.L25:
	imulq	$16807, %rdx, %rsi
	movq	%rsi, %rax
	mulq	%rdi
	movq	%rsi, %rax
	subq	%rdx, %rax
	shrq	%rax
	addq	%rax, %rdx
	shrq	$30, %rdx
	movq	%rdx, %rax
	salq	$31, %rax
	subq	%rdx, %rax
	subq	%rax, %rsi
	movq	%rsi, %rax
	movq	%rsi, %rdx
	subq	$1, %rax
	js	.L27
	vcvtsi2ssq	%rax, %xmm2, %xmm0
.L28:
	vxorps	%xmm4, %xmm4, %xmm4
	vaddss	%xmm4, %xmm0, %xmm0
	vmulss	%xmm3, %xmm0, %xmm0
	vcomiss	%xmm1, %xmm0
	jnb	.L29
	vaddss	%xmm4, %xmm0, %xmm0
	addq	$4, %rcx
	vmovss	%xmm0, -4(%rcx)
	cmpq	%rbx, %rcx
	jne	.L25
.L31:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	-136(%rbp), %rdx
	vxorpd	%xmm1, %xmm1, %xmm1
	movq	%rax, %r12
	movq	-144(%rbp), %rax
	cmpq	%rdx, %rax
	je	.L32
	vxorps	%xmm2, %xmm2, %xmm2
	.p2align 4,,10
	.p2align 3
.L33:
	vcvtss2sd	(%rax), %xmm2, %xmm0
	addq	$4, %rax
	vaddsd	%xmm0, %xmm1, %xmm1
	cmpq	%rdx, %rax
	jne	.L33
.L32:
	vcvtsd2ss	%xmm1, %xmm1, %xmm6
	vmovd	%xmm6, %ebx
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movl	$_ZSt4cout, %edi
	movabsq	$4835703278458516699, %rdx
	subq	%r12, %rax
	movq	%rax, %rcx
	imulq	%rdx
	sarq	$63, %rcx
	sarq	$18, %rdx
	movq	%rdx, %rsi
	subq	%rcx, %rsi
.LEHB1:
	call	_ZNSo9_M_insertIlEERSoT_
	movq	%rax, %rdi
	movl	$3, %edx
	movl	$.LC7, %esi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	vmovd	%ebx, %xmm7
	movl	$_ZSt4cout, %edi
	vcvtss2sd	%xmm7, %xmm7, %xmm0
	call	_ZNSo9_M_insertIdEERSoT_
	movq	%rax, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
	leaq	-144(%rbp), %rsi
	leaq	-112(%rbp), %rdi
	call	_Z9vectorizeIfL17VectorizationType1EEDaRKSt6vectorIT_SaIS2_EE
.LEHE1:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	-104(%rbp), %rdx
	vpxor	%xmm0, %xmm0, %xmm0
	movq	%rax, %rbx
	movq	-112(%rbp), %rax
	vmovdqa	%xmm0, -80(%rbp)
	vmovdqa	%xmm0, -64(%rbp)
	subq	%rax, %rdx
	movq	%rdx, %rdi
	shrq	$5, %rdi
	je	.L34
	addq	%rax, %rdx
	vxorps	%xmm0, %xmm0, %xmm0
	.p2align 4,,10
	.p2align 3
.L35:
	vaddps	(%rax), %ymm0, %ymm0
	addq	$32, %rax
	cmpq	%rax, %rdx
	jne	.L35
	vmovaps	%ymm0, -80(%rbp)
	vzeroupper
.L34:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movl	$_ZSt4cout, %edi
	movabsq	$4835703278458516699, %rdx
	subq	%rbx, %rax
	movq	%rax, %rcx
	imulq	%rdx
	sarq	$63, %rcx
	sarq	$18, %rdx
	movq	%rdx, %rsi
	subq	%rcx, %rsi
.LEHB2:
	call	_ZNSo9_M_insertIlEERSoT_
	movq	%rax, %rdi
	movl	$3, %edx
	movl	$.LC7, %esi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	vmovaps	-80(%rbp), %xmm0
	vxorps	%xmm6, %xmm6, %xmm6
	vmovaps	-80(%rbp), %ymm7
	movl	$_ZSt4cout, %edi
	vaddss	%xmm6, %xmm0, %xmm1
	vshufps	$85, %xmm0, %xmm0, %xmm2
	vaddss	%xmm1, %xmm2, %xmm2
	vunpckhps	%xmm0, %xmm0, %xmm1
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm1, %xmm0, %xmm1
	vextractf128	$0x1, %ymm7, %xmm0
	vshufps	$85, %xmm0, %xmm0, %xmm2
	vaddss	%xmm1, %xmm0, %xmm1
	vaddss	%xmm1, %xmm2, %xmm2
	vunpckhps	%xmm0, %xmm0, %xmm1
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm0, %xmm1, %xmm0
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	vzeroupper
	call	_ZNSo9_M_insertIdEERSoT_
	movq	%rax, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
.LEHE2:
	movq	-112(%rbp), %rdi
	testq	%rdi, %rdi
	je	.L36
	movq	-96(%rbp), %rsi
	movl	$32, %edx
	subq	%rdi, %rsi
	call	_ZdlPvmSt11align_val_t
.L36:
	movq	-144(%rbp), %rdi
	testq	%rdi, %rdi
	je	.L50
	movq	-128(%rbp), %rsi
	subq	%rdi, %rsi
	call	_ZdlPvm
.L50:
	addq	$120, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L27:
	.cfi_restore_state
	movq	%rax, %rsi
	andl	$1, %eax
	shrq	%rsi
	orq	%rax, %rsi
	vcvtsi2ssq	%rsi, %xmm2, %xmm0
	vaddss	%xmm0, %xmm0, %xmm0
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L29:
	movl	$0x3f7fffff, (%rcx)
	addq	$4, %rcx
	cmpq	%rcx, %rbx
	jne	.L25
	jmp	.L31
.L44:
	movq	%rax, %r12
	jmp	.L38
.L43:
	movq	%rax, %r12
	vzeroupper
	jmp	.L40
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA11149:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE11149-.LLSDACSB11149
.LLSDACSB11149:
	.uleb128 .LEHB0-.LFB11149
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB11149
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L43-.LFB11149
	.uleb128 0
	.uleb128 .LEHB2-.LFB11149
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L44-.LFB11149
	.uleb128 0
.LLSDACSE11149:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDAC11149
	.type	main.cold, @function
main.cold:
.LFSB11149:
.L38:
	.cfi_escape 0xf,0x3,0x76,0x70,0x6
	.cfi_escape 0x10,0x3,0x2,0x76,0x68
	.cfi_escape 0x10,0x6,0x2,0x76,0
	.cfi_escape 0x10,0xc,0x2,0x76,0x78
	movq	-112(%rbp), %rdi
	movq	-96(%rbp), %rsi
	subq	%rdi, %rsi
	testq	%rdi, %rdi
	je	.L63
	movl	$32, %edx
	vzeroupper
	call	_ZdlPvmSt11align_val_t
.L40:
	movq	-144(%rbp), %rdi
	movq	-128(%rbp), %rsi
	subq	%rdi, %rsi
	testq	%rdi, %rdi
	je	.L41
	call	_ZdlPvm
.L41:
	movq	%r12, %rdi
.LEHB3:
	call	_Unwind_Resume
.LEHE3:
.L63:
	vzeroupper
	jmp	.L40
	.cfi_endproc
.LFE11149:
	.section	.gcc_except_table
.LLSDAC11149:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC11149-.LLSDACSBC11149
.LLSDACSBC11149:
	.uleb128 .LEHB3-.LCOLDB8
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
.LLSDACSEC11149:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE8:
	.section	.text.startup
.LHOTE8:
	.p2align 4
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB12139:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE12139:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC4:
	.long	805306368
	.align 4
.LC5:
	.long	1065353216
	.hidden	__dso_handle
	.ident	"GCC: (mk-2022.0) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
