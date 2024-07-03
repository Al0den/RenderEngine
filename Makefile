build:
	g++ -c ./src/*.cpp
	ar rcs renderer.a *.o
	mv renderer.a ./lib/librenderer.a
	rm *.o
