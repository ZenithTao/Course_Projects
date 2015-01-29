CP=.:book.jar
JAVA=java -Xss2M -cp $(CP)
JAVAC=javac -cp $(CP) -g
HW=hw3
GHOME=/home/cs323000
BASE=run1
SEP=/

.SUFFIXES: .java .class
.java.class:; $(JAVAC) $<

help: # print this help (default action)
	@echo "The Makefile defines the following actions:"
	@sed -n '/^[-a-z0-9]*:.*#/{;s/^/ make /;s/:.*#/	---/;p;}' Makefile

copy: # make your initial personal copy of these files
	/bin/cp -ipr $(GHOME)/share/$(HW) $(HOME)/cs323

compile: TwoEC.class Driver.class # recompile if needed

# "run" is used by the tests (they may change BASE and SEP first)
run: compile
	### running Driver on $(BASE).txt, copying output to $(BASE)-out.txt
	$(JAVA) -ea Driver $(BASE).txt "$(SEP)" | tee $(BASE)-out.txt
	### comparing your output to expected
	diff -b $(BASE)-out.txt $(BASE)-exp.txt
	### Good, you got the expected output

run1: # run Driver on run1.txt
	@$(MAKE) BASE=run1 run

run2: # run Driver on run2.txt
	@$(MAKE) BASE=run2 run

run3: # run Driver on run3.txt
	@$(MAKE) BASE=run3 run

run4: # run Driver on run4.txt
	@$(MAKE) BASE=run4 run

movies: # run Driver on movies.txt (a large input)
	@$(MAKE) BASE=movies run

turnin: # turn in your work
	$(GHOME)/turnin $(HW)

clean: # remove less important files
	/bin/rm -f *.class *-out.txt *~
