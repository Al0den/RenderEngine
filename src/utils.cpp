#include "../include/utils.hpp"

bool isInBoundingBox(RelativePosition pos, BoundingBox box) {
    return pos.pos_x >= box.pos_x && pos.pos_x <= box.pos_x + box.width && pos.pos_y >= box.pos_y && pos.pos_y <= box.pos_y + box.height;
}
