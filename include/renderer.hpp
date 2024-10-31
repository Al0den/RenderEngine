#pragma once

#define ALODEN_RENDER_ENGINE

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/config.hpp"
#include "../include/module.hpp"

#include <mutex>
#include <vector>
#include <iostream>
#include <chrono>

class RenderEngine {
    public:
        RenderEngine(int width, int height);
        ~RenderEngine();

        std::mutex lock;

        bool renderFrame(); // Return false if the renderer has stopped rendering

        void setFramerate(float framerate) { this->framerate = framerate; };
        void setFramelock(bool frameLocked) { this->frameLocked = frameLocked; };
        void toggleFramelock() { this->frameLocked = !this->frameLocked; };

        int getFramecount() const { return numFrames; }

        void addModule(std::shared_ptr<Module> module) { modules.push_back(module); }

        SDL_Renderer *getRenderer() { return renderer; }

    private:
        bool frameLocked = true;
        float framerate = 60.0f;

        bool rendering = false;
        int numFrames = 0;

        std::vector<std::shared_ptr<Module>> modules{};

        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Event e;

        // Keep the time of the previously rendered frame
        
        std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
};
