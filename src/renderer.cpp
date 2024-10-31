#include "../include/renderer.hpp"
#include <chrono>

RenderEngine::RenderEngine(int width, int height) {
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
}

RenderEngine::~RenderEngine() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool RenderEngine::renderFrame() {
    std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> timeDiff = currentTime - lastFrameTime;
    float timeDiffSeconds = timeDiff.count();

    if (timeDiffSeconds < 1.0f / framerate) {
        uint msToWait = (1.0f / framerate - timeDiffSeconds) * 1000;
        SDL_Delay(msToWait);
    }

    static int mouseX, mouseY;

    bool isClickedRight = false;
    bool isClickedLeft = false;

    SDL_Keycode keys[128] = {0};
    int keyIndex = 0;

    //Handle sdl events
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }

        if(e.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouseX, &mouseY);
        }

        if(e.type == SDL_MOUSEBUTTONUP) {
            if(e.button.button == SDL_BUTTON_LEFT) {
                isClickedRight = true;
            } else if(e.button.button == SDL_BUTTON_RIGHT) {
                isClickedLeft = true;
            }
        }

        //Get keystrokes from keyup
        if(e.type == SDL_KEYUP) {
            keys[keyIndex] = e.key.keysym.sym;
            keyIndex++;
        }
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Send click related events to modules
    if(isClickedRight || isClickedLeft) {
        std::cout << "Clicked at " << mouseX << ", " << mouseY << std::endl;
        for(int i=0; i<modules.size(); i++) {
            Module &module = *modules[i];
            BoundingBox box = module.getBoundingBox();
            if(!isInBoundingBox({mouseX, mouseY}, box)) continue;
            if(isClickedRight) module.onClickRight({mouseX - box.pos_x, mouseY - box.pos_y});
            if(isClickedLeft) module.onClickLeft({mouseX - box.pos_x, mouseY - box.pos_y});
        }
    }

    // Send keyup events to modules
    int rollingKeyIndex = 0;
    while(keys[rollingKeyIndex] != 0) {
        for(int i=0; i<modules.size(); i++) {
            Module &module = *modules[i];
            module.onKey(keys[rollingKeyIndex]);
        }
    }

    // Ask all modules to update;
    for(int i=0; i<modules.size(); i++) {
        Module &module = *modules[i];
        module.update();

        SDL_Texture* texture = module.getTexture();
        if (texture) {
            BoundingBox box = module.getBoundingBox();
            if(box.width <= 0 || box.height <= 0) continue;
            SDL_Rect destRect = { box.pos_x, box.pos_y, box.width, box.height };
            SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        }
    }

    SDL_RenderPresent(renderer);

    lastFrameTime = std::chrono::high_resolution_clock::now();
    numFrames++;
    return true;
}
