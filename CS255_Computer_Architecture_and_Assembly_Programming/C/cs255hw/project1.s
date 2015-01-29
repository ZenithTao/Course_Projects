***********************************************************************************
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
***********************************************************************************
* ********************************************************************
* Do not touch the following 2 xdef lists:
* ********************************************************************
        xdef Start, Stop, End
        xdef a, b, len_a, len_b, min, max, common

* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* You can add more xdef here to export labels to EGTAPI
* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	xdef i, j

* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*      Put your assembler program here - between the start and stop label
*      DO NOT define any variables here - see the variable section below
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Start:

* start with A[]
* fisrt choose the A[0] as the max because we start with a0

FindMaxInA: 	
			MOVE.L a, max			
       		MOVE.L #1, i

WhileMaxStart1:
	MOVE.L  i, D0
	CMP.L   #len_a, D0
	BGE     WhileMaxEnd1	* if i is greater than the length of the array, end
       
	MOVEA.L #a, A0		* else test A[i + 1] ? (max = A[i])
	MOVE.L  i, D0
	MULS 	#4, D0
 	MOVE.L  0(A0, D0.W), D0
	
	MOVE.L  max, D1
	CMP.L   D1, D0
 	BLE 	IfMax1End	* if A[i + 1] < max, then i++, and start the whileloop 
	
	MOVE.L  D0, max		* else let max = A[i + 1]

IfMax1End:	
	MOVE.L 	i, D0
	ADD.L	#1, D0
	MOVE.L 	D0, i

	BRA	WhileMaxStart1
WhileMaxEnd1:

test1end:

* Find the min in A
* move the A[0] to min
FindMinA:	MOVE.L a, min
		MOVE.L #1, i

WhileMinStart1:
	MOVE.L 	i, D0
	CMP.L 	#len_a, D0
	BGE	WhileMinEnd1	* if i >= len_a, end the while loop

	MOVEA.L #a, A0
	MOVE.L 	i, D0
	MULS 	#4, D0
	MOVE.L 	0(A0, D0.W), D0
	
	MOVE.L 	min, D1
	CMP.L	D0, D1
	BLT	IfMinEnd1	* if min < A[i], do the next loop

	MOVE.L 	D0, min		* else, let min = A[i]

IfMinEnd1:
	MOVE.L	i, D0		* else loop
	ADD.L 	#1, D0
	MOVE.L 	D0, i

	BRA 	WhileMinStart1

WhileMinEnd1:	
	


* Now we have A[min] and A[max], then what we need to do is to check whether
* B has specific values less than A[min] and A[max]

* first we check the max value

FindMaxAB:	MOVE.L #0, i	

WhileMaxStart:			
	MOVE.L 	i, D0		* start the while loop
	CMP.L  	#len_b, D0
	BGE	WhileMaxEnd
	
	MOVEA.L #b, A0
	MULS 	#4, D0
	MOVE.L 	0(A0, D0.W), D0
	MOVE.L 	max, D1
	CMP.L 	D0, D1		* compare B[i] to max
	BGE 	IfMaxEnd	
	
	MOVE.L 	D0, max		* if B[i] > max, move B[i] to max

IfMaxEnd:
	MOVE.L 	i, D0		* i++
	ADD.L 	#1, D0
	MOVE.L 	D0, i
	
	BRA 	WhileMaxStart

* end the while loop				
WhileMaxEnd:			


* likewise, we test B with the min value in A

FindMinAB: 	MOVE.L #0, i

WhileMinStart:
	MOVE.L 	i, D0		* start the while loop
	CMP.l 	#len_b, D0
	BGE   	WhileMinEnd
	
	MOVEA.L #b, A0
	MULS 	#4, D0
	MOVE.L 	0(A0, D0.W), D0
	
	MOVE.L 	min, D1
	CMP.L	D1, D0		* compare B[i] to min
	BGT 	IfMinEnd	* if B[i] < MIN, move B[i] to max
	
	MOVE.L 	D0, min

IfMinEnd:
	MOVE.L 	i, D0		* i++
	ADD.L 	#1, D0
	MOVE.L 	D0, i
	
	BRA 	WhileMinStart
	
WhileMinEnd:



* The final step is to find the common variables between A and B
* the basic algorithm is to use two loops 
FindCommon:	MOVE.L #0, i	* this is the array index for A
		MOVE.L #0, common	* this is the number of the common variables


WhileAStart: 
	MOVE.L 	#0, j			* this is the array index for B
	MOVE.L 	i, D0
	CMP.L 	#len_a, D0
	BGE 	WhileAEnd
	
	MOVEA.L #a, A0
	MULS 	#4, D0
	MOVE.L 	0(A0, D0.W), D0	 	* D0 = value of A[i]

WhileBStart:
	MOVE.L 	j, D1
	CMP.L 	#len_b, D1
	BGE 	WhileBEnd
	
	MOVEA.L #b, A0
	MULS 	#4, D1
	MOVE.L 	0(A0, D1.W), D1		* D1 = value of B[j]
	
	CMP.L 	D0, D1
	BNE 	IncreseJ		* if A[i] != B[j], just increase the index j by 1 
	
	MOVE.L 	common, D4
	ADD.L 	#1, D4
	MOVE.L 	D4, common		* if A[i] = B[j], then increase the sum by 1

IncreseJ:
	MOVE.L 	j, D3			* j++
	ADD.L 	#1, D3
	MOVE.L 	D3, j			
	
	CMP.L 	#len_b, D3		* test the j, if j >= length of array B, then i++
	BLT 	WhileBStart

	MOVE.L 	i, D3			* or i++
	ADD.L 	#1, D3
	MOVE.L 	D3, i

WhileBEnd:				
	BRA 	WhileAStart		* end the B loop and start from the A loop

WhileAEnd:





* ********************************************************************
* Don't touch the stop label - you need it to stop the program
* ********************************************************************
Stop:   nop


End:    nop

* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*    Variable Section -   Put your variables here
*
*    DO NOT define A, B and GCD !!!
*    They are already defined below
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
i:	ds.l 1
j: 	ds.l 1


* ********************************************************************
* XXXX Don't touch anything below this line !!!
* ********************************************************************
a:      dc.l  1, 2, 3, 4, 5
b:      dc.l  6, 7, 8, 9, 10, 11
len_a: 	equ 5
len_b:	equ 6
min:	ds.l 1
max:	ds.l 1
common: ds.l 1

* ********************************************************************
* Add more variables below if you need them
* ********************************************************************

        end