PROGRAM = izvrsni
verzija = 1

$(PROGRAM): $(verzija).o
	gcc $(verzija).o -lglut -lGLU -lGL -lm -o $(PROGRAM)

$(verzija).o: $(verzija).c
	gcc -c $(verzija).c
	
clean:
	rm *o $(PROGRAM)
	
clean_o:
	rm *o
