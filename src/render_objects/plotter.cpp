#include "../../include/render_objects/plotter.hpp"
#include "../../include/renderer.hpp"

#include <iostream>
#include <assert.h>

using namespace rend;

Plotter::Plotter(int x, int y, int width, int height) : RenderObject() {
    plots = std::vector<plot_t>();

    axis_color.r = 255;
    axis_color.g = 255;
    axis_color.b = 255;
    
    display_x = x;
    display_y = y;
    this->width = width;
    this->height = height;

    x_display_round = 2;
    y_display_round = 2;
        
    x_padding = 70;
    y_padding = 30;

    relative = false;

    font = TTF_OpenFont("/Users/alois/Desktop/projects/render_engine/include/fonts/CourierPrime-Bold.ttf", 16);
    if (font == nullptr) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface* surface = TTF_RenderText_Blended(font, "W", color);
    
    if (surface == NULL) {
        printf("Failed to render text for char width measurement. SDL_ttf Error: %s\n", TTF_GetError());
    }

    char_width = surface->w;
    SDL_FreeSurface(surface);
    plot_texture = nullptr;

    heights = new bool[4000];
    widths = new bool[4000];

    toggleBoundingBox(true);
    setBoundingBox(x, y, width, height);
}

Plotter::~Plotter() {
    if (plot_texture != nullptr) {
        SDL_DestroyTexture(plot_texture);
    }
    delete[] heights;
    delete[] widths;
}

int Plotter::plot(double *x, double *y, int num_values, SDL_Color color) {
    plot_t new_plot;
    new_plot.x = std::vector<double>();
    new_plot.y = std::vector<double>();
    for(int i=0; i<num_values; i++) {
        new_plot.x.push_back(x[i]);
        new_plot.y.push_back(y[i]);
    }
    new_plot.color = color;
    new_plot.link_points = true;
    plots.push_back(new_plot);
    update = true;
    return (int)(plots.size() - 1);
}

int Plotter::scatter(double *x, double *y, int num_values, SDL_Color color) {
    plot_t new_plot;
    new_plot.x = std::vector<double>();
    new_plot.y = std::vector<double>();
    for(int i=0; i<num_values; i++) {
        new_plot.x.push_back(x[i]);
        new_plot.y.push_back(y[i]);
    }
    new_plot.color = color;
    new_plot.link_points = false;
    plots.push_back(new_plot);
    update = true;
    return (int)(plots.size() - 1);
}

void Plotter::drawAxis(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, axis_color.r, axis_color.g, axis_color.b, 255);

    int line_start_x, line_start_y;
    int line_end_x, line_end_y;
    line_start_x = x_padding;
    line_start_y = y_padding;
    line_end_x = x_padding;
    line_end_y = height - y_padding;
    
    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);

    int arrow_length = 10;
    double theta = - M_PI * 5 / 3;
    
    line_end_y = y_padding + arrow_length * sin(theta);
    line_end_x = x_padding + arrow_length * cos(theta);

    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);

    theta = - M_PI * 4 / 3;
    line_end_y = y_padding + arrow_length * sin(theta);
    line_end_x = x_padding + arrow_length * cos(theta);

    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);

    line_start_x = x_padding;
    line_start_y = height - y_padding;
    line_end_x = width - x_padding;
    line_end_y = height - y_padding;

    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);

    theta = M_PI / 6;
    line_start_y = height - y_padding - arrow_length * sin(theta);
    line_start_x = width - x_padding - arrow_length * cos(theta);

    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);

    theta = M_PI * 11 / 6;
    line_start_y = height - y_padding - arrow_length * sin(theta);
    line_start_x = width - x_padding - arrow_length * cos(theta);

    SDL_RenderDrawLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y);
}

double round_nth_decimal(double value, int n) {
    return round(value * pow(10, n)) / pow(10, n);
};

