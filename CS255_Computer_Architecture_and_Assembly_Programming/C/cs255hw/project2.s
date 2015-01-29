***********************************************************************************
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
***********************************************************************************
* BubbleSort skeleton file
* 
        xdef Start, Stop, End
	xref WriteInt, WriteLn
        xdef A, B, Start2
        xdef BubbleSort

*****************************************************************************
* Main program: call BubbleSort twice to sort 2 different arrays
*	 	and print the sorted arrays out
*
*       ******   DO NOT change the main program.  ***********
* 
* Write your BubbleSort routine starting at the "BubbleSort:" label below
*****************************************************************************
Start:
	move.l #A, D0
	move.l #5, D1
	bsr    BubbleSort		* Sort array A

**** Print array A
	move.l #0, i
Print1: 
	move.l #5, D0
	cmp.l  i, D0
	beq    Start2

	move.l #A, A0
	move.l i, D0
	muls  #4, D0
	move.l  0(A0, D0.w), D0
	jsr    WriteInt

	addi.l  #1, i
	bra    Print1

Start2:
        move.l #str, A0
        move.l #5, D0
	jsr    WriteLn

        move.l #B, D0
        move.l #10, D1
        bsr    BubbleSort		* Sort array B

**** Print array B
        move.l #0, i
Print2:
        move.l #10, D0
        cmp.l  i, D0
        beq    Stop

        move.l #B, A0
        move.l i, D0
        muls  #4, D0
        move.l  0(A0, D0.w), D0
        jsr    WriteInt

        addi.l  #1, i
        bra    Print2


*************************************************************************
* Write your code for the BubleSort() function here
*
*  The BubbleSort subroutine MUST receive the inputs as follows:
*
*          D0 = addres of int array to be sorted
*          D1 = N
*
*  No value needs to be returned....
*************************************************************************


BubbleSort:
****************************************************************************
* Write your assembler instructions below (do not pass the rts instruction)
****************************************************************************
	suba.l 	#12, A7	    * using for store done, k, help
	move.l 	#0, 8(A7)    * done = 8(A7)  
	move.l 	#1, 4(A7)    * k = 4(A7)

whilestart:
	move.l  8(A7), D2
	cmp.l   #0, D2			* done ?= 0
	bne     whileend
    
    	move.l  #1, 8(A7)		* k = 1
    	move.l  #0, i			* i = 0
    
forloopstart:				
	move.l  D1, D2			* get the N - k
	sub.l   4(A7), D2
	cmp.l   i, D2
	ble     forloopend		* end the for loop
	
ifloopstart:
	movea.l D0, A0 		   	* A0 contains the address of the array
	move.l  i, D2       		* D2 = i
	muls    #4, D2      		* get the offset
	move.l  0(A0, D2.w), D3  	* D3 = array[i]
	move.l  4(A0, D2.w), D4  	* D4 = array[i+1]
	
	cmp.l   D3, D4
	bge     ifloopend     		* if A[i] <= A[i+1]
	
Swap:					
	move.l  D3, 0(A7)    
	move.l  D4, D3
	move.l  0(A7), D4
	move.l  D3, 0(A0, D2.w)   	* get the swap element back to the array
	move.l  D4, 4(A0, D2.w)
	move.l  #0, 8(A7)            	* change the done status
	
ifloopend:
	move.l  i, D2    		* i = i + 1
	add.l   #1, D2
	move.l  D2, i
	bra	    forloopstart
		
forloopend:				    
	move.l  4(A7),  D2		* k = k + 1
	add.l   #1, D2
	move.l  D2, 4(A7)
	bra     whilestart
	
whileend:   
	adda.l #12, A7			* restore the stack

    rts	*** I have added a rts to make sure your function returns....

* ********************************************************************
* Add more variables here if you need them
* ********************************************************************
help: ds.l 1
done: ds.l 1
k:    ds.l 1

* ********************************************************************
* Don't touch the stop label - you need it to stop the program
* ********************************************************************
Stop:   nop
        nop

* ********************************************************************
* Don't touch these variables (used by main program)
* ********************************************************************
A:      dc.l 8, -1, -8, 4, 11
B:      dc.l 78, -1, 8, 45, 11, -89, 56, 9, 12, -19
i:      ds.l 1
str:    dc.b '--------'

End:

        end