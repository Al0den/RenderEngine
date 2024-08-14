#pragma once 

#include <SDL2/SDL.h>

namespace rend {
    typedef struct BoundingBox_s {
        int bounding_x;
        int bounding_y;
        int bounding_width;
        int bounding_height;
    } BoundingBox;

    class RenderObject {
        public:
            RenderObject();
            virtual ~RenderObject();
            
            virtual void render(void *render_engine) = 0;

            int getDrawPriority();
            void setDrawPriority(int priority);
            
            bool boundingBoxActivated();
            void toggleBoundingBox();
            void toggleBoundingBox(bool toggle);
            void setBoundingBox(int x, int y, int width, int height);
            BoundingBox getBoundingBox();
            
            bool clicksActivated();
            void toggleClick();
            void toggleClick(bool toggle);
            virtual void onClick(int x, int y, Uint8 button);

            virtual void updatePosition();

        private:
            int draw_priority;

            bool clicks;
            bool bounding_box;

            BoundingBox box;
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
