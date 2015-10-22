default:
	make api.o
	make lib/string.o
	make -C test
	make -C test2
	make -C test3
	make -C test4
	
api.o:api.c api.h
	i686-pc-linux-gnu-gcc -c -o api.o api.c -O0 -Wall
	
lib/string.o:lib/string.c lib/stdio.h
	i686-pc-linux-gnu-gcc -c -o lib/string.o lib/string.c -O2 -Wall