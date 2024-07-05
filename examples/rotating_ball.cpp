#include <RenderEngine>

#include <math.h>

int main() {
    rend::RenderEngine renderer(800, 600, REND_INFOBOX);

    renderer.info_box->setRowCol(2, 2);
    renderer.info_box->setName(0, 0, "x");
    renderer.info_box->setName(0, 1, "y");
    renderer.info_box->setName(1, 0, "theta");

    double distance_to_center = 200;
    double ball_x = distance_to_center;
    double ball_y = 0;

    auto obj = std::make_unique<rend::BallRenderer>(&ball_x, &ball_y, 20.0, 255, 255, 255, 0); 

    renderer.attachObject(std::move(obj));
    
    double theta = 0.0;

    while(renderer.handleEvents()) {
        theta += 0.01;
        ball_x = distance_to_center * cos(theta);
        ball_y = distance_to_center * sin(theta);

        renderer.info_box->setValue(0, 0, std::to_string(ball_x).substr(0, std::to_string(ball_x).find(".") + 2));
        renderer.info_box->setValue(0, 1, std::to_string(ball_y).substr(0, std::to_string(ball_y).find(".") + 2));
        renderer.info_box->setValue(1, 0, std::to_string(theta).substr(0, std::to_string(theta).find(".") + 4));

        if(theta > 2 * M_PI) {
            theta -= 2 * M_PI;
        }

        SDL_Delay(10);
    }
}


