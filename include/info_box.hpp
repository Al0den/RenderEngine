#pragma once

#include <cstdlib>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

namespace rend {
    class InfoBox {
        public:
            InfoBox(int num_row, int num_col);
            ~InfoBox();

            int addRow();
            int addCol();

            int removeRow();
            int removeCol();

            void setRowCol(int row, int col);
            
            void setName(int col, int row, string name);
            void setValue(int col, int row, string value);

            void setNameValue(int col, int row, string name, string value);

            void render(SDL_Renderer *renderer, int x, int y);
    

        private:
            TTF_Font *font;

            string **info_box_names;
            string **info_box_values;

            int num_cols;
            int rows_per_col;
            
            int row_width;

            bool fixed_row_width;

            int determineCharWidth();
    };
}
