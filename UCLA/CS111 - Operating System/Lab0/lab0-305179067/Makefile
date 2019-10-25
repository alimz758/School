default: 
	gcc -Wall -Wextra -g -o lab0 lab0.c
#run smoke-tests
check: default successTest diffTest exitCode1Test exitCode2Test exitCode3Test exitCode4Test exitCode139Test
	#after running different tests, check wether they were successful or not
	if [[ -e testFile.txt ]]; then \
	echo "Smoke tests were successful!"; \
	rm -f testFile.txt; \
	else \
	echo "Didn't pass the Smoke-Tests!"; \
	fi
clean: 
	rm -f *.o *.txt *.tar.gz lab0
dist: default
	tar -czvf lab0-305179067.tar.gz lab0.c Makefile *.png README
# ------------------------------------ Smoke Tests --------------------------------
# used https://www.tldp.org/LDP/abs/html/fto.html for bash/files
# Success Test
successTest:
	echo "Write some text" > inputFile.txt; \
	./lab0 --input inputFile.txt --output outputFile.txt; \
	if [[ $$? -eq 0 ]]; then \
		echo "Exiting with code 0 " > log.txt; \
	fi; \
	rm -f inputFile.txt outputFile.txt;
#check if the input is the same as output
diffTest:
	echo "Write some text" > inputFile.txt; \
	./lab0 --input inputFile.txt --output outputFile.txt; \
	cmp inputFile.txt outputFile.txt; \
	if [[ $$? -eq 0 ]]; then \
	echo "Exit with code 0; inpout matches the output" >> testFile.txt;\
	fi; \
	rm -f inputFile.txt outputFile.txt;
#Test for exit code 1 using unrecognized argument
exitCode1Test:
	./lab0 --invalid; \
	if [[ $$? -eq 1 ]]; then \
    echo "Exiting with code 1 - unrecognized argument" >> testFile.txt; \
    fi;
# Test for exit code 2 with invalid input file.
exitCode2Test:
	#setting the input to some random file 
	./lab0 --input=egndslfklg45ly654mglkrgm453ig450gjlksdnf3i4t.txt; \
	if [[ $$? -eq 2 ]]; then \
    echo "Exiting with code 2 - unable to open input file" >> testFile.txt; \
    fi;
# Test for exit code 3 with output file that denies write permissions, modified using chmod -w.
exitCode3Test:
	touch output.txt; chmod -w output.txt; \
	./lab0 --output output.txt; \
	if [[ $$? -eq 3 ]]; then \
    echo "Exiting with code 3 - unable to open output file" >> testFile.txt; \
    fi; \
	rm -f output.txt;
# Test for exit code 4 with --segfault and --catch parameters.
exitCode4Test:
	./lab0 --segfault --catch; \
	if [[ $$? -eq 4 ]]; then \
     echo "Exiting with code 4 - caught and received SIGSEGV" >> testFile.txt; \
	fi;
# Test for exit code 139, the default segmentation fault code using --segfault parameter.
exitCode139Test:
	./lab0 --segfault --catch --dump-core; \
	if [[ $$? -eq 139 ]]; then \
	echo "Exiting with code 139 - dumped core." >> testFile.txt; \
	fi;