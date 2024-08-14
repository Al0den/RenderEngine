#pragma once
#define ALODEN_RENDER_ENGINE

//#define DEBUG

#include <SDL2/SDL.h>

#include "../include/config.hpp"
#include "../include/render_object.hpp"

#include <mutex>
#include <vector>

namespace rend {
    class RenderEngine {
        public:
            static std::mutex lock;

            RenderEngine(int width, int height, int config);
            ~RenderEngine();

            void renderFrame();

            void setZoomFactor(double zf);
            double getZoomFactor();
             
            void attachObject(RenderObject *object);

            void startRenderLoop();
            bool handleEvents();

            void *RenderLoop();
            void setBackgroundColor(int red, int green, int blue, int alpha);
            void setGridColor(int red, int green, int blue, int alpha);
            void setFPS(int fps);

            void setCustomRenderFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window));
            void setCustomOverlapFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window));
            void setCustomSDLEventHandler(void (override_func)(SDL_Event *e));

            SDL_Renderer *getRendererHandle();
            SDL_Window *getWindowHandle();
            SDL_Surface *getSurfaceHandle();

            int getHeight();
            int getWidth();
            int *getBackgroundColor();

            void posToLocal(double x, double y, int *local_x, int *local_y);

            bool isPaused();
            void togglePlay();

            void clearObjects();
            void updateObjects();
        private:
            SDL_Window *window;

            double zoom_factor;

            bool grid;
            bool renderLoop;

            double *offset_x;
            double *offset_y;

            bool paused;
            bool hide;
            bool debug;

            int w;
            int h;

            int fps;

            SDL_Renderer *renderer;
            SDL_Surface *surface;
            SDL_Event e;

            std::vector<RenderObject*> objects;
            std::vector<RenderObject*> click_objects;
            std::vector<RenderObject*> bounding_objects;

            void (*customRenderFunction)(SDL_Renderer *renderer, SDL_Window *window);
            void (*renderOverlapFunction)(SDL_Renderer *renderer, SDL_Window *window);
            void (*customSDLEventHandler)(SDL_Event *e);

            void drawBackground();
            void renderAllObjects();
            void renderAllBoundingBoxes();

            int background_color[4];
            int grid_color[4];
            
            // 0 - Base renderer, 1 - Show bounding boxes
            int mode;
    };
}
