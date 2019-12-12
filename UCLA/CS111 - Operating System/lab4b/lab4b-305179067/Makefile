default:
	gcc -Wall -Wextra -lmraa -o lab4b lab4b.c -lm
clean:
	rm -f lab4b lab4b-305179067.tar.gz *~
dist:
	tar -czvf lab4b-305179067.tar.gz lab4b.c Makefile README test.sh
check: default
	chmod +x test.sh
	@-./test.sh