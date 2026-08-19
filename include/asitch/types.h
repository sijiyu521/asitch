#ifndef ASITCH_TYPES_H
#define ASITCH_TYPES_H

#include <stdbool.h>
#include <stdint.h>

/** 颜色（RGBA，0~255）。 */
typedef struct AsitchColor {
    uint8_t r, g, b, a;
} AsitchColor;

/** 二维向量 / 点。 */
typedef struct AsitchVec2 {
    float x, y;
} AsitchVec2;

/** 轴对齐矩形。 */
typedef struct AsitchRect {
    float x, y, w, h;
} AsitchRect;

static inline AsitchColor asitch_color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    AsitchColor c = { r, g, b, 255 };
    return c;
}

#endif /* ASITCH_TYPES_H */
