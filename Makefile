CC = clang 
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g

all: banhammer

banhammer: banhammer.o node.o ll.o ht.o speck.o bv.o bf.o parser.o
	$(CC) -o banhammer banhammer.o node.o ll.o ht.o speck.o bv.o bf.o parser.o -lm
node.o: node.c
	$(CC) $(CFLAGS) -c node.c
ll.o: ll.c
	$(CC) $(CFLAGS) -c ll.c
ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c
speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c
bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c
bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c
parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c
banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

clean:
	rm -f banhammer banhammer.o node.o ll.o ht.o speck.o bv.o bf.o parser.o

scan-build: clean
	scan-build make   
	
format: 
	clang-format -i -style=file banhammer.c node.c node.h ll.c ll.h ht.c ht.h speck.c speck.h bv.c bv.h bf.c bf.h parser.c parser.h 
