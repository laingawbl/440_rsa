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
	.file	"monty.c"
	.text
	.align	2
	.global	list
	.syntax unified
	.arm
	.type	list, %function
list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r1
	mov	r6, r0
	ldr	r2, .L9
	ldr	r1, .L9+4
	mov	r0, #1
	bl	__printf_chk
	cmp	r4, #0
	ble	.L5
	mov	r7, #1
	ldr	r9, .L9+8
	ldr	r8, .L9+12
	add	r6, r6, r4, lsl #2
	lsl	r5, r4, #5
	b	.L4
.L3:
	ldr	r2, [r6, #-4]!
	mov	r1, r8
	mov	r0, r7
	bl	__printf_chk
	subs	r4, r4, #1
	sub	r5, r5, #32
	beq	.L5
.L4:
	tst	r4, #7
	bne	.L3
	mov	r2, r5
	mov	r1, r9
	mov	r0, r7
	bl	__printf_chk
	b	.L3
.L5:
	mov	r0, #10
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	putchar
.L10:
	.align	2
.L9:
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.size	list, .-list
	.align	2
	.global	declaim
	.syntax unified
	.arm
	.type	declaim, %function
declaim:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r2
	mov	r6, r1
	mov	r2, r0
	ldr	r1, .L18
	mov	r0, #1
	bl	__printf_chk
	cmp	r4, #0
	ble	.L15
	mov	r7, #1
	ldr	r9, .L18+4
	ldr	r8, .L18+8
	add	r6, r6, r4, lsl #2
	lsl	r5, r4, #5
	b	.L14
.L13:
	ldr	r2, [r6, #-4]!
	mov	r1, r8
	mov	r0, r7
	bl	__printf_chk
	subs	r4, r4, #1
	sub	r5, r5, #32
	beq	.L15
.L14:
	tst	r4, #7
	bne	.L13
	mov	r2, r5
	mov	r1, r9
	mov	r0, r7
	bl	__printf_chk
	b	.L13
.L15:
	mov	r0, #10
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	putchar
.L19:
	.align	2
.L18:
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.size	declaim, .-declaim
	.align	2
	.global	gte
	.syntax unified
	.arm
	.type	gte, %function
gte:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r1, r1, #256
	add	r3, r0, #256
.L22:
	ldr	ip, [r3, #-4]!
	ldr	r2, [r1, #-4]!
	cmp	ip, r2
	beq	.L25
	movhi	r0, #1
	movls	r0, #0
	bx	lr
.L25:
	cmp	r0, r3
	bne	.L22
	mov	r0, #1
	bx	lr
	.size	gte, .-gte
	.align	2
	.global	mult
	.syntax unified
	.arm
	.type	mult, %function
mult:
	@ args = 0, pretend = 0, frame = 520
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	mov	r5, r1
	mov	r6, #0
	sub	sp, sp, #524
	mov	r10, r2
	add	r0, sp, #4
	mov	r2, #512
	mov	r1, #0
	bl	memset
	sub	r4, r4, #4
	sub	r5, r5, #4
	add	ip, sp, #260
	add	r7, sp, #516
.L27:
	mov	r2, r5
	mov	r9, r6
	ldr	lr, [r4, #4]!
	sub	r3, ip, #256
.L28:
	mov	r0, r9
	eor	r1, r1
	ldr	r9, [r2, #4]!
	ldr	fp, [r3]
	umlal	r0, r1, lr, r9
	adds	r0, r0, fp
	str	r0, [r3], #4
	adc	r1, r1, #0
	cmp	ip, r3
	bne	.L28
	cmp	ip, r7
	bne	.L27
	mov	r9, r1
	mov	r2, #512
	add	r1, sp, #4
	mov	r0, r10
	bl	memcpy
	add	sp, sp, #524
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L35:
	.align	2
.L34:
	.word	__stack_chk_guard
	.size	mult, .-mult
	.align	2
	.global	add
	.syntax unified
	.arm
	.type	add, %function
add:
	@ args = 0, pretend = 0, frame = 264
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	mov	r4, #0
	ldr	r5, .L42
	sub	sp, sp, #268
	ldr	r3, [r5]
	mov	r7, r1
	mov	r8, r0
	mov	r6, r2
	add	r0, sp, #4
	mov	r2, #256
	mov	r1, r4
	str	r3, [sp, #260]
	bl	memset
	mov	r2, sp
	sub	ip, r7, #4
	sub	lr, r8, #4
	add	r7, sp, #256
.L37:
	ldr	r3, [lr, #4]!
	mov	r1, #0
	mov	r9, #0
	mov	r0, r3
	ldr	r8, [ip, #4]!
	cmp	r9, r1
	add	r3, r3, r8
	add	r3, r3, r4
	cmpeq	r3, r0
	movne	r4, #0
	andeq	r4, r4, #1
	str	r3, [r2, #4]!
	orrcc	r4, r4, #1
	cmp	r7, r2
	bne	.L37
	mov	r2, #256
	add	r1, sp, #4
	mov	r0, r6
	bl	memcpy
	ldr	r2, [sp, #260]
	ldr	r3, [r5]
	mov	r0, r4
	cmp	r2, r3
	bne	.L41
	add	sp, sp, #268
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L41:
	bl	__stack_chk_fail
.L43:
	.align	2
.L42:
	.word	__stack_chk_guard
	.size	add, .-add
	.align	2
	.global	sub
	.syntax unified
	.arm
	.type	sub, %function
sub:
	@ args = 0, pretend = 0, frame = 264
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r4, #0
	ldr	r5, .L50
	sub	sp, sp, #264
	ldr	r3, [r5]
	mov	r8, r0
	mov	r7, r1
	mov	r6, r2
	add	r0, sp, #4
	mov	r2, #256
	mov	r1, r4
	str	r3, [sp, #260]
	bl	memset
	mov	r2, sp
	sub	ip, r8, #4
	sub	r0, r7, #4
	add	lr, sp, #256
.L45:
	ldr	r1, [ip, #4]!
	ldr	r3, [r0, #4]!
	sub	r3, r1, r3
	sub	r3, r3, r4
	cmp	r1, r3
	movne	r4, #0
	andeq	r4, r4, #1
	str	r3, [r2, #4]!
	orrcc	r4, r4, #1
	cmp	lr, r2
	bne	.L45
	mov	r2, #256
	add	r1, sp, #4
	mov	r0, r6
	bl	memcpy
	ldr	r2, [sp, #260]
	ldr	r3, [r5]
	mov	r0, r4
	cmp	r2, r3
	bne	.L49
	add	sp, sp, #264
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L49:
	bl	__stack_chk_fail
.L51:
	.align	2
.L50:
	.word	__stack_chk_guard
	.size	sub, .-sub
	.align	2
	.global	mm_init
	.syntax unified
	.arm
	.type	mm_init, %function
mm_init:
	@ args = 0, pretend = 0, frame = 1040
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	ldr	r3, .L77
	sub	sp, sp, #1040
	sub	sp, sp, #4
	ldr	r3, [r3]
	str	r1, [sp, #4]
	mov	r2, #256
	mov	r1, #0
	mov	r9, r0
	add	r0, sp, #8
	str	r3, [sp, #1036]
	bl	memset
	mov	r2, #256
	add	r0, sp, #264
	mov	r1, #0
	bl	memset
	mov	r2, #1
	mov	r3, r2
	ands	r3, r3, #1
	mov	r8, #2048
	str	r2, [sp, #8]
	add	r6, sp, #4
	add	r5, sp, #776
	add	r7, sp, #260
	mov	r4, #0
	bne	.L74
.L53:
	mov	r10, r3
	add	r0, sp, #516
	add	r1, sp, #260
.L55:
	ldr	lr, [r1]
	ldr	ip, [r0]
	lsr	r2, lr, #1
	orr	r2, r2, r4, lsl #31
	str	r2, [r1], #-4
	lsr	r3, ip, #1
	orr	r3, r3, r10, lsl #31
	cmp	r1, r6
	and	r4, lr, #1
	str	r3, [r0], #-4
	and	r10, ip, #1
	bne	.L55
	subs	r8, r8, #1
	beq	.L56
	ldr	r3, [sp, #8]
	mov	r4, #0
	ands	r3, r3, #1
	beq	.L53
.L74:
	mov	r2, #256
	mov	r1, r4
	mov	r0, r5
	sub	fp, r9, #4
	add	r10, sp, #772
	bl	memset
	mov	r2, r10
	mov	ip, fp
	add	lr, sp, #4
.L54:
	ldr	r3, [lr, #4]!
	mov	r1, #0
	mov	fp, #0
	mov	r0, r3
	ldr	r10, [ip, #4]!
	cmp	fp, r1
	add	r3, r3, r10
	add	r3, r3, r4
	cmpeq	r3, r0
	movne	r4, #0
	andeq	r4, r4, #1
	orrcc	r4, r4, #1
	cmp	r7, lr
	str	r3, [r2, #4]!
	bne	.L54
	mov	r2, #256
	mov	r1, r5
	add	r0, sp, #8
	bl	memcpy
	mov	r3, #1
	b	.L53
.L56:
	ldr	r4, [sp, #4]
	mov	r2, #256
	mov	r1, r9
	mov	r0, r4
	bl	memcpy
	mov	r2, #256
	add	r1, sp, #264
	add	r0, r4, r2
	bl	memcpy
	add	r1, sp, #8
	mov	r2, #256
	add	r0, r4, #512
	bl	memcpy
	mov	r2, #260
	mov	r1, r8
	mov	r0, r5
	bl	memset
	mov	r3, #1
	mov	r4, #4096
	mov	r6, #256
	add	r7, sp, #1024
	str	r3, [sp, #776]
	add	r7, r7, #4
.L58:
	add	r0, sp, #1024
	add	r0, r0, #8
	mov	r3, r0
	ldr	r1, [sp, #1032]
.L59:
	lsl	r2, r1, #1
	ldr	r1, [r3, #-4]!
	orr	r2, r2, r1, lsr #31
	cmp	r3, r5
	str	r2, [r3, #4]
	bne	.L59
	ldr	r3, [sp, #776]
	ldr	r2, [sp, #1032]
	lsl	r3, r3, #1
	cmp	r2, #0
	str	r3, [sp, #776]
	bne	.L60
	add	r3, r9, #256
.L62:
	ldr	r1, [r0, #-4]!
	ldr	r2, [r3, #-4]!
	cmp	r1, r2
	beq	.L75
	bls	.L63
.L60:
	mov	r2, r6
	mov	r1, r8
	add	r0, sp, #520
	bl	memset
	mov	r2, r8
	add	ip, sp, #516
	add	r1, sp, #772
	sub	r0, r9, #4
.L64:
	ldr	lr, [r1, #4]!
	ldr	r3, [r0, #4]!
	sub	r3, lr, r3
	sub	r3, r3, r2
	cmp	lr, r3
	movne	r2, #0
	andeq	r2, r2, #1
	orrcc	r2, r2, #1
	cmp	r1, r7
	str	r3, [ip, #4]!
	bne	.L64
	mov	r2, r6
	add	r1, sp, #520
	mov	r0, r5
	bl	memcpy
	str	r8, [sp, #1032]
.L63:
	subs	r4, r4, #1
	bne	.L58
	ldr	r3, [sp, #4]
	mov	r2, #256
	add	r0, r3, #768
	mov	r1, r5
	bl	memcpy
	ldr	r3, .L77
	ldr	r2, [sp, #1036]
	ldr	r3, [r3]
	cmp	r2, r3
	bne	.L76
	add	sp, sp, #1040
	add	sp, sp, #4
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L75:
	cmp	r3, r9
	bne	.L62
	b	.L60
.L76:
	bl	__stack_chk_fail
.L78:
	.align	2
.L77:
	.word	__stack_chk_guard
	.size	mm_init, .-mm_init
	.align	2
	.global	mm_redc
	.syntax unified
	.arm
	.type	mm_redc, %function
mm_redc:
	@ args = 0, pretend = 0, frame = 1824
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	mov	r4, r1
	ldr	r3, .L103
	sub	sp, sp, #1824
	sub	sp, sp, #4
	ldr	r3, [r3]
	str	r0, [sp, #16]
	str	r1, [sp, #20]
	mov	fp, r2
	mov	r1, #0
	mov	r2, #512
	add	r0, sp, #796
	str	r3, [sp, #1820]
	bl	memset
	add	r3, sp, #28
	mov	r0, r3
	mov	r1, r5
	mov	r2, #256
	str	r3, [sp, #12]
	add	r7, sp, #540
	bl	memcpy
	mov	r1, r4
	mov	r2, #256
	add	r0, sp, #284
	bl	memcpy
	add	r5, sp, #796
	add	r1, r4, #256
	mov	r0, r7
	mov	r2, #256
	bl	memcpy
	mov	r4, #0
	mov	r6, r5
	mov	r8, #64
	mov	r9, r5
.L80:
	add	r3, sp, #796
	add	ip, r4, r8
	add	ip, r3, ip, lsl #2
	ldr	r3, [sp, #12]
	mov	r0, #0
	sub	r2, r3, #4
	mov	r5, r4
	mov	r3, r6
	ldr	lr, [r7, r4, lsl #2]
	str	r9, [sp]
.L81:
	mov	r10, #0
	mov	r1, r10
	ldr	r4, [r2, #4]!
	umlal	r0, r1, lr, r4
	ldr	r4, [r3]
	adds	r9, r0, r4
	str	r9, [r3], #4
	adc	r10, r1, #0
	cmp	ip, r3
	mov	r0, r10
	bne	.L81
	subs	r8, r8, #1
	ldr	r9, [sp]
	add	r4, r5, #1
	add	r6, r6, #4
	bne	.L80
	add	r0, sp, #1296
	add	r0, r0, #12
	mov	r2, #512
	mov	r1, r8
	bl	memset
	add	r3, sp, #800
	add	r4, r3, #248
	ldr	r3, [sp, #20]
	add	ip, sp, #1552
	sub	r7, r3, #4
	add	r6, sp, #792
	add	ip, ip, #12
.L83:
	mov	r2, r7
	mov	r5, r8
	ldr	lr, [r6, #4]!
	sub	r3, ip, #256
	str	r4, [sp, #12]
.L84:
	mov	r0, r5
	mov	r1, #0
	ldr	r5, [r2, #4]!
	ldr	r10, [r3]
	umlal	r0, r1, lr, r5
	adds	r4, r0, r10
	str	r4, [r3], #4
	adc	r5, r1, #0
	cmp	r3, ip
	stm	sp, {r4-r5}
	bne	.L84
	mov	ip, r3
	ldmia	sp, {r0-r1}
	ldr	r4, [sp, #12]
	str	r1, [ip], #4
	cmp	r6, r4
	bne	.L83
	add	r1, sp, #1296
	add	r0, sp, #796
	mov	r2, #512
	add	r1, r1, #12
	bl	memcpy
	mov	r5, r9
	mov	ip, #0
	ldr	r3, [sp, #16]
	sub	r6, r3, #4
	add	r3, sp, #800
	add	r7, r3, #252
.L86:
	mov	r9, #0
	mov	r1, #0
	ldr	r0, [r6, #4]!
	ldr	r2, [r5]
	add	r3, r0, ip
	cmp	r9, r1
	add	r3, r3, r2
	cmpeq	r3, r0
	movne	ip, #0
	andeq	ip, ip, #1
	str	r3, [r5], #4
	orrcc	ip, ip, #1
	cmp	r7, r5
	bne	.L86
	sub	r5, fp, #4
	mov	r7, r5
	mov	r2, fp
	ldr	r3, [sp, #16]
	add	r6, r3, #252
	add	r3, sp, #800
	add	r8, r3, #504
.L87:
	ldr	r3, [r6, #4]!
	mov	r1, #0
	mov	fp, #0
	mov	r0, r3
	ldr	lr, [r4, #4]!
	cmp	fp, r1
	add	r3, r3, lr
	add	r3, r3, ip
	cmpeq	r3, r0
	movne	ip, #0
	andeq	ip, ip, #1
	orrcc	ip, ip, #1
	cmp	r4, r8
	str	r3, [r7, #4]!
	bne	.L87
	cmp	ip, #0
	mov	fp, r2
	bne	.L88
	add	r3, r2, #256
	add	r2, sp, #540
.L90:
	ldr	r0, [r3, #-4]!
	ldr	r1, [r2, #-4]!
	cmp	r0, r1
	beq	.L101
	bls	.L79
.L88:
	add	r0, sp, #1296
	mov	r2, #256
	mov	r1, #0
	add	r0, r0, #12
	bl	memset
	mov	r2, #0
	add	ip, sp, #1296
	add	r1, sp, #280
	add	lr, sp, #536
	add	ip, ip, #8
.L92:
	ldr	r0, [r5, #4]!
	ldr	r3, [r1, #4]!
	sub	r3, r0, r3
	sub	r3, r3, r2
	cmp	r0, r3
	movne	r2, #0
	andeq	r2, r2, #1
	orrcc	r2, r2, #1
	cmp	lr, r1
	str	r3, [ip, #4]!
	bne	.L92
	add	r1, sp, #1296
	add	r1, r1, #12
	mov	r0, fp
	mov	r2, #256
	bl	memcpy
.L79:
	ldr	r3, .L103
	ldr	r2, [sp, #1820]
	ldr	r3, [r3]
	cmp	r2, r3
	bne	.L102
	add	sp, sp, #1824
	add	sp, sp, #4
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L101:
	cmp	r3, fp
	bne	.L90
	b	.L88
.L102:
	bl	__stack_chk_fail
.L104:
	.align	2
.L103:
	.word	__stack_chk_guard
	.size	mm_redc, .-mm_redc
	.align	2
	.global	mm_mult
	.syntax unified
	.arm
	.type	mm_mult, %function
mm_mult:
	@ args = 0, pretend = 0, frame = 1036
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	sub	r5, r1, #4
	mov	r9, r4
	sub	sp, sp, #1040
	mov	fp, r2
	
	add	r0, sp, #524
	mov	r2, #512
	mov	r1, #0
	bl	memset
	
	add	r3, sp, #1024
	add	r3, r3, #12
	sub	r4, r4, #4
	add	ip, sp, #780
	str	r3, [sp, #4]
	
.L106:								@ hand-modified
	ldr	lr, [r4, #4]!
	mov	r2, r5
	sub	r3, ip, #256
	eor	r0, r0
	mov	r8, r4
	
.L107:
	ldr	r4, [r2, #4]
	eor	r1, r1
	umlal	r0, r1, lr, r4
	ldr	r7, [r3]
	adds  r7, r0, r7
	str	r7, [r3], #4
	adc	r0, r1, #0
	
	ldr	r4, [r2, #8]
	eor	r1, r1
	umlal	r0, r1, lr, r4
	ldr	r7, [r3]
	adds  r7, r0, r7
	str	r7, [r3], #4
	adc	r0, r1, #0
	
	ldr	r4, [r2, #12]
	eor	r1, r1
	umlal	r0, r1, lr, r4
	ldr	r7, [r3]
	adds  r7, r0, r7
	str	r7, [r3], #4
	adc	r0, r1, #0
	
	ldr	r4, [r2, #16]!
	eor	r1, r1
	umlal	r0, r1, lr, r4
	ldr	r7, [r3]
	adds  r7, r0, r7
	str	r7, [r3], #4
	adc	r0, r1, #0
	
	cmp	ip, r3
	bne	.L107
	
	ldr	r3, [sp, #4]
	str	r0, [ip], #4
	cmp	ip, r3
	mov	r4, r8
	bne	.L106
	
	add	r3, sp, #12
	mov	r0, r3
	add	r1, sp, #524
	mov	r2, #512
	str	r3, [sp, #4]
	bl	memcpy
	
	mov	r2, r9
	mov	r1, fp
	ldr	r0, [sp, #4]
	bl	mm_redc

	add	sp, sp, #1040
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L114:
	.align	2
.L113:
	.word	__stack_chk_guard
	.size	mm_mult, .-mm_mult
	.align	2
	.global	mm_conv
	.syntax unified
	.arm
	.type	mm_conv, %function
mm_conv:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r2
	push	{r4, lr}
	mov	r4, r1
	mov	r2, #256
	mov	r1, r0
	mov	r0, r3
	bl	memcpy
	mov	r2, r4
	add	r1, r4, #768
	pop	{r4, lr}
	b	mm_mult
	.size	mm_conv, .-mm_conv
	.align	2
	.global	mm_exp
	.syntax unified
	.arm
	.type	mm_exp, %function
mm_exp:
	@ args = 0, pretend = 0, frame = 1600
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, #1
	mov	fp, r3
	mov	r7, #0
	ldr	r3, .L561
	sub	sp, sp, #1600
	sub	sp, sp, #4
	ldr	r3, [r3]
	str	r0, [sp, #36]
	str	r1, [sp, #40]
	str	r2, [sp, #44]
	mov	r1, #0
	mov	r2, #256
	add	r0, sp, #60
	str	r3, [sp, #1596]
	bl	memset
	add	r0, sp, #1072
	add	r0, r0, #12
	mov	r2, #512
	mov	r1, #0
	str	r4, [sp, #60]
	bl	memset
	add	lr, sp, #1328
	mov	ip, r4
	add	r8, sp, #312
	add	r4, sp, #60
	add	r6, fp, #764
	add	lr, lr, #12
.L118:
	mov	r2, r6
	mov	r5, r7
	sub	r3, lr, #256
	str	r4, [sp]
.L119:
	mov	r0, r5
	mov	r1, #0
	ldr	r5, [r2, #4]!
	ldr	r9, [r3]
	umlal	r0, r1, ip, r5
	adds	r4, r0, r9
	str	r4, [r3], #4
	adc	r5, r1, #0
	cmp	r3, lr
	bne	.L119
	ldr	r4, [sp]
	str	r5, [lr], #4
	cmp	r4, r8
	beq	.L120
	ldr	ip, [r4, #4]!
	b	.L118
.L562:
	.align	2
.L561:
	.word	__stack_chk_guard
.L120:
	add	r3, sp, #572
	add	r1, sp, #1072
	mov	r0, r3
	mov	r2, #512
	add	r1, r1, #12
	str	r3, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	mov	r2, #256
	add	r0, sp, #316
	ldr	r1, [sp, #36]
	bl	memcpy
	ldr	r2, [sp, #44]
	cmp	r2, #0
	add	r3, r2, #31
	movge	r3, r2
	asr	r3, r3, #5
	cmp	r3, #0
	ble	.L358
	mov	r10, #512
	mov	r5, #0
	ldr	r2, [sp, #40]
	lsl	r3, r3, #2
	str	r3, [sp, #52]
	add	r3, r2, r3
	str	r3, [sp, #48]
	add	r3, sp, #1328
	add	r3, r3, #8
	str	r2, [sp, #32]
	str	r3, [sp, #16]
	add	r4, sp, #568
	add	r6, sp, #312
.L346:
	ldr	r3, [sp, #32]
	ldr	r2, [r3], #4
	tst	r2, #1
	str	r2, [sp, #20]
	str	r3, [sp, #32]
	bne	.L527
.L122:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	lr, r6
	ldr	r7, [sp, #16]
.L126:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r9, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L127:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L127
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r9, r4
	mov	lr, r9
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L126
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	tst	r3, #2
	bne	.L528
.L129:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L133:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L134:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L134
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L133
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #4
	bne	.L529
.L136:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L140:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L141:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L141
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L140
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #8
	bne	.L530
.L143:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L147:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L148:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L148
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L147
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #16
	bne	.L531
.L150:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L154:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L155:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L155
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L154
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #32
	bne	.L532
.L157:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L161:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L162:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L162
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L161
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #64
	bne	.L533
.L164:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L168:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L169:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L169
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L168
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #128
	bne	.L534
.L171:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L175:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L176:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L176
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L175
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #256
	bne	.L535
.L178:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L182:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L183:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L183
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L182
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #512
	bne	.L536
.L185:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L189:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L190:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L190
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L189
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #1024
	bne	.L537
.L192:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L196:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L197:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L197
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L196
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #2048
	bne	.L538
.L199:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L203:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L204:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L204
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L203
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #4096
	bne	.L539
.L206:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L210:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L211:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L211
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L210
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #8192
	bne	.L540
.L213:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L217:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L218:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L218
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L217
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #16384
	bne	.L541
.L220:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L224:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r1, lr
	mov	r8, r7
	mov	lr, r5
	sub	r2, r7, #252
	str	r6, [sp, #24]
	str	r1, [sp]
.L225:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L225
	mov	r1, r7
	mov	r7, r8
	ldr	lr, [sp]
	ldr	r6, [sp, #24]
	cmp	lr, r4
	str	r1, [r7, #4]!
	bne	.L224
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #32768
	bne	.L542
.L227:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L231:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L232:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L232
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L231
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #65536
	bne	.L543
.L234:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L238:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L239:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L239
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L238
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #131072
	bne	.L544
.L241:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L245:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L246:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L246
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L245
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #262144
	bne	.L545
.L248:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L252:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L253:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L253
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L252
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #524288
	bne	.L546
.L255:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L259:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L260:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L260
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L259
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #1048576
	bne	.L547
.L262:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L266:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L267:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L267
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L266
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #2097152
	bne	.L548
.L269:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L273:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L274:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L274
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L273
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #4194304
	bne	.L549
.L276:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L280:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L281:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L281
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L280
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #8388608
	bne	.L550
.L283:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L287:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L288:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L288
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L287
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #16777216
	bne	.L551
.L290:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L294:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L295:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L295
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L294
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #33554432
	bne	.L552
.L297:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L301:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L302:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L302
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L301
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #67108864
	bne	.L553
.L304:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L308:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L309:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L309
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L308
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #134217728
	bne	.L554
.L311:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L315:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L316:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L316
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L315
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #268435456
	bne	.L555
.L318:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L322:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L323:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L323
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L322
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #536870912
	bne	.L556
.L325:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L329:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #24]
.L330:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L330
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #24]
	str	r1, [r7, #4]!
	bne	.L329
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	tst	r3, #1073741824
	bne	.L557
.L332:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L336:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp, #24]
	str	r6, [sp, #28]
.L337:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	stm	sp, {r6-r7}
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L337
	mov	r1, r7
	ldr	r7, [sp, #24]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #28]
	str	r1, [r7, #4]!
	bne	.L336
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #20]
	mov	r8, r9
	cmp	r3, #0
	blt	.L558
.L339:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L343:
	ldr	ip, [lr, #4]!
	mov	r3, r6
	mov	r8, lr
	mov	lr, r5
	sub	r2, r7, #252
	str	r7, [sp]
	str	r6, [sp, #20]
.L344:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L344
	mov	r1, r7
	ldr	r7, [sp]
	cmp	r8, r4
	mov	lr, r8
	ldr	r6, [sp, #20]
	str	r1, [r7, #4]!
	bne	.L343
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #48]
	ldr	r2, [sp, #32]
	mov	r8, r9
	cmp	r3, r2
	bne	.L346
	ldr	r2, [sp, #52]
.L121:
	ldr	r3, [sp, #44]
	asr	r1, r3, #31
	lsr	r1, r1, #27
	add	r3, r3, r1
	and	r3, r3, #31
	subs	r3, r3, r1
	str	r3, [sp, #24]
	ldr	r3, [sp, #40]
	ldr	r3, [r3, r2]
	str	r3, [sp, #20]
	bmi	.L356
	mov	r10, #0
	add	r3, sp, #1328
	add	r3, r3, #8
	add	r4, sp, #568
	str	r3, [sp, #16]
	add	r6, sp, #312
.L355:
	ldr	r3, [sp, #20]
	lsr	r3, r3, r10
	tst	r3, #1
	bne	.L559
.L348:
	add	r0, sp, #1072
	mov	r2, #512
	mov	r1, #0
	add	r0, r0, #12
	bl	memset
	mov	r9, r8
	mov	lr, r6
	ldr	r7, [sp, #16]
.L352:
	mov	r8, #0
	mov	r1, r8
	ldr	ip, [lr, #4]!
	mov	r8, r10
	mov	r3, r6
	mov	r10, lr
	mov	lr, r1
	sub	r2, r7, #252
	str	r6, [sp]
.L353:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r5, r0, lr
	adc	r6, r1, #0
	cmp	r3, r4
	str	r5, [r2], #4
	mov	lr, r6
	bne	.L353
	mov	lr, r10
	mov	r1, r6
	cmp	lr, r4
	mov	r10, r8
	ldr	r6, [sp]
	str	r1, [r7, #4]!
	bne	.L352
	add	r1, sp, #1072
	mov	r2, #512
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #316
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	ldr	r3, [sp, #24]
	add	r10, r10, #1
	cmp	r10, r3
	mov	r8, r9
	ble	.L355
.L356:
	mov	r2, #256
	add	r1, sp, #60
	ldr	r0, [sp, #36]
	bl	memcpy
	ldr	r3, .L563
	ldr	r2, [sp, #1596]
	ldr	r3, [r3]
	cmp	r2, r3
	bne	.L560
	add	sp, sp, #1600
	add	sp, sp, #4
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L557:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L333:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L334:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L334
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L333
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L332
.L541:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L221:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L222:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L222
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L221
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L220
.L540:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L214:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L215:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L215
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L214
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L213
.L539:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L207:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L208:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L208
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L207
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L206
.L538:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L200:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L201:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L201
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L200
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L199
.L537:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L193:
	sub	r2, lr, #252
	mov	r9, lr
	mov	r3, r6
	mov	lr, r5
	ldr	ip, [r7, #4]!
	str	r6, [sp, #24]
	str	r7, [sp]
.L194:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L194
	mov	r1, r7
	mov	lr, r9
	ldr	r7, [sp]
	ldr	r6, [sp, #24]
	cmp	r7, r8
	str	r1, [lr, #4]!
	bne	.L193
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L192
.L536:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L186:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L187:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L187
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L186
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L185
.L535:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L179:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L180:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L180
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L179
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L178
.L534:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L172:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L173:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L173
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L172
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L171
.L533:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L165:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L166:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L166
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L165
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L164
.L532:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L158:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L159:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L159
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L158
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L157
.L531:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L151:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L152:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L152
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L151
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L150
.L530:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L144:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L145:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L145
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L144
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L143
.L529:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L137:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L138:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L138
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L137
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L136
.L528:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L130:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L131:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L131
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L130
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L129
.L527:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L123:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r9, r7
	mov	lr, r5
	add	r3, sp, #312
	str	r6, [sp, #24]
	str	r1, [sp]
.L124:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L124
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L123
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L122
.L549:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L277:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L278:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L278
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L277
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L276
.L548:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L270:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L271:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L271
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L270
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L269
.L547:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L263:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L264:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L264
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L263
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L262
.L546:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L256:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L257:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L257
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L256
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L255
.L545:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L249:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L250:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L250
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L249
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L248
.L544:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L242:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L243:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L243
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L242
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L241
.L543:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L235:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L236:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L236
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L235
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L234
.L564:
	.align	2
.L563:
	.word	__stack_chk_guard
.L542:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L228:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L229:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L229
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L228
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L227
.L553:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L305:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #28]
	str	r1, [sp, #24]
.L306:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	stm	sp, {r6-r7}
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L306
	mov	r1, r7
	ldr	lr, [sp, #24]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #28]
	str	r1, [lr, #4]!
	bne	.L305
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L304
.L552:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L298:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L299:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L299
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L298
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L297
.L551:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L291:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L292:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L292
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L291
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L290
.L550:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L284:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L285:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L285
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L284
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L283
.L555:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L319:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L320:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L320
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L319
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L318
.L554:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L312:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L313:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L313
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L312
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L311
.L556:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L326:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #24]
	str	r1, [sp]
.L327:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L327
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #24]
	str	r1, [lr, #4]!
	bne	.L326
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L325
.L559:
	add	r0, sp, #1072
	mov	r2, #512
	mov	r1, #0
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L349:
	mov	r9, #0
	mov	r1, r9
	ldr	ip, [r7, #4]!
	mov	r9, r10
	mov	r10, r7
	mov	r7, r1
	sub	r2, lr, #252
	add	r3, sp, #312
	str	r6, [sp]
.L350:
	mov	r0, r7
	mov	r1, #0
	ldr	r7, [r3, #4]!
	umlal	r0, r1, ip, r7
	ldr	r7, [r2]
	adds	r5, r0, r7
	adc	r6, r1, #0
	cmp	r4, r3
	str	r5, [r2], #4
	mov	r7, r6
	bne	.L350
	mov	r7, r10
	mov	r1, r6
	cmp	r7, r8
	mov	r10, r9
	ldr	r6, [sp]
	str	r1, [lr, #4]!
	bne	.L349
	add	r1, sp, #1072
	mov	r2, #512
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L348
.L558:
	add	r0, sp, #1072
	mov	r2, r10
	mov	r1, r5
	add	r0, r0, #12
	bl	memset
	ldr	lr, [sp, #16]
	add	r7, sp, #56
.L340:
	ldr	ip, [r7, #4]!
	mov	r1, lr
	sub	r2, lr, #252
	mov	r3, r6
	mov	lr, r5
	mov	r9, r7
	str	r6, [sp, #20]
	str	r1, [sp]
.L341:
	mov	r0, lr
	mov	r1, #0
	ldr	lr, [r3, #4]!
	umlal	r0, r1, ip, lr
	ldr	lr, [r2]
	adds	r6, r0, lr
	adc	r7, r1, #0
	cmp	r4, r3
	str	r6, [r2], #4
	mov	lr, r7
	bne	.L341
	mov	r1, r7
	ldr	lr, [sp]
	cmp	r9, r8
	mov	r7, r9
	ldr	r6, [sp, #20]
	str	r1, [lr, #4]!
	bne	.L340
	add	r1, sp, #1072
	mov	r2, r10
	add	r1, r1, #12
	ldr	r0, [sp, #12]
	bl	memcpy
	add	r2, sp, #60
	mov	r1, fp
	ldr	r0, [sp, #12]
	bl	mm_redc
	b	.L339
.L358:
	mov	r2, #0
	b	.L121
.L560:
	bl	__stack_chk_fail
	.size	mm_exp, .-mm_exp
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"num\000"
.LC1:
	.ascii	"%s\011\000"
.LC2:
	.ascii	"\012 2^%4d  \000"
	.space	2
.LC3:
	.ascii	"%8.8x \000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
