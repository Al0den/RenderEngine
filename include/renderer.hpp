#pragma once

//#define DEBUG

#include "/opt/homebrew/include/SDL2/SDL.h"

#include "../include/config.hpp"
#include "../include/containers.hpp"

#include <mutex>

namespace rend {
    class RenderEngine {
        public:
            RenderEngine(int width, int height, int config);
            ~RenderEngine();

            void renderFrame();
            double zoom_factor;

            void addBall(Ball b);
            void addSpring(Spring s);
            void addLine(Line l);

            void setZoomFactor(double zf);

            void startRenderLoop();
            bool handleEvents();

            std::mutex lock;

            SDL_Window *window;

            void *RenderLoop();

            void (*customRenderFunction)(SDL_Renderer *renderer, SDL_Window *window);
            void (*renderOverlapFunction)(SDL_Renderer *renderer, SDL_Window *window);
            
            double *offset_x;
            double *offset_y;
        private:

            bool grid;
            bool infoBox;

            bool renderLoop;

            int w;
            int h;

            int fps;

            SDL_Renderer *renderer;
            SDL_Surface *surface;
            SDL_Event e;

            Ball *balls;
            Spring *springs;
            Line *lines;      

            int num_balls;
            int num_springs;
            int num_lines;

            void drawBall(Ball b);
            void drawSpring(Spring s);
            void drawLine(Line l);

            void drawBackground();

            void posToLocal(double x, double y, int *local_x, int *local_y);


    };
}
