#pragma once

#include "../render_object.hpp"

#include <SDL2/SDL_ttf.h>
#include <vector>

namespace rend {
    typedef struct Plot {
        std::vector<double> x;
        std::vector<double> y;

        SDL_Color color;
        bool link_points;
    } plot_t;


    class Plotter : public RenderObject {
        public:
            Plotter(int x, int y, int width, int height);
            virtual ~Plotter();

            virtual void render(void *render_engine) override;

            int plot(double *x, double *y, int num_values, SDL_Color color);
            int scatter(double *x, double *y, int num_values, SDL_Color color);

            void setAxisColor(SDL_Color color);
            void setXDisplayRound(int x);
            void setYDisplayRound(int y);
            void setXPadding(int pad);
            void setYPadding(int pad);

            void addValue(int plot_id, double x, double y);
            void removePlot(int plot_id);

        private:
            std::vector<plot_t> plots;

            int x_display_round;
            int y_display_round;
            
            SDL_Color axis_color;

            int display_x, display_y;
            int width;
            int height;

            bool relative; // true for relative, false for fixed
            bool update;

            int x_padding;
            int y_padding;

            int char_width;

            TTF_Font *font;
            SDL_Texture *plot_texture;

            void drawAxis(SDL_Renderer *renderer);
            void drawPoints(SDL_Renderer *renderer);
            void drawAxisText(SDL_Renderer *renderer, double max_x, double max_y, double min_x, double min_y);
            void updatePlotTexture(void *render_engine);

            bool *heights;
            bool *widths;
    };
}
