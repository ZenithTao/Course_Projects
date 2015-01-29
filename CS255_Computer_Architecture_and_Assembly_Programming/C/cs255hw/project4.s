***********************************************************************************
* Liang Tao
* ltao5@emory.eud   Emory ID:2019389
* I worked on this assignment alone, using only this semester's course materials. 
*********************************************************************************** ====================================================================
* Do not touch the following xdef lists:
* ====================================================================
	xref ReadInt, PrintList, malloc
        xdef Start, Stop, End
        xdef head, elem, i
* ********************************************************************
* Add more xdef directives below if you need more external labels
* - You can only add a label as a break point (stop location) in EGTAPI
* - if the label has been xdef !!!
* - And I am pretty sure you need to use break point for debugging 
* - in this project...
* ********************************************************************

Start:
* ********************************************************************
* Put your main program after this line and the Stop label below
* See project page for a description of the main function
* ********************************************************************
	move.l #0, head    * head = null
        move.l #0, i	   * i = 0
forstart:
	move.l i, D1	   * if i >= 5 then stop the program
        cmp.l  #5,D1
        bge    forend
	
	jsr    ReadInt     * else, read the termin0
 	move.l D0, D1	   * use D1 to save the read number
        move.l #8, D0
        jsr    malloc
        move.l A0, elem    
        move.l D1, (A0)    * element.value = read number

        move.l head, -(A7)    * call InsertList
        move.l elem, -(A7)    * use stack to save head and elem
        bsr    InsertList 
        adda.l #8, A7 
        move.l D0, head   

        move.l head, A0
        jsr    PrintList   * After each iteration, print the list
          
        move.l i, D1
        add.l  #1,D1
        move.l D1, i	   * i = i + 1
        bra    forstart    * start the forloop again

forend:
* ====================================================================
* Don't touch the stop label - you need it to stop the program
* ====================================================================
Stop:   nop
        nop

head:   ds.l 1
elem:   ds.l 1
i:	ds.l 1	
* ********************************************************************

InsertList:
* ********************************************************************
* Put your InsertList function here 
* ********************************************************************
* ********************************************************************
*Algorithm for insertlist:
*List InsertList (List head, List elem) {
*    List help;
*    if (head == null || head.value <= elem.value) {
*       elem.next = head;
*	return elem;
*    } else {
*	help = insertList(head.next, List Elem);
*	head.next = help;
*	return head;
*   }
*}
* ********************************************************************
	move.l A6, -(A7)
	move.l A7, A6
	suba.l  #4, A7
	
	move.l 12(A6), D0	* DO = head
	cmp.l  #0, D0		* head =? null
	beq    doif

	move.l 12(A6), A0
	move.l 8(A6), A1
        move.l (A0), D1		* D1 = head.value
	move.l (A1), D2		* D2 = elem.value
	
	cmp.l  D2, D1
	bgt    else
doif:
	move.l 12(A6), A0	* A0 = head
	move.l 8(A6), A1	* A1 = elem
	move.l A0, 4(A1)	* elem.next = head
        move.l 8(A6), D0	* return elem

 	move.l A6, A7
	move.l (A7)+, A6
	rts
else:
	move.l 12(A6), A0	* A0 = head
	move.l 4(A0), -(A7)	* pass two parameters again, recursively
	move.l 8(A6), -(A7)
	bsr    InsertList
	adda.l #8, A7		* pop out from the stack
	move.l D0, -4(A6)	* help = insertList(head.next, List Elem);

	move.l 12(A6), A0	* A0 = head
	move.l -4(A6), 4(A0)    * head.next = help

	move.l 12(A6), D0	* return head

 	move.l A6, A7
	move.l (A7)+, A6
	rts

InsertListEnd:
* ====================================================================
* Don't add anything below this line...
* ====================================================================
End:
        end