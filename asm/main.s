	.arch armv5t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
	.global	rsa_edp
	.syntax unified
	.arm
	.type	rsa_edp, %function
rsa_edp:
	@ args = 1540, pretend = 8, frame = 1040
	@ frame_needed = 0, uses_anonymous_args = 0
	sub	sp, sp, #8
	push	{r4, r5, r6, r7, lr}
	mov	r7, r0
	mov	r6, r1
	sub	sp, sp, #1040
	sub	sp, sp, #4
	ldr	r5, .L6
	add	r4, sp, #1056
	ldr	ip, [r5]
	add	r4, r4, #8
	stm	r4, {r2, r3}
	mov	r1, #0
	mov	r2, #256
	add	r0, sp, #268
	str	ip, [sp, #1036]
	bl	memset
	mov	r2, #512
	mov	r1, #0
	add	r0, sp, #524
	bl	memset
	add	r3, sp, #12
	mov	r0, r3
	mov	r1, r7
	mov	r2, #256
	str	r3, [sp, #4]
	bl	memcpy
	add	r2, sp, #524
	mov	r1, r4
	ldr	r0, [sp, #4]
	bl	mm_conv
	add	r1, sp, #2336
	mov	r3, r4
	add	r0, sp, #524
	ldr	r2, [sp, #2600]
	add	r1, r1, #8
	bl	mm_exp
	add	r2, sp, #268
	mov	r1, r4
	add	r0, sp, #524
	bl	mm_redc
	mov	r2, #256
	add	r1, sp, #268
	mov	r0, r6
	bl	memcpy
	ldr	r2, [sp, #1036]
	ldr	r3, [r5]
	cmp	r2, r3
	bne	.L5
	add	sp, sp, #1040
	add	sp, sp, #4
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	add	sp, sp, #8
	bx	lr
.L5:
	bl	__stack_chk_fail
.L7:
	.align	2
.L6:
	.word	__stack_chk_guard
	.size	rsa_edp, .-rsa_edp
	.align	2
	.global	rsa_proc
	.syntax unified
	.arm
	.type	rsa_proc, %function
rsa_proc:
	@ args = 520, pretend = 8, frame = 4120
	@ frame_needed = 0, uses_anonymous_args = 0
	sub	sp, sp, #8
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	ldr	ip, .L23
	sub	sp, sp, #4096
	sub	sp, sp, #28
	ldr	ip, [ip]
	add	lr, sp, #4096
	add	r7, sp, #4096
	mov	r8, r0
	add	r0, sp, #3600
	add	lr, lr, #20
	add	r7, r7, #60
	mov	r6, r1
	mov	r10, r2
	mov	r1, #0
	mov	r2, #256
	add	r0, r0, #4
	str	r3, [r7, #8]!
	str	ip, [lr]
	bl	memset
	add	r0, sp, #3856
	mov	r2, #256
	mov	r1, #0
	add	r0, r0, #4
	bl	memset
	add	r3, sp, #1552
	add	r3, r3, #8
	sub	r3, r3, #4
	mov	r1, r3
	mov	r0, r7
	str	r3, [sp, #12]
	bl	mm_init
	add	r3, sp, #2576
	add	r3, r3, #8
	sub	r5, r3, #4
	add	r9, sp, #1040
	mov	r4, #256
	ldr	fp, .L23+4
	b	.L9
.L11:
	add	r0, sp, #3856
	mov	r2, r4
	mov	r1, #0
	add	r0, r0, #4
	bl	memset
	add	r0, sp, #3600
	mov	r2, r4
	mov	r1, #0
	add	r0, r0, #4
	bl	memset
.L9:
	add	r0, sp, #3600
	mov	r3, r8
	mov	r2, r4
	mov	r1, #1
	add	r0, r0, #4
	bl	fread
	cmp	r0, #0
	beq	.L21
	mov	r2, #1024
	ldr	r1, [sp, #12]
	mov	r0, r5
	bl	memcpy
	mov	r2, #516
	mov	r1, r7
	mov	r0, r9
	bl	memcpy
	add	r3, sp, #272
	mov	r0, r3
	mov	r2, r4
	mov	r1, #0
	str	r3, [sp, #8]
	bl	memset
	add	r3, sp, #528
	mov	r0, r3
	mov	r2, #512
	mov	r1, #0
	str	r3, [sp, #4]
	bl	memset
	add	r3, sp, #16
	add	r1, sp, #3600
	mov	r0, r3
	mov	r2, r4
	add	r1, r1, #4
	bl	memcpy
	ldr	r2, [sp, #4]
	mov	r1, r5
	bl	mm_conv
	mov	r3, r5
	ldr	r0, [sp, #4]
	ldr	r2, [sp, #1552]
	add	r1, sp, #1296
	bl	mm_exp
	ldr	r2, [sp, #8]
	mov	r1, r5
	ldr	r0, [sp, #4]
	bl	mm_redc
	add	r0, sp, #3856
	mov	r2, r4
	ldr	r1, [sp, #8]
	add	r0, r0, #4
	bl	memcpy
	cmp	r6, #0
	beq	.L10
	add	r0, sp, #3856
	mov	r3, r6
	mov	r2, r4
	mov	r1, #1
	add	r0, r0, #4
	bl	fwrite
.L10:
	cmp	r10, #0
	beq	.L11
	add	r3, sp, #3856
	add	r3, r3, #4
	mov	r2, r4
	mov	r1, fp
	mov	r0, #1
	bl	__printf_chk
	b	.L11
.L21:
	add	r3, sp, #4096
	add	r3, r3, #20
	ldr	r2, [r3]
	ldr	r3, .L23
	ldr	r3, [r3]
	cmp	r2, r3
	bne	.L22
	add	sp, sp, #4096
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	add	sp, sp, #8
	bx	lr
.L22:
	bl	__stack_chk_fail
.L24:
	.align	2
.L23:
	.word	__stack_chk_guard
	.word	.LC0
	.size	rsa_proc, .-rsa_proc
	.align	2
	.global	getkey
	.syntax unified
	.arm
	.type	getkey, %function
getkey:
	@ args = 0, pretend = 0, frame = 264
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, #0
	sub	sp, sp, #268
	mov	r9, r0
	mov	r8, sp
	mov	r5, r1
	ldr	r3, .L54
	mov	r1, r7
	ldr	r3, [r3]
	mov	r2, #256
	add	r0, sp, #4
	str	r3, [sp, #260]
	strh	r7, [r8], #1	@ movhi
	strb	r7, [sp, #2]
	bl	memset
	mov	r1, r7
	mov	r2, #516
	mov	r0, r9
	bl	memset
	mov	r3, r8
	mov	r2, sp
	mov	r0, r5
	ldr	r1, .L54+4
	bl	__isoc99_fscanf
	ldrb	r3, [sp]	@ zero_extendqisi2
	cmp	r3, #48
	bne	.L26
	ldrb	r3, [sp, #1]	@ zero_extendqisi2
	cmp	r3, #48
	bne	.L26
	add	r6, sp, #260
	mov	r4, #255
	ldr	r10, .L54+4
	mov	fp, #16
	b	.L27
.L49:
	ldrb	r3, [sp, #1]	@ zero_extendqisi2
	cmp	r3, #33
	beq	.L28
	mov	r2, fp
	mov	r1, r7
	mov	r0, sp
	bl	strtoul
	sub	r4, r4, #1
	strb	r0, [r6, #-1]!
.L27:
	mov	r3, r8
	mov	r2, sp
	mov	r1, r10
	mov	r0, r5
	bl	__isoc99_fscanf
	cmp	r0, #0
	cmnne	r4, #1
	beq	.L28
	ldrb	r3, [sp]	@ zero_extendqisi2
	cmp	r3, #33
	bne	.L49
.L28:
	mov	r2, #256
	add	r1, sp, #4
	mov	r0, r9
	bl	memcpy
	mov	r2, #256
	mov	r1, #0
	add	r0, sp, #4
	bl	memset
	add	r3, sp, #1
	mov	r2, sp
	ldr	r1, .L54+4
	mov	r0, r5
	bl	__isoc99_fscanf
	ldrb	r3, [sp]	@ zero_extendqisi2
	cmp	r3, #48
	beq	.L50
.L30:
	mov	r2, #16
	mov	r1, #0
	mov	r0, sp
	bl	strtoul
	mov	r4, #254
	strb	r0, [sp, #259]
.L31:
	add	r6, r4, #1
	add	r3, sp, #4
	add	r6, r3, r6
	add	r8, sp, #1
	ldr	r7, .L54+4
	mov	fp, #16
	mov	r10, #0
	b	.L32
.L33:
	mov	r2, fp
	mov	r1, r10
	mov	r0, sp
	bl	strtoul
	sub	r4, r4, #1
	strb	r0, [r6, #-1]!
.L32:
	mov	r3, r8
	mov	r2, sp
	mov	r1, r7
	mov	r0, r5
	bl	__isoc99_fscanf
	mvn	r3, r4
	cmp	r0, #0
	lsr	r3, r3, #31
	moveq	r3, #0
	cmp	r3, #0
	bne	.L33
	mov	r2, #256
	add	r1, sp, #4
	add	r0, r9, r2
	bl	memcpy
	ldr	r1, [sp, #256]
	cmp	r1, #0
	movne	r3, #31
	bne	.L37
	b	.L51
.L52:
	subs	r3, r3, #1
	bcc	.L48
.L37:
	lsr	r2, r1, r3
	tst	r2, #1
	beq	.L52
.L48:
	ldr	r2, .L54
	ldr	r0, [sp, #260]
	ldr	r2, [r2]
	add	r3, r3, #2016
	cmp	r0, r2
	str	r1, [r9, #508]
	str	r3, [r9, #512]
	mov	r0, r9
	bne	.L53
	add	sp, sp, #268
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L50:
	ldrb	r3, [sp, #1]	@ zero_extendqisi2
	cmp	r3, #48
	moveq	r4, #255
	bne	.L30
	b	.L31
.L26:
	mov	r2, sp
	ldr	r1, .L54+8
	mov	r0, #1
	bl	__printf_chk
	mov	r0, #1
	bl	exit
.L53:
	bl	__stack_chk_fail
.L51:
	ldr	r0, .L54+12
	bl	puts
	mov	r0, #1
	bl	exit
.L55:
	.align	2
.L54:
	.word	__stack_chk_guard
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.size	getkey, .-getkey
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 1056
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, #0
	mov	r7, r0
	mov	r5, r1
	mov	fp, r4
	mov	r10, #1
	ldr	r8, .L81
	sub	sp, sp, #1568
	ldr	r3, [r8]
	sub	sp, sp, #4
	ldr	r6, .L81+4
	ldr	r9, .L81+8
	str	r3, [sp, #1564]
	str	r4, [sp, #516]
	str	r4, [sp, #520]
	str	r4, [sp, #524]
.L57:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	bl	getopt
	cmn	r0, #1
	beq	.L76
	sub	r0, r0, #63
	cmp	r0, #49
	ldrls	pc, [pc, r0, asl #2]
	b	.L72
.L60:
	.word	.L59
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L61
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L62
	.word	.L72
	.word	.L63
	.word	.L72
	.word	.L72
	.word	.L72
	.word	.L64
	.word	.L73
.L73:
	str	r10, [sp, #524]
	b	.L57
.L64:
	ldr	r4, [r9]
	b	.L57
.L63:
	ldr	fp, [r9]
	b	.L57
.L62:
	ldr	r3, [r9]
	str	r3, [sp, #516]
	b	.L57
.L61:
	str	r10, [sp, #520]
	b	.L57
.L59:
	ldr	r2, [r5]
	ldr	r3, .L81+12
	ldr	r1, .L81+16
	mov	r0, #1
	bl	__printf_chk
	mov	r0, #1
.L58:
	ldr	r2, [sp, #1564]
	ldr	r3, [r8]
	cmp	r2, r3
	bne	.L77
	add	sp, sp, #1568
	add	sp, sp, #4
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L72:
	mov	r0, #1
	b	.L58
.L76:
	cmp	fp, #0
	beq	.L78
	ldr	r3, [sp, #516]
	cmp	r3, #0
	beq	.L79
	ldr	r1, .L81+20
	mov	r0, fp
	bl	fopen
	ldr	r1, .L81+20
	mov	r6, r0
	ldr	r0, [sp, #516]
	bl	fopen
	cmp	r4, #0
	mov	r5, r0
	beq	.L69
	mov	r0, r4
	ldr	r1, .L81+24
	bl	fopen
	mov	r4, r0
.L69:
	mov	r1, r6
	add	r0, sp, #532
	bl	getkey
	add	r0, sp, #1040
	mov	r2, #516
	mov	r1, #0
	add	r0, r0, #8
	bl	memset
	add	r0, sp, #1040
	add	r1, sp, #532
	mov	r2, #256
	add	r0, r0, #8
	bl	memcpy
	mov	r2, #16
	ldr	r1, [sp, #520]
	ldr	r3, .L81+28
	cmp	r1, #0
	str	r2, [sp, #1560]
	str	r3, [sp, #1304]
	bne	.L80
	add	r1, sp, #536
	mov	r2, #512
	mov	r0, sp
	bl	memcpy
	mov	r0, r5
	ldr	r2, [sp, #524]
	mov	r1, r4
	ldr	r3, [sp, #532]
	bl	rsa_proc
	ldr	r0, [sp, #520]
	b	.L58
.L80:
	add	r1, sp, #1040
	mov	r2, #512
	add	r1, r1, #12
	mov	r0, sp
	bl	memcpy
	mov	r0, r5
	ldr	r2, [sp, #524]
	mov	r1, r4
	ldr	r3, [sp, #1048]
	bl	rsa_proc
	mov	r0, #0
	b	.L58
.L79:
	ldr	r2, [r5]
	ldr	r3, .L81+12
	ldr	r1, .L81+32
	mov	r0, #1
	bl	__printf_chk
	mov	r0, #1
	b	.L58
.L78:
	ldr	r2, [r5]
	ldr	r3, .L81+12
	ldr	r1, .L81+36
	mov	r0, #1
	bl	__printf_chk
	mov	r0, #1
	b	.L58
.L77:
	bl	__stack_chk_fail
.L82:
	.align	2
.L81:
	.word	__stack_chk_guard
	.word	.LC6
	.word	optarg
	.word	.LC4
	.word	.LC5
	.word	.LC9
	.word	.LC10
	.word	65537
	.word	.LC8
	.word	.LC7
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%.*s\000"
	.space	3
.LC1:
	.ascii	"%c%c\000"
	.space	3
.LC2:
	.ascii	"fatal: modulus does not start with 00. but %s\012\000"
	.space	1
.LC3:
	.ascii	"fatal: private exponent too small for fixed public "
	.ascii	"exponent (F4)\000"
	.space	3
.LC4:
	.ascii	"[-dp] -k <keyfile> -i <input file> [-o <output file"
	.ascii	">]\012    options:\012    -d  decrypt using the Ope"
	.ascii	"nSSL standard public exponent F4 (65537). otherwise"
	.ascii	", [default] encrypt using the private \012        e"
	.ascii	"xponent in <keyfile>.\012    -p  [optional] print t"
	.ascii	"he result of the encryption/decryption to stdout.\012"
	.ascii	"    -k  the (easy-format) keyfile. this can be gene"
	.ascii	"rated by extracting the modulus and private exponen"
	.ascii	"t from a PEM\012        file with `openssl -in <pem"
	.ascii	"file> -noout -text` and some awk parsing.\012    -i"
	.ascii	"  the input file.\012    -o  [optional] the output "
	.ascii	"file.\012\000"
	.space	2
.LC5:
	.ascii	"usage: %s %s\000"
	.space	3
.LC6:
	.ascii	"k:i:o:pd\000"
	.space	3
.LC7:
	.ascii	"fatal: no keyfile specified\012usage: %s %s\000"
	.space	3
.LC8:
	.ascii	"fatal: no textfile specified\012usage: %s %s\000"
	.space	2
.LC9:
	.ascii	"r\000"
	.space	2
.LC10:
	.ascii	"w\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
