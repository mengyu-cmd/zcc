.inscope	.Lfun2
main:
.inscope	.LEnv3
.inscope	.Lfor8
=f	.Lf9	y	.Lvar10
.Lforb5:
if	y > .Lf11	goto .Lfort7
goto .Lforn4
.Lfe36:
-f	.Lf12	y	.Lvar13
=f	.Lvar13	y	.Lvar14
goto .Lforb5
.Lfort7:
.inscope	.LEnv15
.inscope	.Lfor20
=f	.Lf21	x	.Lvar22
.Lforb17:
if	x < .Lf23	goto .Lfort19
goto .Lforn16
.Lfe318:
+f	.Lf24	x	.Lvar25
=f	.Lvar25	x	.Lvar26
goto .Lforb17
.Lfort19:
.inscope	.LEnv27
*f	x	x	.Lvar28
*f	y	y	.Lvar29
+f	.Lvar29	.Lvar28	.Lvar30
-f	.Lf31	.Lvar30	.Lvar32
=f	.Lvar32	a	.Lvar33
clr
*f	a	a	.Lvar35
*f	a	.Lvar35	.Lvar36
*f	x	x	.Lvar37
*f	y	.Lvar37	.Lvar38
*f	y	.Lvar38	.Lvar39
*f	y	.Lvar39	.Lvar40
-f	.Lvar40	.Lvar36	.Lvar41
if	.Lvar41 <= .Lf42	goto .Lift43
goto .Liff44
.Lift43:

param 42
call           putchar        1	.Lret45
goto	.Lsn34
.Liff44:

param 32
call           putchar        1	.Lret46
.Lsn34:
clr
.outscope
.outscope
goto .Lfe318
.Lforn16:
clr

param 10
call           putchar        1	.Lret47
clr
.outscope
.outscope
goto .Lfe36
.Lforn4:
clr
.outscope
.outscope
.end
