***********************************************************************************
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
***********************************************************************************
* ====================================================================
* Do not touch the following 3 xdef lists:
* ====================================================================
	xref ReadInt, WriteInt
        xdef Start, Stop, End
        xdef i, j, k
* ********************************************************************
* Add more xdef directives below if you need more external labels
* - You can only add a label as a break point (stop location) in EGTAPI
* - if the label has been xdef!
* - You will probably need to use break points for debugging 
* - in this project.
* ********************************************************************

Start:
* ********************************************************************
* Put your main program after this line and the Stop label below
* Your main program must read in i, j, and k and
* print the value of F(i,j,k)
* ********************************************************************
	jsr    ReadInt	     * read i, j, k from input file
	move.l D0, i
     	jsr    ReadInt
	move.l D0, j
	jsr    ReadInt
	move.l D0, k
	move.l i, -(A7)	     * add the parameters into the stack
        move.l j, -(A7)
	move.l k, -(A7)

	bsr    F	     * call F(i, j, k)
	adda.l #12,A7
	jsr    WriteInt	     * put the value in D0 into termout0

* ====================================================================
* Don't touch the stop label - you need it to stop the program
* ====================================================================
Stop:   nop
        nop
i:      ds.l 1
j:      ds.l 1
k:      ds.l 1


F:
* ********************************************************************
* Put your recursive function F here 
* ********************************************************************
	move.l A6, -(A7)    * Prelude, set the A6 pointer
	move.l A7, A6
	suba.l #8, A7	    * set stack space for the local variables s & t

* now the variables in stack will be
* i = 16(A6), j = 12(A6), k = 8(A6), s = -4(A6), t = -8(A6)

* start of function:

	move.l 16(A6), D1
	move.l 12(A6), D2
	cmp.l  #0, D1
	bgt    T1
T1:	cmp.l  #0, D2
	bgt    else1

	move.l #0,D0
********************************* 
        move.l A6, A7           ; Deallocate local variable(s)
        move.l (A7)+,A6        ; restore caller's frame pointer
*********************************
	rts

else1:  			
	move.l 16(A6), D1	  * else 1
	move.l 12(A6), D2
	add.l  D1, D2		  * D2 = i + j
	cmp.l  8(A6),  D2	  * i + j ? k
	bge    else2
 	add.l  8(A6),D1

	move.l D1,D0		  * if (i+j)< k, return i+k
********************************* 
        move.l A6, A7           ; Deallocate local variable(s)
        move.l (A7)+, A6        ; restore caller's frame pointer
*********************************
	rts

else2: 
       move.l #0, -4(A6)          * else2
       move.l #1, -8(A6)

whileloopstart:
       move.l -8(A6), D1	  * start the for loop
       move.l 16(A6), D2	  * D1 = t, D2 = i, D3 = j, D4 = k
       move.l 12(A6), D3
       move.l 8(A6), D4

       cmp.l  8(A6), D1           * end the loop yet?
       bge    whileloopend

       sub.l  D1,D2		  * D2 = i - t
       sub.l  D1,D3	          * D3 = j - t
       sub.l  #1,D4		  * D4 = k - 1
       
       move.l D2, -(A7)		  * push new parameters into stack
       move.l D3, -(A7)
       move.l D4, -(A7)
       bsr    F			  * recursion of the F func
       adda.l #12, A7

       move.l -4(A6), D5
       add.l  D0,D5
       move.l D5, -4(A6)

       move.l -8(A6), D1	  * add t by 1
       add.l  #1,D1
       move.l D1,-8(A6)
       bra    whileloopstart   

whileloopend:
       move.l -4(A6), D0
********************************* 
       move.l A6, A7           ; Deallocate local variable(s)
       move.l (A7)+, A6        ; restore caller's frame pointer
*********************************
	rts

* ====================================================================
* Don't add anything below this line...
* ====================================================================
End:
        end