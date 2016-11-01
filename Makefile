project: diff.o queue.o
	gcc diff.o queue.o -o project
diff.o: diff.c diff.h queue.h
	gcc diff.c -c -Wall
queue.o: queue.c queue.h data.h
	gcc queue.c -c -Wall
clean:
	rm *.o
