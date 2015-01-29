movea.l #A, A0
move.b 4(A0), D0
EXT.W D0
EXT.L D0
move.l D0, ans_l

movea.l #B, A0
move.w #7, D1
muls #2, D1
move.w 0(A0, D1.w), D0
ext.l D0
move.l D0 ans_l

movea.l #C, A0
move.l k, D0
muls #4, D0
move.l 0(A0, D0.w), ans_l

movea.l #A, A0
move.w j, D0
ext.l D0
add.l k, D0
move.b 0(A0, D0.w), D0
ext.w D0
move.l D0, ans_w

movea.l #C, A0
move.b i, D0
ext.w D0
ext.l D0
add.l k, D0
muls #4, D0
move.l 0(A0, D0.w), D1
move.w D1, ans_w