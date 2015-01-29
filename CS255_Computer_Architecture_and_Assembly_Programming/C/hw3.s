	xdef Start, Stop, End
	xdef Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11, Q12
	xdef v1, v2, v3, v4, v5, v6, v7, v8, v9, v10

Start:
***********************************************************************************
* Put your assembler instructions here
* Write the answer to each question after the corresponding label.
* *** Failure to do so will result in point deductions! ***
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
***********************************************************************************

Q1: move.b #14, v1

Q2: move.b B,v2

Q3: move.b 20000, v3

Q4: move.b v4, 20001

Q5: move.w #-3000, D0

Q6: move.w S, v6

Q7: move.w 20002, v7

Q8: move.l #-400400, v8

Q9: move.l L, v9

Q10: move.l 20004, v10

Q11: move.l #%11010111110110101111100011111110, D0
Q12: ext.w D0


Stop:	nop
	nop

*************************************************
* Put your variable definitions here...
*************************************************

v1: ds.b 1	

v2: dc.b -12

v3: dc.b $FFFFFFA1

v4: dc.b -28

v5: ds.w 1

v6: dc.w -7

v7: dc.w %1111111111000000

v8: ds.l 1

v9: dc.l -15

v10: dc.l @151



*************************************************
* Don't edit any line below this text!
*************************************************
End:
         ORG 10000
B:  dc.b    13
    dc.b     0
S:  dc.w    -4
L:  dc.l    -8

         ORG 20000-$1600
    dc.b    -123
    dc.b       0
    dc.w    -314
    dc.l    -4321
	end
