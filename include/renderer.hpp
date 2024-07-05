#pragma once
#define ALODEN_RENDER_ENGINE

//#define DEBUG

#include <SDL2/SDL.h>

#include "../include/config.hpp"
#include "../include/info_box.hpp"
#include "../include/render_object.hpp"

#include <mutex>
#include <vector>

namespace rend {
    class RenderEngine {
        public:
            RenderEngine(int width, int height, int config);
            ~RenderEngine();

            void renderFrame();

            void setZoomFactor(double zf);
            double getZoomFactor();
             
            void attachObject(std::unique_ptr<RenderObject> object);

            void startRenderLoop();
            bool handleEvents();

            void *RenderLoop();
            void setBackgroundColor(int red, int green, int blue, int alpha);
            void setGridColor(int red, int green, int blue, int alpha);

            SDL_Renderer *getRendererHandle();
            SDL_Window *getWindowHandle();
            SDL_Surface *getSurfaceHandle();

            InfoBox* getInfoBox();

            void posToLocal(double x, double y, int *local_x, int *local_y);

            bool isPaused();
            bool isHidingInfoBox();
            void togglePlay();
            void toggleInfoBox();
        private:
            std::mutex lock;

            SDL_Window *window;

            double zoom_factor;

            bool grid;
            bool renderLoop;

            double *offset_x;
            double *offset_y;

            bool paused;
            bool hide;

            int w;
            int h;

            int fps;

            SDL_Renderer *renderer;
            SDL_Surface *surface;
            SDL_Event e;
            InfoBox *info_box;

            std::vector<std::unique_ptr<RenderObject> > objects;

            void (*customRenderFunction)(SDL_Renderer *renderer, SDL_Window *window);
            void (*renderOverlapFunction)(SDL_Renderer *renderer, SDL_Window *window);

            void drawBackground();
            void renderAllObjects();

            void setCustomRenderFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window));
            void setCustomOverlapFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window));

            int background_color[4];
            int grid_color[4];
    };
}
