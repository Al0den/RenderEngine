#pragma once

struct BoundingBox {
    int pos_x;
    int pos_y;
    int width;
    int height;
};

struct RelativePosition {
    int pos_x;
    int pos_y;
};

struct PercentPosition {
    float pos_x;
    float pos_y;
};

bool isInBoundingBox(RelativePosition pos, BoundingBox box);

