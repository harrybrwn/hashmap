	.file	"hashmap.c"
	.text
	.globl	djb2
	.type	djb2, @function
djb2:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	$5381, -8(%rbp)
	jmp	.L2
.L3:
	movq	-8(%rbp), %rax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movl	-12(%rbp), %eax
	cltq
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
.L2:
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -24(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jne	.L3
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	djb2, .-djb2
	.globl	sdbm
	.type	sdbm, @function
sdbm:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L6
.L7:
	movl	-12(%rbp), %eax
	cltq
	movq	-8(%rbp), %rdx
	salq	$6, %rdx
	addq	%rax, %rdx
	movq	-8(%rbp), %rax
	salq	$16, %rax
	addq	%rdx, %rax
	subq	-8(%rbp), %rax
	movq	%rax, -8(%rbp)
.L6:
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -24(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jne	.L7
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	sdbm, .-sdbm
	.globl	rshash
	.type	rshash, @function
rshash:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	$63689, -24(%rbp)
	movq	$378551, -8(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L10
.L11:
	movq	-16(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, %rdx
	movl	-28(%rbp), %eax
	cltq
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	imulq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)
.L10:
	movq	-40(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -40(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -28(%rbp)
	cmpl	$0, -28(%rbp)
	jne	.L11
	movq	-16(%rbp), %rax
	andl	$2147483647, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	rshash, .-rshash
	.globl	fnv_1
	.type	fnv_1, @function
fnv_1:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	$16777619, -8(%rbp)
	movl	$2166136261, %eax
	movq	%rax, -16(%rbp)
	jmp	.L14
.L15:
	movl	-20(%rbp), %eax
	cltq
	xorq	-16(%rbp), %rax
	imulq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L14:
	movq	-40(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -40(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -20(%rbp)
	cmpl	$0, -20(%rbp)
	jne	.L15
	movq	-16(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	fnv_1, .-fnv_1




	.type	prehash, @function
prehash:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	$16777619, -8(%rbp)
	movl	$2166136261, %eax
	movq	%rax, -16(%rbp)
	jmp	.L18
.L19:
	movl	-20(%rbp), %eax
	cltq
	xorq	-16(%rbp), %rax
	imulq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L18:
	movq	-40(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -40(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -20(%rbp)
	cmpl	$0, -20(%rbp)
	jne	.L19
	movq	-16(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	prehash, .-prehash





	.globl	New_Map
	.type	New_Map, @function
New_Map:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$32, %edi
	call	create_map
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	New_Map, .-New_Map
	.globl	Create_Map
	.type	Create_Map, @function
Create_Map:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	create_map
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	Create_Map, .-Create_Map
	.globl	Map_close
	.type	Map_close, @function
Map_close:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L26
.L27:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	delete_tree
	addq	$1, -8(%rbp)
.L26:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	-8(%rbp), %rax
	ja	.L27
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	Map_close, .-Map_close
	.globl	Map_put
	.type	Map_put, @function
Map_put:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	prehash
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-8(%rbp), %rax
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, %rax
	movl	%eax, -12(%rbp)
	movq	-8(%rbp), %rdx
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_new_node
	movq	%rax, %rcx
	movl	-12(%rbp), %edx
	movq	-24(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	add_node
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 16(%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	Map_put, .-Map_put
	.globl	Map_get
	.type	Map_get, @function
Map_get:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	prehash
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-24(%rbp), %rax
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, %rax
	movl	%eax, -28(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movl	-28(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L30
	movl	$0, %eax
	jmp	.L31
.L30:
	movq	-16(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-24(%rbp), %rax
	je	.L32
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	search
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L33
	movl	$0, %eax
	jmp	.L31
.L33:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	jmp	.L31
.L32:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
.L31:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	Map_get, .-Map_get
	.globl	Map_delete
	.type	Map_delete, @function
Map_delete:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	prehash
	movq	%rax, -40(%rbp)
	movq	-56(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-40(%rbp), %rax
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, -32(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L35
	movq	-56(%rbp), %rax
	movq	16(%rax), %rax
	leaq	-1(%rax), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, 16(%rax)
	jmp	.L34
.L35:
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	salq	$3, %rdx
	leaq	(%rax,%rdx), %rbx
	movq	-40(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_delete_node
	movq	%rax, (%rbx)
	movq	-56(%rbp), %rax
	movq	16(%rax), %rax
	leaq	-1(%rax), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, 16(%rax)
.L34:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	Map_delete, .-Map_delete
	.globl	Map_resize
	.type	Map_resize, @function
Map_resize:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	create_map
	movq	%rax, -16(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	16(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	$0, -24(%rbp)
	jmp	.L38
.L40:
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	(%rax), %rax
	movq	-24(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L39
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	copy_nodes
.L39:
	addq	$1, -24(%rbp)
.L38:
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	8(%rax), %rax
	cmpq	-24(%rbp), %rax
	ja	.L40
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	Map_close
	movq	-40(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	Map_resize, .-Map_resize
	.globl	Map_keys
	.type	Map_keys, @function
Map_keys:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$0, -20(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L42
.L44:
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L43
	movl	-20(%rbp), %edx
	movq	-48(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	node_keys
	movl	%eax, -20(%rbp)
.L43:
	addq	$1, -16(%rbp)
.L42:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	-16(%rbp), %rax
	ja	.L44
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	Map_keys, .-Map_keys
	.type	node_rotateleft, @function
node_rotateleft:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	25(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 17(%rax)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 25(%rax)
	movq	-24(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L46
	movq	-24(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L47
.L46:
	movl	$-1, %edx
.L47:
	movq	-24(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L48
	movq	-24(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L49
.L48:
	movl	$-1, %eax
.L49:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movb	%dl, 16(%rax)
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L50
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L51
.L50:
	movl	$-1, %edx
.L51:
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L52
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L53
.L52:
	movl	$-1, %eax
.L53:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movb	%dl, 16(%rax)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	node_rotateleft, .-node_rotateleft
	.type	node_rotateright, @function
node_rotateright:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	25(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	17(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 25(%rax)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 17(%rax)
	movq	-24(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L56
	movq	-24(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L57
.L56:
	movl	$-1, %edx
.L57:
	movq	-24(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L58
	movq	-24(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L59
.L58:
	movl	$-1, %eax
.L59:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movb	%dl, 16(%rax)
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L60
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L61
.L60:
	movl	$-1, %edx
.L61:
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L62
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L63
.L62:
	movl	$-1, %eax
.L63:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movb	%dl, 16(%rax)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	node_rotateright, .-node_rotateright
	.type	insert_node, @function
insert_node:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	33(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L66
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L67
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	leaq	25(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	insert_node
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L68
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L69
.L68:
	movl	$-1, %edx
.L69:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L70
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L71
.L70:
	movl	$-1, %eax
.L71:
	subl	%eax, %edx
	movl	%edx, %eax
	cmpl	$2, %eax
	jne	.L75
	movq	-32(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	movq	33(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L73
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateright
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
	jmp	.L75
.L73:
	movq	-24(%rbp), %rax
	movq	(%rax), %rbx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	movq	%rax, 25(%rbx)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateright
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
	jmp	.L75
.L67:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 25(%rax)
	jmp	.L75
.L66:
	movq	-32(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	33(%rax), %rax
	cmpq	%rax, %rdx
	jbe	.L75
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L76
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	leaq	17(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	insert_node
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L77
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L78
.L77:
	movl	$-1, %edx
.L78:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L79
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L80
.L79:
	movl	$-1, %eax
.L80:
	subl	%eax, %edx
	movl	%edx, %eax
	cmpl	$2, %eax
	jne	.L75
	movq	-32(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	movq	33(%rax), %rax
	cmpq	%rax, %rdx
	jbe	.L82
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
	jmp	.L75
.L82:
	movq	-24(%rbp), %rax
	movq	(%rax), %rbx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateright
	movq	%rax, 17(%rbx)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
	jmp	.L75
.L76:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 17(%rax)
.L75:
	movq	-24(%rbp), %rax
	movq	(%rax), %rcx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L83
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L84
.L83:
	movl	$-1, %edx
.L84:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L85
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L86
.L85:
	movl	$-1, %eax
.L86:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	addl	$1, %eax
	movb	%al, 16(%rcx)
	nop
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	insert_node, .-insert_node
	.type	search, @function
search:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-16(%rbp), %rax
	jne	.L88
	movq	-8(%rbp), %rax
	jmp	.L89
.L88:
	movq	-8(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-16(%rbp), %rax
	jbe	.L90
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	search
	jmp	.L89
.L90:
	movq	-8(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-16(%rbp), %rax
	jnb	.L91
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	search
	jmp	.L89
.L91:
	movl	$0, %eax
.L89:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	search, .-search
	.type	delete_tree, @function
delete_tree:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L94
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rax, %rdi
	call	delete_tree
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movq	%rax, %rdi
	call	delete_tree
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
.L94:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	delete_tree, .-delete_tree
	.section	.rodata
	.align 8
.LC0:
	.string	"Error: out of memory allocating nodes"
	.text
	.type	_new_node, @function
_new_node:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$41, %edi
	call	malloc
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L96
	movl	$.LC0, %edi
	call	perror
	movl	$0, %eax
	jmp	.L97
.L96:
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movb	$0, 16(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 25(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 17(%rax)
	movq	-8(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rdx, 33(%rax)
	movq	-8(%rbp), %rax
.L97:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	_new_node, .-_new_node
	.type	add_node, @function
add_node:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L99
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movq	-32(%rbp), %rax
	movq	%rax, (%rdx)
	jmp	.L102
.L99:
	movq	-8(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	%rax, %rdx
	jne	.L101
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movq	-32(%rbp), %rax
	movq	%rax, (%rdx)
	jmp	.L102
.L101:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	insert_node
.L102:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	add_node, .-add_node
	.type	min_node, @function
min_node:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.L104
.L105:
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movq	%rax, -8(%rbp)
.L104:
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	jne	.L105
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	min_node, .-min_node
	.type	_delete_node, @function
_delete_node:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L108
	movq	-40(%rbp), %rax
	jmp	.L109
.L108:
	movq	-40(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-48(%rbp), %rax
	jbe	.L110
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_delete_node
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 25(%rax)
	jmp	.L111
.L110:
	movq	-40(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-48(%rbp), %rax
	jnb	.L112
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_delete_node
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 17(%rax)
	jmp	.L111
.L112:
	movq	-40(%rbp), %rax
	movq	33(%rax), %rax
	cmpq	-48(%rbp), %rax
	jne	.L111
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L113
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	jne	.L114
.L113:
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L115
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	%rax, -16(%rbp)
	jmp	.L116
.L115:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rax, -16(%rbp)
.L116:
	cmpq	$0, -16(%rbp)
	je	.L117
	movq	-40(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	(%rdx), %rcx
	movq	%rcx, (%rax)
	movq	8(%rdx), %rcx
	movq	%rcx, 8(%rax)
	movq	16(%rdx), %rcx
	movq	%rcx, 16(%rax)
	movq	24(%rdx), %rcx
	movq	%rcx, 24(%rax)
	movq	32(%rdx), %rcx
	movq	%rcx, 32(%rax)
	movzbl	40(%rdx), %edx
	movb	%dl, 40(%rax)
	jmp	.L118
.L117:
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	$0, -40(%rbp)
.L118:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	free
	jmp	.L111
.L114:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rax, %rdi
	call	min_node
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 33(%rax)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_delete_node
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 17(%rax)
.L111:
	cmpq	$0, -40(%rbp)
	jne	.L119
	movq	-40(%rbp), %rax
	jmp	.L109
.L119:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L120
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L121
.L120:
	movl	$-1, %edx
.L121:
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L122
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L123
.L122:
	movl	$-1, %eax
.L123:
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movb	%dl, 16(%rax)
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L124
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L125
.L124:
	movl	$-1, %edx
.L125:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L126
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L127
.L126:
	movl	$-1, %eax
.L127:
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -20(%rbp)
	cmpl	$1, -20(%rbp)
	jle	.L128
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L129
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L130
.L129:
	movl	$-1, %edx
.L130:
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L131
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L132
.L131:
	movl	$-1, %eax
.L132:
	subl	%eax, %edx
	movl	%edx, %eax
	testl	%eax, %eax
	js	.L128
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	node_rotateright
	jmp	.L109
.L128:
	cmpl	$1, -20(%rbp)
	jle	.L133
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L134
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L135
.L134:
	movl	$-1, %edx
.L135:
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L136
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L137
.L136:
	movl	$-1, %eax
.L137:
	subl	%eax, %edx
	movl	%edx, %eax
	testl	%eax, %eax
	jns	.L133
	movq	-40(%rbp), %rax
	movq	25(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 25(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	node_rotateright
	jmp	.L109
.L133:
	cmpl	$-1, -20(%rbp)
	jge	.L138
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L139
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L140
.L139:
	movl	$-1, %edx
.L140:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L141
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L142
.L141:
	movl	$-1, %eax
.L142:
	subl	%eax, %edx
	movl	%edx, %eax
	testl	%eax, %eax
	jg	.L138
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	jmp	.L109
.L138:
	cmpl	$-1, -20(%rbp)
	jge	.L143
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L144
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	25(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %edx
	jmp	.L145
.L144:
	movl	$-1, %edx
.L145:
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L146
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	17(%rax), %rax
	movzbl	16(%rax), %eax
	movzbl	%al, %eax
	jmp	.L147
.L146:
	movl	$-1, %eax
.L147:
	subl	%eax, %edx
	movl	%edx, %eax
	testl	%eax, %eax
	jle	.L143
	movq	-40(%rbp), %rax
	movq	17(%rax), %rax
	movq	%rax, %rdi
	call	node_rotateright
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 17(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	node_rotateleft
	jmp	.L109
.L143:
	movq	-40(%rbp), %rax
.L109:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	_delete_node, .-_delete_node
	.type	copy_nodes, @function
copy_nodes:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L149
	movq	-32(%rbp), %rax
	movq	25(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	copy_nodes
.L149:
	movq	-32(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L150
	movq	-32(%rbp), %rax
	movq	17(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	copy_nodes
.L150:
	movq	-32(%rbp), %rax
	movq	33(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	8(%rdx), %rcx
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, %rax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movq	33(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_new_node
	movq	%rax, %rcx
	movl	-4(%rbp), %edx
	movq	-24(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	add_node
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	copy_nodes, .-copy_nodes
	.section	.rodata
	.align 8
.LC1:
	.string	"Error: ran out of memory allocating a Map"
	.align 8
.LC2:
	.string	"Error: ran out of memory allocating a node array"
	.text
	.type	create_map, @function
create_map:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$24, %edi
	call	malloc
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L152
	movl	$.LC1, %edi
	call	perror
	movl	$0, %eax
	jmp	.L153
.L152:
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L154
	movl	$.LC2, %edi
	call	perror
	movq	-8(%rbp), %rax
	jmp	.L153
.L154:
	movq	$0, -16(%rbp)
	jmp	.L155
.L156:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	$0, (%rax)
	addq	$1, -16(%rbp)
.L155:
	movq	-16(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jb	.L156
	movq	-8(%rbp), %rax
	movq	$0, 16(%rax)
	movq	-8(%rbp), %rax
.L153:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	create_map, .-create_map
	.type	node_keys, @function
node_keys:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L158
	movl	-20(%rbp), %eax
	jmp	.L159
.L158:
	movl	-20(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -20(%rbp)
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, (%rdx)
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	testq	%rax, %rax
	je	.L160
	movq	-8(%rbp), %rax
	movq	25(%rax), %rax
	movl	-20(%rbp), %edx
	movq	-16(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	node_keys
	movl	%eax, -20(%rbp)
.L160:
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	testq	%rax, %rax
	je	.L161
	movq	-8(%rbp), %rax
	movq	17(%rax), %rax
	movl	-20(%rbp), %edx
	movq	-16(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	node_keys
	movl	%eax, -20(%rbp)
.L161:
	movl	-20(%rbp), %eax
.L159:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	node_keys, .-node_keys
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
