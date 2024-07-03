#pragma once

//#define DEBUG

#include <SDL2/SDL.h>

#include "../include/config.hpp"
#include "../include/containers.hpp"
#include "../include/info_box.hpp"

#include <mutex>
#include <vector>

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

            void setBackgroundColor(int red, int green, int blue, int alpha);
            void setGridColor(int red, int green, int blue, int alpha);
            
            double *offset_x;
            double *offset_y;

            InfoBox *info_box;

            bool paused;
            bool hide;
        private:
            bool grid;

            bool renderLoop;

            int w;
            int h;

            int fps;

            SDL_Renderer *renderer;
            SDL_Surface *surface;
            SDL_Event e;
            
            std::vector<Ball> balls;
            std::vector<Spring> springs;
            std::vector<Line> lines;

            void drawBall(Ball b);
            void drawSpring(Spring s);
            void drawLine(Line l);

            void drawBackground();

            void posToLocal(double x, double y, int *local_x, int *local_y);

            int background_color[4];
            int grid_color[4];
    };
}
