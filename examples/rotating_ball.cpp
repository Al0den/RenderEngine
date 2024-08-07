#include "../include/RenderEngine"

#include <math.h>
#include <iostream>

int main() {
    rend::RenderEngine renderer(800, 600, REND_INFOBOX);

    rend::InfoBox *info_box = renderer.getInfoBox();
    auto plotter = std::make_unique<rend::Plotter>(20, 250, 400, 300);

    int num_points = 1000;
    double *x = (double*)malloc(sizeof(double) * num_points);
    double *y = (double*)malloc(sizeof(double) * num_points);
    
    for (int i=0; i<num_points; i++) {
        x[i] = 0;
        y[i] = 0;
    }

    plotter->plot(x, y, num_points, {255, 0, 0});
    plotter->setDynamic(true);
    plotter->setUpdateFrequency(1000);
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
    int i = 0;

    while(renderer.handleEvents()) {
        i++;
        theta += 0.01;
        ball_x = distance_to_center * cos(theta);
        ball_y = distance_to_center * sin(theta);

        info_box->setValue(0, 0, std::to_string(ball_x).substr(0, std::to_string(ball_x).find(".") + 2));
        info_box->setValue(0, 1, std::to_string(ball_y).substr(0, std::to_string(ball_y).find(".") + 2));
        info_box->setValue(1, 0, std::to_string(theta).substr(0, std::to_string(theta).find(".") + 4));
        
        for(int j=num_points - 1; j > 0; j--) {
            x[j] = x[j-1];
            y[j] = y[j-1];
        }
        x[0] = i;
        y[0] = cos(theta);

        if(theta > 2 * M_PI) {
            theta -= 2 * M_PI;
        }
        std::cout << x[0] << " " << y[0] << std::endl;

        SDL_Delay(10);
    }
}


