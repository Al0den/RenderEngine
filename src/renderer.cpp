#include "../include/renderer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <SDL_keycode.h>
#include <SDL_render.h>
#include <iostream>
#include <assert.h>

std::mutex rend::RenderEngine::lock = std::mutex();

using namespace rend;

RenderEngine::RenderEngine(int width, int height, int config) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Aloden - RenderEngine", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    surface = SDL_GetWindowSurface(window);
    zoom_factor = 1.0;

    h = height;
    w = width;

    offset_x = new double;
    offset_y = new double;
    
    *offset_x = 0;
    *offset_y = 0;
    
    grid = false;
    fps = 60;

    renderLoop = false;
    paused = false;
    hide = false;

    debug = config & REND_DEBUG;
    grid = config & REND_GRID;

    customRenderFunction = nullptr;
    renderOverlapFunction = nullptr;
    customSDLEventHandler = nullptr;

    mode = 0;

    startRenderLoop();

    setBackgroundColor(32, 32, 32, 255);
    setGridColor(242, 242, 242, 255);

    dragging = false;
}

RenderEngine::~RenderEngine() {
    lock.lock();
    renderLoop = false;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete offset_x;
    delete offset_y;
    lock.unlock();
}

void RenderEngine::renderFrame() {
    drawBackground();
    if(mode == 0) {
        renderAllObjects();
    } else if(mode == 1) {
        renderAllBoundingBoxes();
    }
}

void RenderEngine::renderAllBoundingBoxes() {
    SDL_SetRenderDrawColor(getRendererHandle(), 0, 255, 0, 255);
    for(int i=0; i<(int)bounding_objects.size(); i++) {
        BoundingBox box = bounding_objects[i]->getBoundingBox();
        SDL_Rect rect = {box.bounding_x, box.bounding_y, box.bounding_width, box.bounding_height};
        SDL_RenderDrawRect(getRendererHandle(), &rect);
    }
    return;
}

void RenderEngine::posToLocal(double x, double y, int *local_x, int *local_y) {
    *local_x = (int)((double)w / 2 + (x - *offset_x) * zoom_factor);
    *local_y = (int)((double)h / 2 - (y + *offset_y) * zoom_factor);
}

 void RenderEngine::setCustomRenderFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window)) {
     customRenderFunction = override_func;
}
void RenderEngine::setCustomOverlapFunction(void (override_func)(SDL_Renderer *renderer, SDL_Window *window)) {
    renderOverlapFunction = override_func;
}

void RenderEngine::setCustomSDLEventHandler(void (override_func)(SDL_Event *e)) {
    customSDLEventHandler = override_func;
}

void RenderEngine::setBackgroundColor(int red, int green, int blue, int alpha) {
    background_color[0] = red;
    background_color[1] = green;
    background_color[2] = blue;
    background_color[3] = alpha;
}

void RenderEngine::updateObjects() {
    click_objects.clear();
    bounding_objects.clear();

    for(int i=0; i<(int)objects.size(); i++) {
        if(objects[i]->boundingBoxActivated()) {
            bounding_objects.push_back(objects[i]);
        }
        if(objects[i]->clicksActivated()) {
            click_objects.push_back(objects[i]);
        }
    }
}

void RenderEngine::setGridColor(int red, int green, int blue, int alpha) {
    grid_color[0] = red;
    grid_color[1] = green;
    grid_color[2] = blue;
    grid_color[3] = alpha;
}

void RenderEngine::drawBackground() {
    SDL_SetRenderDrawColor(renderer, background_color[0], background_color[1], background_color[2], background_color[3]);
    SDL_RenderClear(renderer);

    if (grid) {
        SDL_SetRenderDrawColor(renderer, grid_color[0], grid_color[1], grid_color[2], grid_color[3]);
        int x_off = (int)*offset_x % 100;
        int y_off = (int)*offset_y % 100;

        for (int i=0; i<w + 100; i+=100) {
            SDL_RenderDrawLine(renderer, i + x_off, 0, i + x_off, h);
        }
        for (int i=0; i<h + 100; i+=100) {
            SDL_RenderDrawLine(renderer, 0, i + y_off, w, i + y_off);
        }
    }
}

void RenderEngine::attachObject(RenderObject *object) {
    if(object->clicksActivated()) {
        assert(object->boundingBoxActivated());
    }
    lock.lock();
    objects.push_back(object);
    if(object->clicksActivated()) {
        click_objects.push_back(object);
    }
    if(object->boundingBoxActivated()) {
        bounding_objects.push_back(object);
    }
    lock.unlock();
}

void RenderEngine::setZoomFactor(double zf) {
    lock.lock();
    zoom_factor = zf;
    lock.unlock();
}
 
double RenderEngine::getZoomFactor() {
    return zoom_factor;
}

bool inBoundingBox(int x, int y, BoundingBox box) {
    if(x > box.bounding_x && x < box.bounding_x + box.bounding_width && y > box.bounding_y && y < box.bounding_y + box.bounding_height) {
        return true;
    }
    return false;
}

