a.out:main.o func.o
	gcc -o a.out main.o func.o
	make clean
main.o:
func.o:
.PHONY:clean
clean:
	rm -rf main.o func.o
