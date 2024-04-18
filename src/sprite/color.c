#include <sp.h>

void spColor(Sprite* sp, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    sp->red = red;
    sp->green = green;
    sp->blue = blue;
    sp->alpha = alpha;
}