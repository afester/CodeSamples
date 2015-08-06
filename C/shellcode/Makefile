.PHONY:	exploitable

all: exploitable

exploitable:
	# gcc -g -Wall -pedantic -m32 -o exploitable exploitable.c

	# create two exploitable apps, one with executable stack and one with non-executable stack
	gcc -g -Wall -pedantic -m32 -fno-stack-protector -o exploitable exploitable.c

	cp exploitable exploitable_es
	execstack -s exploitable_es

	sudo chown root.root exploitable exploitable_es
	sudo chmod +s exploitable exploitable_es

	# gcc -g -Wall -pedantic -m32 -fno-stack-protector -S -o exploitable.S exploitable.c
	# gcc -g -Wall -pedantic -m32 -S -o exploitable2.S exploitable.c
	# gcc -g -Wall -pedantic -m32 -mpreferred-stack-boundary=5 -S -o exploitable3.S exploitable.c

clean:
	-rm -f exploitable exploitable_es
	-rm *.o *.S
