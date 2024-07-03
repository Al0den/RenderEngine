#pragma once

namespace rend {
    struct Spring {
        double *x1;
        double *y1;
        double *x2;
        double *y2;
        double rest_length;

        double width;

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        int draw_priority;
    };

    struct Line {
        double *x1;
        double *y1;
        double *x2;
        double *y2;

        double width;

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        int draw_priority;
    };

    struct Ball {
        double *center_x;
        double *center_y;
        double radius;

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        int draw_priority;
    };

    typedef struct Spring Spring;
    typedef struct Line Line;
    typedef struct Ball Ball;
}
