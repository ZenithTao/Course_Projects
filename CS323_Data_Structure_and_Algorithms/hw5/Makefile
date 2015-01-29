# Implicit rule for compiling .java files:
.SUFFIXES: .java .class
.java.class:; javac $<

CLASS   = cs323
GHOME	= /home/cs323000
HW	= hw5

help:	# print this help message (default action)
	@echo "The Makefile defines the following commands:"
	@sed -n '/^[a-z0-9]*:.*#/{;s/^/ make /;s/:.*#/	---/;p;}' Makefile

copy: # make your personal copy of these files
	@echo "*** Copying files to your ~/$(CLASS)/$(HW)/ directory"
	cp -ipr $(GHOME)/share/$(HW) $(HOME)/$(CLASS)/

compile: Driver.class BWT.class # compile BWT.java and Driver.java

run: compile # test BWT on banana.txt (tiny file)
	/bin/rm -f bwt*.txt
	java -ea Driver banana.txt % bwt.txt
	java -ea Driver bwt.txt % bwt-back.txt
	cmp banana.txt bwt-back.txt
	: ### Good, you recovered the original file ###

ghost: compile # test BWT on ghost.txt (small file)
	/bin/rm -f bwt*.txt
	java -ea Driver ghost.txt % bwt.txt
	java -ea Driver bwt.txt % bwt-back.txt
	cmp ghost.txt bwt-back.txt
	: ### Good, you recovered the original file ###

pipi: compile # timed test of BWT on pipi.txt (big repetitive file)
	/bin/rm -f bwt*.txt
	time java -ea Driver pipi.txt % bwt.txt
	time java -ea Driver bwt.txt % bwt-back.txt
	cmp pipi.txt bwt-back.txt
	: ### Good, you recovered the original file ###

manber: demo/Manber.class # demonstrate demo/Manber.java
	java -cp demo -ea Manber 'yabbadabbadoo!'

charsort: demo/CharSort.class # demonstrate demo/CharSort.java
	java -cp demo -ea CharSort 'yabbadabbadoo!'

turnin: # turn in your BWT.java
	$(GHOME)/turnin $(HW)

clean:	# delete temporary and backup files
	/bin/rm -f *.class demo/*.class bwt*.txt *~ demo/*~