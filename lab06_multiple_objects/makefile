LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

maze: maze.c initShader.o matrix_lib.o
	gcc -o maze maze.c initShader.o matrix_lib.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c
matrix_lib.o: matrix_lib.c matrix_lib.h
	gcc -c matrix_lib.c