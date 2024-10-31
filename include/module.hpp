#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include<assert.h>
#include<iostream>

#include "utils.hpp"

class Module {
    public:
        Module(BoundingBox box, SDL_Renderer *renderer);

        virtual ~Module() = default;

        virtual void onClickRight(RelativePosition clickPosition) {};
        virtual void onClickLeft(RelativePosition clickPosition) {};
        virtual void onDrag(RelativePosition dragStart, RelativePosition dragEnd, int numFrames) {  };
        virtual void whileDrag(RelativePosition dragStart, RelativePosition dragCurrent, int numFrames) {  };
        virtual void onKey(SDL_Keycode key) {};

        virtual void update() {};

        PercentPosition getPercentPosition(RelativePosition pos) const { return { (float)pos.pos_x / box.width, (float)pos.pos_y / box.height }; }
        RelativePosition relativeFromWorld(int world_x, int world_y) const { return { world_x - box.pos_x, world_y - box.pos_y }; }

        BoundingBox getBoundingBox() const { return box; };
        SDL_Texture* getTexture() const { return texture; }
        SDL_Renderer *getRenderer() const { return renderer; }
        int getPriority() const { return priority; }

    private:
        BoundingBox box;

        SDL_Texture *texture;
        SDL_Renderer *renderer;

        int priority = 0;
};

