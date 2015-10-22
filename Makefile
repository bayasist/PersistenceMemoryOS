

default:
	make -C kernel
	make -C kernel\app
	make a.zip

BootLoader: 32bit.asm boot.asm gdt.asm gdtData.asm main.asm print.asm
	nasm main.asm -o BootLoader

a.zip: BootLoader kernel\KImage kernel\default.font kernel\app\test\test kernel\app\test2\test kernel\app\test3\test kernel\app\test4\test 
	.\Join.exe -o a.zip map.txt


full:
	make -C kernel
	make -C kernel\app\test
	make a.zip
	