default: 
	gcc -Wall -Wextra -g -o simpsh lab1.c
#run smoke-tests
check: default test1 test2 test3
	#after running different tests, check wether they were successful or not
	if [[ -e testFile.txt ]]; then \
	echo "Smoke tests were successful!"; \
	rm -f testFile.txt; \
	else \
	echo "Didn't pass the Smoke-Tests!"; \
	fi
clean: 
	rm -f *.o *.txt *.tar.gz simpsh report.pdf
dist: default
	tar -czvf lab1-305179067.tar.gz lab1.c Makefile README report.pdf bashTest.bash dashTest.dash

test1:
	echo ""
	echo "Test 1"
	./simpsh --pipe --command 0 1 1 sleep 1 --creat --profile --rdwr test1io.txt \
	  >c1out.txt 2>c1err.txt
	if [ $? == 0 ] && [ ! -s c1err.txt ] && [ -s c1out.txt ] && [ -e test1io.txt ] \
	  && [ ! -s test1io.txt ] && wc -l < c1out.txt | grep -q "1"
	then
	  echo "Passed Test 1"
	else
	  echo "Failed Test 1"
	fi
test2:
	echo ""
	echo "Test 2 "
	./simpsh --creat --excl --rdwr test2io.txt --command 0 0 0 time -p sleep 1 \
	  --pipe --wait --rdonly a8.txt --profile --command 3 2 0 cat --command 1 0 0 cat \
	  >c2out.txt 2>c2err.txt
	T2RC=$?
	sleep 1
	if [ $T2RC == 0 ] && [ -e test2io.txt ] && grep -q "real 1." test2io.txt \
	  && grep -q "Knowledge crowns those who seek her" test2io.txt \
	  && grep -q "Hello world! CS 111! " test2io.txt \
	  && grep -q "exit 0 time -p sleep 1" c2out.txt && ! grep -q "cat" c2out.txt \
	  && [ ! -s c20err.txt ]
	then
	  echo "Passed Test 2"
	else
	  echo "Failed Test 2"
	fi
test3:
	./simpsh >c1out.txt 2>&1; \
	if [[ $$? == 0 ]] && [[ ! -s c1out.txt ]]; \
	then \
	echo "--wronly failed" >> testFile.txt; \
	exit 0; \
	fi