default:
	make test

main.o:main.c
	i686-pc-linux-gnu-gcc -c -o main.o main.c -O2 -Wall

test:main.o ../linker-script.txt ../api.o ../lib/string.o
	i686-pc-linux-gnu-ld -T ../linker-script.txt -Map kernel.map --oformat binary -o test main.o ../api.o ../lib/string.o
