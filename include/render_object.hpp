#pragma once 

#include <SDL2/SDL.h>

namespace rend {
    class RenderObject {
        public:
            RenderObject();
            virtual ~RenderObject();
            
            virtual void render(void *render_engine) = 0;

            int getDrawPriority();
            void setDrawPriority(int priority);

        private:
            int draw_priority;
    };

    class BallRenderer : public RenderObject {
        public:
            BallRenderer(double *x, double *y, double rad, unsigned char red, unsigned char green, unsigned char blue, int draw_priority);
            virtual ~BallRenderer();
            
            virtual void render(void *render_engine) override;

        private:

            double *x;
            double *y;
            double rad;
            
            unsigned char red;
            unsigned char green;
            unsigned char blue;

    };

    class SpringRenderer : public RenderObject {
        public:
            SpringRenderer(double *x1, double *y1, double *x2, double *y2, double width, unsigned char red, unsigned char green, unsigned char blue, int draw_priority);
            virtual ~SpringRenderer();

            virtual void render(void *render_engine) override;

        private:
            double *x1;
            double *y1;
            double *x2;
            double *y2;
            
            double width;
            
            unsigned char red, green, blue;

    };
}
