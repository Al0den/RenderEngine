# Render Engine

This is an all purpose render engine, built only using SDL2 (And SDL\_ttf). 
It contains some basic features, all extensible and overridable.

By default, this renderer already runs in a separate thread.

# How to use

This requires the use of the C++14 standard (For the use of vectors)

Simply clone the reposity, and run `make -B`. Then, in any project, include the `include/RenderEngine"` file. When linking, you will need to link the library built inside `lib`, SDL2 and SDL\_ttf

Another option is to recompile the whole code, and simply include it (Still linking SDL2 and SDL\_ttf though) 

# Examples

With the following code, we should obtain a ball moving in circles, its `cos(theta)` relative to `i` graph, and an Info Box:
```cpp
#include "../include/RenderEngine"

#include <math.h>

int main() {
    rend::RenderEngine renderer(800, 600, 0);

    rend::InfoBox *info_box = new rend::InfoBox(2, 2, 15, 15);
    rend::Plotter *plotter = new rend::Plotter(20, 250, 400, 300);

    int pid = plotter->plot(nullptr, nullptr, 0, {255, 0, 0});

    plotter->setDynamic(true);
    plotter->setUpdateFrequency(100);
    plotter->setDrawPriority(1);

    renderer.attachObject(plotter);

    info_box->setRowCol(2, 2);
    info_box->setName(0, 0, "x");
    info_box->setName(0, 1, "y");
    info_box->setName(1, 0, "theta");

    renderer.attachObject(info_box);

    double distance_to_center = 200;
    double ball_x = distance_to_center;
    double ball_y = 0;

    rend::BallRenderer *ball = new rend::BallRenderer(&ball_x, &ball_y, 20.0, 255, 255, 255, 0); 

    renderer.attachObject(ball);
    
    double theta = 0.0;
    int i = 0;

    while(renderer.handleEvents()) {
        i++;
        theta += 0.01;
        ball_x = distance_to_center * cos(theta);
        ball_y = distance_to_center * sin(theta);

        info_box->setValue(0, 0, std::to_string(ball_x).substr(0, std::to_string(ball_x).find(".") + 2));
        info_box->setValue(0, 1, std::to_string(ball_y).substr(0, std::to_string(ball_y).find(".") + 2));
        info_box->setValue(1, 0, std::to_string(theta).substr(0, std::to_string(theta).find(".") + 4));

        if(i % 10 == 0) {
            plotter->addValue(pid, i, cos(theta));
        }

        if(theta > 2 * M_PI) {
            theta -= 2 * M_PI;
        }

        SDL_Delay(10);
    }
}
```

![image](https://github.com/user-attachments/assets/d84b9923-5327-4d6f-98e2-1094198c833d)