void Plotter::drawAxisText(SDL_Renderer *renderer, double max_x, double max_y, double min_x, double min_y) {
    memset(heights, 0, 4000 * sizeof(bool));
    memset(widths, 0, 4000 * sizeof(bool));


    for(int i=0; i<(int)plots.size(); i++) {
        for(int j=0; j<(int)plots[i].x.size(); j++) {
            int x, y;

            x = x_padding - char_width;
            if(min_y != max_y) {
                y = height - y_padding - ((plots[i].y[j] - min_y) / (max_y - min_y)) * (height - 2 * y_padding);
            } else {
                y = height - y_padding;
            }
            std::string text_before_cut = std::to_string(round_nth_decimal(plots[i].y[j], y_display_round));
            int cut_num = y_display_round > 0 ? y_display_round + 1 : 0;
            std::string text = text_before_cut.substr(0, text_before_cut.find(".") + cut_num);
            SDL_Color textColor = {255, 255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (surface == NULL) {
                printf("Failed to render text for char width measurement. SDL_ttf Error: %s\n", TTF_GetError());
                return;
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == NULL) {
                printf("Failed to create texture from surface. SDL Error: %s\n", SDL_GetError());
                return;
            }
            SDL_Rect rect = {x - surface->w, y - surface->h / 2, surface->w, surface->h};
            // Check that no text is drawn on top of another text
            bool overlap = false;
            for(int i=0; i<surface->h; i++) {
                if(heights[y + i]) {
                    overlap = true;
                    break;
                }
            }
            if(!overlap) {
                for(int i=-(int)(char_width/2); i<surface->h + (int)(char_width/2); i++) {
                    heights[y + i] = true;
                }
                rect = {x - surface->w, y - surface->h / 2, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x_padding, y, x_padding - 5, y);
            }
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            if(min_x != max_x) {
                x = x_padding + ((plots[i].x[j] - min_x) / (max_x - min_x)) * (width - 2 * x_padding);
            } else {
                x = x_padding;
            }
            y = height - y_padding + char_width;

            text_before_cut = std::to_string(round_nth_decimal(plots[i].x[j], x_display_round));
            cut_num = x_display_round > 0 ? x_display_round + 1 : 0;
            text = text_before_cut.substr(0, text_before_cut.find(".") + cut_num);
            surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (surface == NULL) {
                printf("Failed to render text for char width measurement. SDL_ttf Error: %s\n", TTF_GetError());
                return;
            }
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == NULL) {
                printf("Failed to create texture from surface. SDL Error: %s\n", SDL_GetError());
                return;
            }
            overlap = false;
            for(int i=0; i<surface->w; i++) {
                if(widths[x + i]) {
                    overlap = true;
                    break;
                }
            }
            if (!overlap) {
                for (int i=-(int)(char_width/2); i<surface->w + (int)(char_width/2); i++) {
                    widths[x + i] = true;
                }
                rect = {x - surface->w / 2, y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x, height - y_padding, x, height - y_padding + 5);
            }
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }
}

void Plotter::drawPoints(SDL_Renderer *renderer) {
    double max_x = -INFINITY;
    double max_y = -INFINITY;
    double min_x = INFINITY;
    double min_y = INFINITY;

    for (int i=0; i<(int)plots.size(); i++) {
        for(int j=0; j<(int)plots[i].x.size(); j++) {
            if(plots[i].x[j] > max_x) {
                max_x = plots[i].x[j];
            }
            if(plots[i].x[j] < min_x) {
                min_x = plots[i].x[j];
            }
            if(plots[i].y[j] > max_y) {
                max_y = plots[i].y[j];
            }
            if(plots[i].y[j] < min_y) {
                min_y = plots[i].y[j];
            }
        }
    }
    if(max_x == -INFINITY || min_x == INFINITY || min_y == INFINITY || max_y == -INFINITY) {
        return;
    }

    if(max_x > 0) {
        max_x *= 1.1;
    } else {
        max_x *= 0.9;
    }

    if(max_y > 0) {
        max_y *= 1.1;
    } else {
        max_y *= 0.9;
    }

    assert(min_x <= max_x);
    assert(min_y <= max_y);

    int initial_x = x_padding;
    int initial_y = height - y_padding;

    int prev_x, prev_y;

    for(int i=0; i<(int)plots.size(); i++) {
        plot_t current_plot = plots[i];
        SDL_SetRenderDrawColor(renderer, current_plot.color.r, current_plot.color.g, current_plot.color.b, 255);
        for (int j=0; j<(int)current_plot.y.size(); j++) {
            int x, y;
            if(max_x == min_x) {
                x = initial_x + width/2;
                y = initial_y + height/2;
            } else {
                x = initial_x + (current_plot.x[j] - min_x) / (max_x - min_x) * (width - 2 * x_padding);
                y = initial_y - (current_plot.y[j] - min_y) / (max_y - min_y) * (height - 2 * y_padding);
            }
            if(j > 0 && current_plot.link_points) { 
                SDL_RenderDrawLine(renderer, prev_x, prev_y, x, y);
            }
            prev_x = x;
            prev_y = y;

            SDL_Rect rect = {x-2, y-2, 4, 4};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    drawAxisText(renderer, max_x, max_y, min_x, min_y);
}
void Plotter::updatePlotTexture(void *render_engine) {
    RenderEngine *engine = (RenderEngine*)render_engine;
    SDL_Renderer *renderer = engine->getRendererHandle();
    SDL_SetRenderTarget(renderer, plot_texture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    drawPoints(renderer);
    drawAxis(renderer);

    SDL_SetRenderTarget(renderer, NULL);
}

void Plotter::render(void *render_engine) {
    RenderEngine *engine = (RenderEngine*)render_engine;
    if((int)plots.size() < 1) {
        return;
    }
    if(plot_texture == nullptr) {
        plot_texture = SDL_CreateTexture(engine->getRendererHandle(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        update = true;
    }
    if(update) {
        updatePlotTexture(render_engine);
    }

    SDL_Rect rect = {display_x, display_y, width, height};

    SDL_SetTextureBlendMode(plot_texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(engine->getRendererHandle(), plot_texture, NULL, &rect);
}

void Plotter::addValue(int plot_id, double x, double y) {
    RenderEngine::lock.lock();
    plots[plot_id].x.push_back(x);
    plots[plot_id].y.push_back(y);
    RenderEngine::lock.unlock();
    update = true;
}

void Plotter::removePlot(int plot_id) {
    RenderEngine::lock.lock();
    plots.erase(plots.begin() + plot_id);
    RenderEngine::lock.unlock();
    update = true;
}

void Plotter::setAxisColor(SDL_Color color) {
    axis_color = color;
}

void Plotter::setXDisplayRound(int x) {
    x_display_round = x;
}

void Plotter::setYDisplayRound(int y) {
    y_display_round = y;
}

void Plotter::setXPadding(int pad) {
    x_padding = pad;
}

void Plotter::setYPadding(int pad) {
    y_padding = pad;
}
