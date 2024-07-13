#include <RenderEngine>

#include <math.h>
#include <iostream>

int main() {
    rend::RenderEngine renderer(800, 600, REND_INFOBOX);

    rend::InfoBox *info_box = renderer.getInfoBox();
    auto plotter = std::make_unique<rend::Plotter>(20, 250, 400, 300);

    int num_points = 30;
    double *x = (double*)malloc(sizeof(double) * num_points);
    double *y = (double*)malloc(sizeof(double) * num_points);
    
    for (int i=0; i<num_points; i++) {
        x[i] = i;
        y[i] = i * i;
    }

    plotter->plot(x, y, num_points, {255, 0, 0});
    plotter->draw_priority = 10;

    renderer.attachObject(std::move(plotter));

    info_box->setRowCol(2, 2);
    info_box->setName(0, 0, "x");
    info_box->setName(0, 1, "y");
    info_box->setName(1, 0, "theta");

    double distance_to_center = 200;
    double ball_x = distance_to_center;
    double ball_y = 0;

    auto ball = std::make_unique<rend::BallRenderer>(&ball_x, &ball_y, 20.0, 255, 255, 255, 0); 
    renderer.attachObject(std::move(ball));
    
    double theta = 0.0;

    while(renderer.handleEvents()) {
        theta += 0.01;
        ball_x = distance_to_center * cos(theta);
        ball_y = distance_to_center * sin(theta);

        info_box->setValue(0, 0, std::to_string(ball_x).substr(0, std::to_string(ball_x).find(".") + 2));
        info_box->setValue(0, 1, std::to_string(ball_y).substr(0, std::to_string(ball_y).find(".") + 2));
        info_box->setValue(1, 0, std::to_string(theta).substr(0, std::to_string(theta).find(".") + 4));
        if(theta > 2 * M_PI) {
            theta -= 2 * M_PI;
        }

        SDL_Delay(10);
    }
}


