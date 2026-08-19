#include "asitch/sprite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asitch/camera.h"

/* ---- AsitchEntity  trampolines（供场景调度） ---- */

static void sprite_entity_update(AsitchEntity *self, float dt)
{
    AsitchSprite *s = (AsitchSprite *)self;
    if (s->update)
        s->update(s, dt);
}

static void sprite_entity_render(AsitchEntity *self, SDL_Renderer *ren,
                                 const AsitchCamera *cam)
{
    AsitchSprite *s = (AsitchSprite *)self;
    if (!s->visible)
        return;

    AsitchRect world = { s->x, s->y, s->w, s->h };
    SDL_Rect dst;
    if (cam) {
        AsitchRect t = asitch_camera_transform_rect(cam, world);
        dst.x = (int)t.x;
        dst.y = (int)t.y;
        dst.w = (int)t.w;
        dst.h = (int)t.h;
    } else {
        dst.x = (int)world.x;
        dst.y = (int)world.y;
        dst.w = (int)world.w;
        dst.h = (int)world.h;
    }

    if (s->image) {
        SDL_RenderCopy(ren, s->image, NULL, &dst);
    } else {
        SDL_SetRenderDrawColor(ren, s->color.r, s->color.g, s->color.b, s->color.a);
        SDL_RenderFillRect(ren, &dst);
    }
}

static void sprite_entity_destroy(AsitchEntity *self)
{
    asitch_sprite_destroy((AsitchSprite *)self);
}

/* ---- public API ---- */

AsitchSprite *asitch_sprite_create(float x, float y, float w, float h,
                                   AsitchColor color)
{
    AsitchSprite *s = calloc(1, sizeof(AsitchSprite));
    if (!s)
        return NULL;
    s->entity.update = sprite_entity_update;
    s->entity.render = sprite_entity_render;
    s->entity.destroy = sprite_entity_destroy;
    s->x = x;
    s->y = y;
    s->w = w;
    s->h = h;
    s->color = color;
    s->visible = true;
    s->image = NULL;
    s->tag[0] = '\0';
    return s;
}

void asitch_sprite_destroy(AsitchSprite *s)
{
    /* 纹理归 AssetManager 管，这里不销毁 */
    free(s);
}

void asitch_sprite_set_position(AsitchSprite *s, float x, float y)
{
    s->x = x;
    s->y = y;
}

void asitch_sprite_set_size(AsitchSprite *s, float w, float h)
{
    s->w = w;
    s->h = h;
}

void asitch_sprite_set_image(AsitchSprite *s, SDL_Texture *image)
{
    s->image = image;
}

AsitchRect asitch_sprite_get_bounds(const AsitchSprite *s)
{
    AsitchRect r = { s->x, s->y, s->w, s->h };
    return r;
}

bool asitch_sprite_intersects(const AsitchSprite *a, const AsitchSprite *b)
{
    return a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h &&
           a->y + a->h > b->y;
}
