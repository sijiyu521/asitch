#ifndef ASITCH_ENGINE_H
#define ASITCH_ENGINE_H

#include <SDL.h>
#include <stdbool.h>

#include "assets.h"
#include "camera.h"
#include "input.h"
#include "scene.h"
#include "types.h"

/**
 * Engine — Asitch 2D 引擎核心（对应 JS 版 Engine）。
 *
 * JS 版基于 requestAnimationFrame；C 版为标准的 SDL 主循环
 * （asitch_engine_run 阻塞运行，直到 asitch_engine_stop 或窗口关闭）。
 */
typedef struct AsitchEngine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    AsitchColor background;
    float fixed_dt; /* >0 时使用固定步长（秒），0 表示可变步长 */
    bool running;
    AsitchScene *current_scene;
    AsitchAssets *assets;
    AsitchInput *input;
    AsitchCamera *camera;
    void *user_data;
} AsitchEngine;

typedef struct AsitchEngineConfig {
    const char *title;     /* 窗口标题，NULL 时为 "Asitch" */
    int width, height;     /* <=0 时默认 800x600 */
    AsitchColor background;/* 全 0 时默认 #222222 */
    float fixed_dt;        /* 默认 0（可变步长） */
} AsitchEngineConfig;

/** 创建引擎（初始化 SDL、窗口与渲染器）。失败返回 NULL。 */
AsitchEngine *asitch_engine_create(const AsitchEngineConfig *cfg);
void asitch_engine_destroy(AsitchEngine *e);

/** 以给定场景启动主循环（阻塞，直到停止）。 */
void asitch_engine_run(AsitchEngine *e, AsitchScene *scene);
/** 停止主循环。 */
void asitch_engine_stop(AsitchEngine *e);

/** 切换场景：调用旧场景 on_exit 与新场景 on_enter。 */
void asitch_engine_set_scene(AsitchEngine *e, AsitchScene *scene);
/** 调整窗口/视口尺寸。 */
void asitch_engine_resize(AsitchEngine *e, int w, int h);

#endif /* ASITCH_ENGINE_H */
