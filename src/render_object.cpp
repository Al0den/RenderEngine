#include "../include/render_object.hpp"
#include "../include/renderer.hpp"
#include <SDL_render.h>

#include <iostream>
#include <assert.h>

using namespace rend;

RenderObject::RenderObject() {
    draw_priority = 0;
    clicks = false;
    bounding_box = false;
}

RenderObject::~RenderObject() {}

void RenderObject::render(void *render_engine) {
    assert(render_engine != nullptr);
    std::cout << "Warning: Tried to render a RenderObject" << std::endl;
    return;
}

int RenderObject::getDrawPriority() {
    return draw_priority;
}

void RenderObject::setDrawPriority(int priority) {
    RenderEngine::lock.lock();
    draw_priority = priority;
    RenderEngine::lock.unlock();
}

void RenderObject::setBoundingBox(int x, int y, int width, int height) {
    bounding_box = true;
    box = {x, y, width, height};
}

void RenderObject::toggleClick() {
    clicks = !clicks;
}

void RenderObject::toggleClick(bool toggle) {
    clicks = toggle;
} 

void RenderObject::toggleBoundingBox() {
    bounding_box = !bounding_box;
}

void RenderObject::toggleBoundingBox(bool toggle) {
    bounding_box = toggle;
}

void RenderObject::onClick(int x, int y, Uint8 button) {
    assert(x >= 0 && y >= 0);
    return;
}

void RenderObject::customPositionUpdate() {
    return;
}

void RenderObject::updatePosition(int displacement_x, int displacement_y) {
    std::cout << displacement_x << " " << displacement_y << std::endl;
    return;
}

bool RenderObject::boundingBoxActivated() {
    return bounding_box;
}

bool RenderObject::clicksActivated() {
    return clicks;
}

BoundingBox RenderObject::getBoundingBox() {
    return box;
}

BallRenderer::BallRenderer(double *x, double *y, double rad, unsigned char red, unsigned char green, unsigned char blue, int draw_priority) : RenderObject() {
    this->x = x;
    this->y = y;
    this->rad = rad;
    this->red = red;
    this->blue = blue;
    this->green = green;
    setDrawPriority(draw_priority);
}

BallRenderer::~BallRenderer() {}

void BallRenderer::render(void *render_engine) {
    RenderEngine *engine = (RenderEngine*)render_engine;
    SDL_Renderer *renderer = engine->getRendererHandle();

    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    int local_x, local_y; 
    engine->posToLocal(*x, *y, &local_x, &local_y);
    double radius = rad * engine->getZoomFactor();
    for (int i=0; i<radius; i++) {
        for(int j=0; j<radius; j++) {
            if (i*i + j*j < radius*radius) {
                SDL_RenderDrawPoint(renderer, local_x + i, local_y + j);
                SDL_RenderDrawPoint(renderer, local_x - i, local_y + j);
                SDL_RenderDrawPoint(renderer, local_x + i, local_y - j);
                SDL_RenderDrawPoint(renderer, local_x - i, local_y - j);
            }
        }
    }
}

SpringRenderer::SpringRenderer(double *x1, double *y1, double *x2, double *y2, double width, unsigned char red, unsigned char green, unsigned char blue, int draw_priority) : RenderObject() {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->width = width;
    this->red = red;
    this->green = green;
    this->blue = blue;
    setDrawPriority(draw_priority);
}

SpringRenderer::~SpringRenderer() {}

void SpringRenderer::render(void *render_engine) {
    RenderEngine *engine = (RenderEngine*)render_engine;
    SDL_Renderer *renderer = engine->getRendererHandle();
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);

    int local_x1, local_y1, local_x2, local_y2;
    engine->posToLocal(*x1, *y1, &local_x1, &local_y1);
    engine->posToLocal(*x2, *y2, &local_x2, &local_y2);

    SDL_RenderDrawLine(renderer, local_x1, local_y1, local_x2, local_y2);
}
