	.text
	.global	memcpy
	.type	memcpy, %function
	.global	memcpy_aligned
	.type	memcpy_aligned, %function
memcpy:
	ands	r3, r1, #3
	bne	.Lsrc_align	@ src not aligned

.Lsrc_aligned:
	tst	r0, #3
	bne	.Ldst_align	@ dst not aligned

memcpy_aligned:
	@ Notes: we could copy more at each loop
	@	16, 32 or even 64 bytes but at the
	@	cost of having to store & restore a few
	@	registers (2 for 16 bytes, 6 for 32 or 64)
.Lloop:
	subs	r2, r2, #8
	ldmgeia	r1!, {r3, ip}
	stmgeia	r0!, {r3, ip}
	bgt	.Lloop
	bxeq	lr

	@ It's OK if r2 < 2, we're only testing the lowest bits now
	@	r2 = n modulo granularity of the main loop
	tst	r2, #4
	ldrne	r3, [r1], #4
	strne	r3, [r0], #4

.Llast_3:
	@ n < 4
	@ src or dst may be unligned
	tst	r2, #2
	ldrneb	r3, [r1], #1
	strneb	r3, [r0], #1
	ldrneb	r3, [r1], #1
	strneb	r3, [r0], #1
	tst	r2, #1
	ldrneb	r3, [r1], #1
	strneb	r3, [r0], #1

	bx	lr

.Lsrc_align:
	subs	r2, r2, #4
	@ r2 = n - 4
	blt	.Llast_3

	cmp	r3, #2
	add	r2, r2, r3	@ r2 = (n - 4) + mis
	ldrltb	r3, [r1], #1	@ mis: 1
	strltb	r3, [r0], #1
	ldrleb	r3, [r1], #1	@ mis: 1,2
	strleb	r3, [r0], #1
	ldrb	r3, [r1], #1	@ mis: 1,2,3
	strb	r3, [r0], #1
				@ n' = n - (4 - mis)
				@    = r2

	b	.Lsrc_aligned

.Ldst_align:
	@ (r1 & 3) == 0
	@ (r0 & 3) != 0

	cmp	r2, #3
	blt	.Llast_3

	tst	r0, #1
	bne	.Lcopy_by_1

.Lcopy_by_2:
	subs	r2, r2, #2
	ldrgeh	r3, [r1], #2	@ mis: 1
	strgeh	r3, [r0], #2
	bgt	.Lcopy_by_2
	bxeq	lr

	add	r2, r2, #2

.Lcopy_by_1:
	subs	r2, r2, #1
	ldrgeb	r3, [r1], #1	@ mis: 1
	strgeb	r3, [r0], #1
	bgt	.Lcopy_by_1

	bx	lr

	.size	memcpy, .-memcpy
	.size	memcpy_aligned, .-memcpy_aligned
