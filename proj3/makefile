LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

shadow_lighting: shadow_lighting.c initShader.o matrix_lib.o
	gcc -o shadow shadow_lighting.c initShader.o matrix_lib.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c
matrix_lib.o: matrix_lib.c matrix_lib.h
	gcc -c matrix_lib.c
