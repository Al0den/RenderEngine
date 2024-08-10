#include "../include/info_box.hpp"
#include "../include/renderer.hpp"

#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <iostream>
#include <assert.h>

using namespace rend;

InfoBox::InfoBox(int num_row, int num_col, int x, int y) {
    this->x = x;
    this->y = y;

    num_cols = num_col;
    rows_per_col = num_row;
    row_width = 0;

    fixed_row_width = false;

    info_box_names = new string*[num_cols];
    info_box_values = new string*[num_cols];

    for (int i=0; i<num_cols; i++) {
        info_box_names[i] = new string[rows_per_col];
        info_box_values[i] = new string[rows_per_col];
        
        for (int j=0; j<rows_per_col; j++) {
            info_box_names[i][j] = "Empty";
            info_box_values[i][j] = "Void";
        }
    }

    font = TTF_OpenFont("/Users/alois/Desktop/projects/render_engine/include/fonts/CourierPrime-Bold.ttf", 16);

    if (font == nullptr) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }

    lock = &(rend::RenderEngine::lock);
}

InfoBox::~InfoBox() {
    if (info_box_names != nullptr) {
        for(int i=0; i<num_cols; i++) {
            delete[] info_box_names[i];
        }
    }
    if (info_box_values != nullptr) {
        for (int i=0; i<num_cols; i++) {
            delete[] info_box_values[i];
        }
    }
    delete[] info_box_names;
    delete[] info_box_values;
}

int InfoBox::addRow() {
    lock->lock();
    if(rows_per_col == 0 || num_cols == 0) {
        lock->unlock();
        return -1;
    }

    rows_per_col++;
    string **new_names = new string*[num_cols];
    string **new_values = new string*[num_cols];

    for (int i=0; i<num_cols; i++) {
        new_names[i] = new string[rows_per_col];
        new_values[i] = new string[rows_per_col];

        std::memcpy(new_names[i], info_box_names[i], (rows_per_col-1)*sizeof(string));
        std::memcpy(new_values[i], info_box_values[i], (rows_per_col-1)*sizeof(string));

        delete[] info_box_names[i];
        delete[] info_box_values[i];
    }

    delete[] info_box_names;
    delete[] info_box_values;

    info_box_names = new_names;
    info_box_values = new_values;
    
    lock->unlock();
    return rows_per_col;
}

int InfoBox::addCol() {
    lock->lock();
    if(rows_per_col == 0 || num_cols == 0) {
        lock->unlock();
        return -1;
    }

    num_cols++;
    string **new_names = new string*[num_cols];
    string **new_values = new string*[num_cols];

    for (int i=0; i<num_cols; i++) {
        new_names[i] = new string[rows_per_col];
        new_values[i] = new string[rows_per_col];
        if (i < num_cols - 1) {
            std::memcpy(new_names[i], info_box_names[i], rows_per_col*sizeof(string));
            std::memcpy(new_values[i], info_box_values[i], rows_per_col*sizeof(string));
        }

        delete[] info_box_names[i];
        delete[] info_box_values[i];
    }

    delete[] info_box_names;
    delete[] info_box_values;

    info_box_names = new_names;
    info_box_values = new_values;

    lock->unlock();
    return num_cols;
}

int InfoBox::removeRow() {
    lock->lock();
    if (rows_per_col == 0 || num_cols == 0) {
        lock->unlock();
        return -1;
    }
    rows_per_col--;
    string **new_names = new string*[num_cols];
    string **new_values = new string*[num_cols];

    for (int i=0; i<num_cols; i++) {
        new_names[i] = new string[rows_per_col];
        new_values[i] = new string[rows_per_col];

        std::memcpy(new_names[i], info_box_names[i], rows_per_col*sizeof(string));
        std::memcpy(new_values[i], info_box_values[i], rows_per_col*sizeof(string));

        delete[] info_box_names[i];
        delete[] info_box_values[i];
    }
    delete[] info_box_names;
    delete[] info_box_values;

    info_box_names = new_names;
    info_box_values = new_values;
    
    lock->unlock();
    return rows_per_col;
}

