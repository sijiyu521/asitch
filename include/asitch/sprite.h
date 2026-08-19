#ifndef ASITCH_SPRITE_H
#define ASITCH_SPRITE_H

#include <SDL.h>
#include <stdbool.h>

#include "scene.h"
#include "types.h"

/**
 * Sprite — 轻量 2D 精灵（实体基类），支持纹理渲染与纯色矩形回退。
 * 对应 JS 版 Sprite。
 */
typedef struct AsitchSprite AsitchSprite;

/** 用户自定义的每帧更新钩子（对应 JS 中覆盖 sprite.update）。 */
typedef void (*AsitchSpriteUpdate)(AsitchSprite *self, float dt);

struct AsitchSprite {
    AsitchEntity entity; /* 基类，必须为首成员 */
    float x, y, w, h;
    SDL_Texture *image;  /* NULL 时回退为纯色矩形 */
    AsitchColor color;
    bool visible;
    char tag[64];
    AsitchSpriteUpdate update; /* 可为 NULL */
    void *user_data;
};

AsitchSprite *asitch_sprite_create(float x, float y, float w, float h,
                                   AsitchColor color);
/** 释放精灵（不销毁其纹理，纹理由 AssetManager 管理）。 */
void asitch_sprite_destroy(AsitchSprite *s);

void asitch_sprite_set_position(AsitchSprite *s, float x, float y);
void asitch_sprite_set_size(AsitchSprite *s, float w, float h);
/** 设置渲染所用纹理；传 NULL 回退为纯色矩形。 */
void asitch_sprite_set_image(AsitchSprite *s, SDL_Texture *image);

AsitchRect asitch_sprite_get_bounds(const AsitchSprite *s);
/** AABB 碰撞检测。 */
bool asitch_sprite_intersects(const AsitchSprite *a, const AsitchSprite *b);

#endif /* ASITCH_SPRITE_H */
