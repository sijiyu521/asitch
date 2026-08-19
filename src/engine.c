#include "asitch/engine.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

AsitchEngine *asitch_engine_create(const AsitchEngineConfig *cfg)
{
    AsitchEngineConfig c = cfg ? *cfg : (AsitchEngineConfig){ 0 };
    if (!c.title)
        c.title = "Asitch";
    if (c.width <= 0)
        c.width = 800;
    if (c.height <= 0)
        c.height = 600;
    if (c.background.r == 0 && c.background.g == 0 && c.background.b == 0 &&
        c.background.a == 0)
        c.background = asitch_color_rgb(0x22, 0x22, 0x22);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Engine: SDL_Init failed: %s\n", SDL_GetError());
        return NULL;
    }

    AsitchEngine *e = calloc(1, sizeof(AsitchEngine));
    if (!e)
        return NULL;

    e->width = c.width;
    e->height = c.height;
    e->background = c.background;
    e->fixed_dt = c.fixed_dt;

    e->window = SDL_CreateWindow(c.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 c.width, c.height,
                                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!e->window) {
        fprintf(stderr, "Engine: SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        free(e);
        return NULL;
    }

    e->renderer = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!e->renderer) /* 无加速/无 vsync 环境（如 dummy 驱动）时回退软件渲染 */
        e->renderer = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_SOFTWARE);
    if (!e->renderer) {
        fprintf(stderr, "Engine: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(e->window);
        SDL_Quit();
        free(e);
        return NULL;
    }

    e->assets = asitch_assets_create(e->renderer);
    e->input = asitch_input_create();
    e->camera = malloc(sizeof(AsitchCamera));
    asitch_camera_init(e->camera, c.width, c.height);

    return e;
}

void asitch_engine_destroy(AsitchEngine *e)
{
    if (!e)
        return;
    asitch_assets_destroy(e->assets);
    asitch_input_destroy(e->input);
    free(e->camera);
    if (e->renderer)
        SDL_DestroyRenderer(e->renderer);
    if (e->window)
        SDL_DestroyWindow(e->window);
    free(e);
    SDL_Quit();
}

void asitch_engine_set_scene(AsitchEngine *e, AsitchScene *scene)
{
    if (e->current_scene && e->current_scene->on_exit)
        e->current_scene->on_exit(e->current_scene);
    e->current_scene = scene;
    if (scene && scene->on_enter)
        scene->on_enter(scene, e);
}

void asitch_engine_stop(AsitchEngine *e)
{
    e->running = false;
}

void asitch_engine_resize(AsitchEngine *e, int w, int h)
{
    e->width = w;
    e->height = h;
    asitch_camera_resize(e->camera, w, h);
}

void asitch_engine_run(AsitchEngine *e, AsitchScene *scene)
{
    asitch_engine_set_scene(e, scene);
    if (e->running)
        return;
    e->running = true;

    uint64_t last = SDL_GetTicks64();

    while (e->running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                e->running = false;
            } else if (ev.type == SDL_WINDOWEVENT &&
                       ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                asitch_engine_resize(e, ev.window.data1, ev.window.data2);
            } else {
                asitch_input_handle_event(e->input, &ev);
            }
        }
        if (!e->running)
            break;

        uint64_t now = SDL_GetTicks64();
        float raw_dt = (float)(now - last) / 1000.0f;
        last = now;
        float dt = e->fixed_dt > 0 ? e->fixed_dt : raw_dt;

        asitch_camera_update(e->camera, dt);
        if (e->current_scene)
            asitch_scene_update(e->current_scene, dt);

        SDL_SetRenderDrawColor(e->renderer, e->background.r, e->background.g,
                               e->background.b, e->background.a);
        SDL_RenderClear(e->renderer);
        if (e->current_scene)
            asitch_scene_render(e->current_scene, e->renderer);
        SDL_RenderPresent(e->renderer);

        asitch_input_consume_frame(e->input);
    }
}
