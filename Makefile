build:
	g++ -c ./src/*.cpp -std=c++17
	ar rcs renderer.a *.o
	mv renderer.a ./lib/librenderer.a
	rm *.o
