build:
	g++ -c ./src/*.cpp -std=c++17
	ar rcs renderer.a *.o
	mv renderer.a ./lib/librenderer.a
	rm *.o
rotating_ball:
	make -B
	g++ -lrenderer -lSDL2 -lSDL2_ttf -std=c++17 examples/rotating_ball.cpp -o ./build/rotating_ball.out
