*
	xdef Start, Stop, End
	xdef Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10
	xdef A, B, C
	xdef i, j, k
	xdef head
	xdef ans_b, ans_w, ans_l

Start:
*******************************************************************
* Put your assembler instructions here
* Write the answer to each question after the corresponding label.
* DO NOT REMOVE ANY LABEL IN THIS ASSIGNMENT (all labels are now NECESSARY)
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
* *** Failure to do so will result in point deductions ***
*******************************************************************

Q1: MOVEA.L #A, A0		* move the array address to A0 address register
    MOVE.B 4(A0), D0		* 4 bytes
    EXT.W D0
    EXT.L D0 			* extend the byte value to long because ans_l is long
    MOVE.L D0, ans_l

*         ans_l = A[4];





Q2: MOVEA.L #B, A0		* move the array address to A0 address register
    MOVE.W 14(A0), D0		* B is a short array, so add 7 * 2 bytes 
    EXT.L D0
    MOVE.L D0, ans_l		* likewise, ans_l is a long value
    
*         ans_l = B[7];





Q3: MOVEA.L #C, A0	  	* move the array address to A0 address register
    MOVE.L k, D0
    MULS #4, D0			* C is long array
    MOVE.L 0(A0, D0.W), ans_l
*         ans_l = C[k];





Q4: MOVEA.L #A, A0		* move the array address to A0 address register
    MOVE.W j, D0
    EXT.L D0
    ADD.L k, D0			* add j and k
    MOVE.B 0(A0, D0.W), D1	* get the A[j + k]
    EXT.W D1			* extend the byte to short value
    MOVE.W D1, ans_w
*         ans_w = A[j + k];      




Q5: MOVEA.L #C, A0		* move the array address to A0 address register
    MOVE.B i, D0	
    EXT.W D0
    EXT.L D0
    ADD.L k, D0			* add i and k
    MULS #4, D0
    MOVE.L 0(A0, D0.W), D1	* get the long word value in C array
    MOVE.W D1, ans_w
*         ans_w = C[i + k];      




Q6: MOVEA.L #A, A0		* move A and B into different address registers
    MOVEA.L #B, A1
    MOVE.W j, D1
    MOVE.B 0(A0, D1.W), D3	* put the A[j] into D3
    EXT.W D3
    EXT.L D3
    MOVE.B i, D2
    MULS #2, D2
    EXT.W D2
    MOVE.W 0(A1, D2.W), D4	* put the B[i] into D4
    EXT.L D4
    ADD.L D3, D4		* extend the short value B[i] to long and then add
    MOVE.L D4, ans_l
*         ans_l = A[j] + B[i];  




Q7: MOVE.L #A, A0		* move the array address to A0 address register
    MOVE.W j, D0
    MOVE.B 0(A0, D0.W), D1	* get the A[j]
    SUB.B #30, D1		* get the A[j] - 30
    EXT.W D1
    MOVE.B 0(A0, D1.W), D2	* get the A[A[j] - 30]
    EXT.W D2
    EXT.L D2			* extend the byte value to long
    MOVE.L D2, ans_l
*         ans_l = A[A[j] - 30];      



	
Q8: MOVE.L #B, A0
    MOVE.W 28(A0), ans_w
*         ans_w = B[14]
* The index of the B array(14) has exceeded the upbound(10), and the answer is -1




Q9: MOVEA.L head, A0		* put the head address into the A0 address register
    MOVE.L (A0), ans_l		* because head is the first element of the linked list
*	  ans_l = head.value1;



Q10: MOVEA.L head, A0		* put the head address into the A0 address register
     MOVEA.L 6(A0), A0
     MOVEA.L 6(A0), A0		* get the head.next.next
     MOVE.L 4(A0), ans_w 	* get the head.next.next.value
*	  ans_w = head.next.next.value2;



*************************************************
* Don't write any code below this line
*************************************************

Stop:	nop
	nop

*************************************************
* Don't touch these variables
* You do NOT need to define more variables !!!
*************************************************

ans_b: ds.b 2
ans_w: ds.w 1
ans_l: ds.l 1

i:     dc.b  2
************************************************************************
* We need to add a 1 byte dummy variable to make the next variable j
* locate on an EVEN address.
* Due to some architectural constraints, short and int variables MUST
* start on an even address (or you will get an "odd address" error)
************************************************************************
dummy: ds.b 1

j:   dc.w  3
k:   dc.l  4

A:  dc.b   4, 11, -22, 33, -44, 55, -66, 77, -88, 99

B:  dc.w   24, 111, -222, 333, -444, 555, -666, 777, -888, 999

C:  dc.l   987, 1111, -2222, 3333, -4444, 5555, -6666, 7777, -8888, 9999


head:   dc.l  list1

list3:  dc.l 1234
        dc.w 12
	dc.l list4
list2:  dc.l 5678
        dc.w 34
	dc.l list3
list4:  dc.l 9012
        dc.w 56
	dc.l list5
list1:  dc.l 3456
        dc.w 78
	dc.l list2
list5:  dc.l 7890
        dc.w 90
	dc.l 0


End:
	end