void InfoBox::setRowCol(int row, int col) {
    while (rows_per_col < row) {
        addRow();
    }
    while (num_cols < col) {
        addCol();
    }
    while (rows_per_col > row) {
        removeRow();
    }
    while (num_cols > col) {
        removeCol();
    }
}

int InfoBox::removeCol() {
    lock->lock();
    if(rows_per_col == 0 || num_cols == 0) {
        lock->unlock();
        return -1;
    }

    num_cols--;
    string **new_names = new string*[num_cols];
    string **new_values = new string*[num_cols];

    for (int i=0; i<num_cols; i++) {
        new_names[i] = new string[rows_per_col];
        new_values[i] = new string[rows_per_col];
        std::memcpy(new_names[i], info_box_names[i], rows_per_col*sizeof(string));
        std::memcpy(new_values[i], info_box_values[i], rows_per_col*sizeof(string));
        delete[] info_box_names[i];
        delete[] info_box_values[i];
    }

    delete[] info_box_names;
    delete[] info_box_values;

    info_box_names = new_names;
    info_box_values = new_values;

    lock->unlock();

    return num_cols;
}

void InfoBox::setName(int row, int col, std::string name) {
    lock->lock();
    if (col >= num_cols || row >= rows_per_col) {
        lock->unlock();
        return;
    }
    info_box_names[col][row] = name;
    lock->unlock();
}

void InfoBox::setValue(int row, int col, string value) {
    if (col >= num_cols || row >= rows_per_col) {
        return;
    }
    info_box_values[col][row] = value;
}

void InfoBox::setNameValue(int row, int col, string name, string value) {
    setName(row, col, name);
    setValue(row, col, value);
}

int InfoBox::determineCharWidth() {
    if (font == NULL) {
        printf("Font is not loaded. Cannot determine char width.\n");
        return 0;
    }

    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface* surface = TTF_RenderText_Blended(font, "W", color);
    
    if (surface == NULL) {
        printf("Failed to render text for char width measurement. SDL_ttf Error: %s\n", TTF_GetError());
        return 0;
    }

    int width = surface->w;
    SDL_FreeSurface(surface);

    return width;
}

void InfoBox::render(void *renderer_void) {
    RenderEngine *engine = (RenderEngine*)renderer_void; 
    SDL_Renderer *renderer = engine->getRendererHandle();
    int max_width_name = 0;
    int max_width_value = 0;
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < rows_per_col; j++) {
            if (info_box_names[i][j].length() > max_width_name) {
                max_width_name = info_box_names[i][j].length();
            }
            if (info_box_values[i][j].length() > max_width_value) {
                max_width_value = info_box_values[i][j].length();
            }
        }
    }

    double padding = 0.3;
    int char_width = determineCharWidth();
    int char_height = TTF_FontHeight(font);

    int total_width = (max_width_name + 2 + max_width_value + padding * 2) * char_width * num_cols;
    int total_height = (char_height + padding) * rows_per_col;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_Rect rect = {x, y, total_width, total_height};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int border_width = 1;

    SDL_Rect top_border = {x, y, total_width, border_width};
    SDL_RenderFillRect(renderer, &top_border);
    SDL_Rect bottom_border = {x, y + total_height - border_width, total_width, border_width};
    SDL_RenderFillRect(renderer, &bottom_border);
    SDL_Rect left_border = {x, y, border_width, total_height};
    SDL_RenderFillRect(renderer, &left_border);
    SDL_Rect right_border = {x + total_width - border_width, y, border_width, total_height};
    SDL_RenderFillRect(renderer, &right_border);

    x += padding * char_width;

    SDL_Color textColor = {255, 255, 255, 255};
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < rows_per_col; j++) {
            if (!info_box_names[i][j].empty()) {
                std::string text = info_box_names[i][j] + ": " + info_box_values[i][j];
                
                SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
                if (textSurface) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    if (textTexture) {
                        int text_x = x + padding;
                        int text_y = y + j * (char_height + padding) + padding;
                        SDL_Rect textRect = {text_x, text_y, textSurface->w, textSurface->h};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    SDL_FreeSurface(textSurface);
                }
            }
        }
        x += (max_width_name + 2 + max_width_value + padding * 2) * char_width + 1;
    }
}
