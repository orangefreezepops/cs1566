LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

texture: textureLab.c initShader.o
	gcc -o texture textureLab.c initShader.o $(LIBS)

initShader: initShader.c
	gcc -c initShader.c
