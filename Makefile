compilet: buildt run

compile: build run

build:
	windres resources/resource.rc -O coff -o resources/resource.res
	gcc -mwindows -I include -L lib -o main src/main.c resources/resource.res -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf

buildt:
	windres resources/resource.rc -O coff -o resources/resource.res
	gcc -g -I include -L lib -o main src/main.c resources/resource.res -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf

run:
	./main.exe
