#include "asitch/scene.h"

#include <stdlib.h>
#include <string.h>

AsitchScene *asitch_scene_create(void)
{
    AsitchScene *s = calloc(1, sizeof(AsitchScene));
    if (!s)
        return NULL;
    s->background = asitch_color_rgb(0x11, 0x11, 0x11);
    return s;
}

void asitch_scene_destroy(AsitchScene *s)
{
    if (!s)
        return;
    for (size_t i = 0; i < s->count; i++) {
        AsitchEntity *e = s->entities[i];
        if (e && e->destroy)
            e->destroy(e);
    }
    free(s->entities);
    free(s);
}

void asitch_scene_add(AsitchScene *s, AsitchEntity *e)
{
    if (!s || !e)
        return;
    if (s->count == s->capacity) {
        size_t cap = s->capacity ? s->capacity * 2 : 8;
        AsitchEntity **next = realloc(s->entities, cap * sizeof(AsitchEntity *));
        if (!next)
            return;
        s->entities = next;
        s->capacity = cap;
    }
    s->entities[s->count++] = e;
    e->scene = s;
}

void asitch_scene_remove(AsitchScene *s, AsitchEntity *e)
{
    if (!s || !e)
        return;
    for (size_t i = 0; i < s->count; i++) {
        if (s->entities[i] == e) {
            memmove(&s->entities[i], &s->entities[i + 1],
                    (s->count - i - 1) * sizeof(AsitchEntity *));
            s->count--;
            return;
        }
    }
}

void asitch_scene_update(AsitchScene *s, float dt)
{
    if (!s)
        return;
    for (size_t i = 0; i < s->count; i++) {
        AsitchEntity *e = s->entities[i];
        if (e && e->update)
            e->update(e, dt);
    }
}

void asitch_scene_render(AsitchScene *s, SDL_Renderer *ren)
{
    if (!s || !ren)
        return;
    SDL_SetRenderDrawColor(ren, s->background.r, s->background.g, s->background.b,
                           s->background.a);
    SDL_RenderClear(ren);
    for (size_t i = 0; i < s->count; i++) {
        AsitchEntity *e = s->entities[i];
        if (e && e->render)
            e->render(e, ren, s->camera);
    }
}
