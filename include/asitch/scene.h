#ifndef ASITCH_SCENE_H
#define ASITCH_SCENE_H

#include <SDL.h>
#include <stddef.h>

#include "types.h"

typedef struct AsitchEngine AsitchEngine;
typedef struct AsitchCamera AsitchCamera;
typedef struct AsitchScene AsitchScene;

/**
 * Entity — 场景实体的公共基类（对应 JS 版「任何有 update/render 的对象」）。
 *
 * 具体类型（如 AsitchSprite）把 AsitchEntity 作为首成员，
 * 即可安全地向上转型为 AsitchEntity* 加入场景。
 */
typedef struct AsitchEntity AsitchEntity;

typedef void (*AsitchEntityUpdate)(AsitchEntity *self, float dt);
typedef void (*AsitchEntityRender)(AsitchEntity *self, SDL_Renderer *ren,
                                   const AsitchCamera *cam);
typedef void (*AsitchEntityDestroy)(AsitchEntity *self);

struct AsitchEntity {
    AsitchEntityUpdate update;
    AsitchEntityRender render;
    AsitchEntityDestroy destroy; /* 可选，场景销毁时调用 */
    AsitchScene *scene;          /* 反向引用，由 asitch_scene_add 设置 */
};

/**
 * Scene — 场景容器：维护一组实体，每帧驱动其 update / render，
 * 并持有背景色与可选相机。
 */
struct AsitchScene {
    AsitchEntity **entities;
    size_t count;
    size_t capacity;
    AsitchColor background;
    AsitchCamera *camera; /* 可选；非 NULL 时传递给实体渲染 */
    void (*on_enter)(AsitchScene *self, AsitchEngine *engine);
    void (*on_exit)(AsitchScene *self);
    void *user_data;
};

AsitchScene *asitch_scene_create(void);
/** 销毁场景；带有 destroy 回调的实体会一并销毁。 */
void asitch_scene_destroy(AsitchScene *s);

void asitch_scene_add(AsitchScene *s, AsitchEntity *e);
void asitch_scene_remove(AsitchScene *s, AsitchEntity *e);

void asitch_scene_update(AsitchScene *s, float dt);
/** 先用背景色清屏，再依次渲染每个实体。 */
void asitch_scene_render(AsitchScene *s, SDL_Renderer *ren);

#endif /* ASITCH_SCENE_H */
