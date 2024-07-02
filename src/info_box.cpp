#include "../include/info_box.hpp"

#include <cstdlib>

using namespace rend;

InfoBox::InfoBox(int num_row, int num_col) {
    num_cols = num_col;
    rows_per_col = num_row;
    row_width = 0;

    fixed_row_width = false;

    info_box_names = nullptr;
    info_box_values = nullptr;
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
}

int InfoBox::addRow() {
    if(rows_per_col == 0 || num_cols == 0) {
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

    return rows_per_col;
}

int InfoBox::addCol() {
    if(rows_per_col == 0 || num_cols == 0) {
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

    return num_cols;
}

int InfoBox::removeRow() {
    if (rows_per_col == 0 || num_cols == 0) {
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
    if(rows_per_col == 0 || num_cols == 0) {
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

    return num_cols;
}

void InfoBox::setName(int col, int row, string name) {
    if (col >= num_cols || row >= rows_per_col) {
        return;
    }
    info_box_names[col][row] = name;
}

void InfoBox::setValue(int col, int row, string value) {
    if (col >= num_cols || row >= rows_per_col) {
        return;
    }
    info_box_values[col][row] = value;
}

void InfoBox::setNameValue(int col, int row, string name, string value) {
    setName(col, row, name);
    setValue(col, row, value);
}

void InfoBox::render(SDL_Renderer *renderer, int x, int y) {
    // Display the info box
    int max_width = 0;
    SDL_Rect rect;

    for (int i=0; i<num_cols; i++) {
        for (int j=0; j<rows_per_col; j++) {
            if (info_box_names[i][j].length() > max_width) {
                max_width = info_box_names[i][j].length();
            }
        }
    }

    if (fixed_row_width) {
        max_width = row_width;
    }
}