bool RenderEngine::handleEvents() {
    if (!renderLoop) {
        return false;
    }
    bool still_dragging;
    RenderObject* objectDragged = nullptr;

    while(SDL_PollEvent(&e) != 0) {
        still_dragging = false;
        if(e.type == SDL_QUIT) {
            renderLoop = false;
        }

        if(customSDLEventHandler != nullptr) {
            customSDLEventHandler(&e);
        }

        if(e.type == SDL_MOUSEBUTTONDOWN) {
            for(int i=0; i<(int)click_objects.size(); i++) {
                click_objects[i]->onClick(e.motion.x, e.motion.y, e.button.button);
            }
            if(e.button.button == SDL_BUTTON_LEFT) {
                if(!dragging) {
                    start_x = e.motion.x;
                    start_y = e.motion.y; 
                    for(int i=0; i<(int)bounding_objects.size(); i++) {
                        if(inBoundingBox(start_x, start_y, objects[i]->getBoundingBox())) {
                            objectDragged = objects[i];
                            break;
                        }
                    }
                }

                if(objectDragged != nullptr) {
                    std::cout << "Started dragging" << std::endl;
                    still_dragging = true;
                    dragging = true;
                }

            }
        }

        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
            case SDLK_UP:
                *offset_y -= (double)w / 20 / zoom_factor;
                break;
            case SDLK_DOWN:
                *offset_y += (double)h / 20 / zoom_factor;
                break;
            case SDLK_LEFT:
                *offset_x -= (double)w / 20 / zoom_factor;
                break;
            case SDLK_RIGHT:
                *offset_x += (double)w / 20 / zoom_factor;
                break;
            case SDLK_EQUALS:
                zoom_factor *= 1.1;
                break;
            case SDLK_MINUS:
                zoom_factor /= 1.1;
                break;
            case SDLK_r:
                *offset_x = 0;
                *offset_y = 0;
                break;
            case SDLK_p:
                paused = !paused;
                break;
            case SDLK_h:
                hide = !hide;
                break;
            case SDLK_SPACE:
                mode++;
                if(mode > 1) {
                    mode = 0;
                }
            }
        }

        if(!still_dragging && dragging) {
            std::cout << "Finished dragging" << std::endl;
            assert(objectDragged != nullptr);
            // Finished dragging;
            int finish_x, finish_y;
            finish_x = e.motion.x;
            finish_y = e.motion.y;
            BoundingBox box = objectDragged->getBoundingBox();
            objectDragged->updatePosition(finish_x - start_x, finish_y - start_y);
            dragging = false;
            objectDragged = nullptr;
        }
    }
    lock.unlock();
    return renderLoop;
}

void *RenderEngine::RenderLoop() {
    while (renderLoop) {
        lock.lock();
        if (customRenderFunction != nullptr) {
            customRenderFunction(renderer, window);
        } else {
            SDL_GetWindowSize(window, &w, &h);
            renderFrame();
            if(renderOverlapFunction != nullptr) {
                renderOverlapFunction(renderer, window);
            }
            SDL_RenderPresent(renderer);
        }
        lock.unlock();
        SDL_Delay(1000/fps);
    }
    return NULL;
}

void RenderEngine::setFPS(int fps) {
    assert(fps > 0);
    this->fps = fps;
}

void RenderEngine::renderAllObjects() {
    int n = objects.size();
    int current_prio = 0;
    int next_prio = 0;
    while(next_prio < INT_MAX) {
        current_prio = next_prio;
        next_prio = INT_MAX;
        for(int i=0; i<n; i++) {
            if(objects[i] == nullptr) {
                if(debug) {
                    std::cerr << "Null pointer in objects" << std::endl;
                }
                continue;
            }
            if (objects[i]->getDrawPriority() == current_prio) {
                objects[i]->render(static_cast<void*>(this));
            } else if(objects[i]->getDrawPriority() < next_prio && objects[i]->getDrawPriority() > current_prio) {
                next_prio = objects[i]->getDrawPriority();
            }
        }
        if(next_prio == INT_MAX) {
            break;
        }
    }
}

static void* staticRenderLoop(void (*arg)) {
    RenderEngine *engine = static_cast<RenderEngine*>(arg);
    return engine->RenderLoop();
}

void RenderEngine::startRenderLoop() {
    if (renderLoop) return;
    renderLoop = true;
    pthread_t render_thread;
    pthread_create(&render_thread, NULL, staticRenderLoop, static_cast<void*>(this));
}

SDL_Renderer *RenderEngine::getRendererHandle() {
    return renderer;
}

SDL_Window *RenderEngine::getWindowHandle() {
    return window;
}

SDL_Surface *RenderEngine::getSurfaceHandle() {
    return surface;
}

bool RenderEngine::isPaused() {
    return paused;
}

void RenderEngine::togglePlay() {
    paused = !paused;
}

void RenderEngine::clearObjects() {
    lock.lock();
    objects.clear();
    lock.unlock();
}

int RenderEngine::getHeight() {
    return h;
}

int RenderEngine::getWidth() {
    return w;
}

int *RenderEngine::getBackgroundColor() {
    return background_color;
}
