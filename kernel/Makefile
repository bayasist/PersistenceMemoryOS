default:
	make KImage
headers = api.h common.h gdt.h idt.h main.h memory.h screen.h app/app.h app/apiData.h int.h window.h mouse.h keyboard.h
objects = screen.o idt.o main.o common.o int_event.s.o memory.o gdt.o app/app.s.o app/app.o api.o api.s.o int.o window.o mouse.o keyboard.o

gdt.o:gdt.c $(headers)
	i686-pc-linux-gnu-gcc -c -o gdt.o gdt.c -O2 -Wall
	
kernel.o:kernel_main.c $(headers)
	i686-pc-linux-gnu-gcc -c -o kernel.o kernel_main.c -O2 -Wall

main.o:main.c $(headers)
	i686-pc-linux-gnu-gcc -c -o main.o main.c -O2 -Wall
	
screen.o:screen.c $(headers)
	i686-pc-linux-gnu-gcc -c -o screen.o screen.c -O2 -Wall
	
api.o:api.c $(headers)
	i686-pc-linux-gnu-gcc -c -o api.o api.c -O0 -Wall

idt.o:idt.c $(headers)
	i686-pc-linux-gnu-gcc -c -o idt.o idt.c -O2 -Wall

common.o:common.c $(headers)
	i686-pc-linux-gnu-gcc -c -o common.o common.c -O2 -Wall

memory.o:memory.c $(headers)
	i686-pc-linux-gnu-gcc -c -o memory.o memory.c -O2 -Wall

window.o:window.c $(headers)
	i686-pc-linux-gnu-gcc -c -o window.o window.c -O2 -Wall

mouse.o:mouse.c $(headers)
	i686-pc-linux-gnu-gcc -c -o mouse.o mouse.c -O2 -Wall

keyboard.o:keyboard.c $(headers)
	i686-pc-linux-gnu-gcc -c -o keyboard.o keyboard.c -O2 -Wall
	
int.o:int.c $(headers)
	i686-pc-linux-gnu-gcc -c -o int.o int.c -O2 -Wall

KImage:kernel.o linker-script.txt $(objects)
	i686-pc-linux-gnu-ld -T linker-script.txt -Map kernel.map -nostdlib -e kmain --oformat binary -o KImage kernel.o $(objects) /lib/gcc/i686-pc-linux-gnu/4.8.2/libgcc.a


int_event.s.o:int_event.s
	i686-pc-linux-gnu-as int_event.s -o int_event.s.o
	
app/app.s.o:app/app.s
	i686-pc-linux-gnu-as app/app.s -o app/app.s.o
	
api.s.o:api.s
	i686-pc-linux-gnu-as api.s -o api.s.o
	
app/app.o:app/app.c app/app.h $(headers)
	i686-pc-linux-gnu-gcc -c -o app/app.o app/app.c -O2 -Wall
	