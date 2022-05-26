all:task0 task1 task2 task3 patch
task0.o:task0.c 
	gcc -g -m32 -c -o task0.o task0.c
task0: task0.o 
	gcc -m32 -o task0 task0.o
task1.o:task1.c 
	gcc -g -m32 -c -o task1.o task1.c
task1: task1.o 
	gcc -m32 -o task1 task1.o 
task2.o:task2.c 
	gcc -g -m32 -c -o task2.o task2.c
task2: task2.o 
	gcc -m32 -o task2 task2.o 
task3.o:task3.c 
	gcc -g -m32 -c -o task3.o task3.c
task3: task3.o 
	gcc -m32 -o task3 task3.o
patch.o:patch.c 
	gcc -g -m32 -c -o patch.o patch.c
patch: patch.o 
	gcc -m32 -o patch patch.o 

.PHONY: clean
clean:
	 rm -rf ./*.o task0
	 rm -rf ./*.o task1
	 rm -rf ./*.o task2
	 rm -rf ./*.o task3
	 rm -rf ./*.o patch
