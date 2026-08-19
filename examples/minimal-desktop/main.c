/**
 * Asitch — minimal desktop example（对应原 examples/minimal-web）
 *
 * 一个 64x64 的精灵在窗口中左右移动：方向键或 A/D 控制，
 * 移出边界后从另一侧绕回。ESC 或关闭窗口退出。
 */
#include <stdio.h>

#include "asitch/asitch.h"

static AsitchEngine *g_engine;

static void player_update(AsitchSprite *self, float dt)
{
    const float speed = 200.0f;
    AsitchInput *input = g_engine->input;

    if (asitch_input_key_down(input, SDL_SCANCODE_RIGHT) ||
        asitch_input_key_down(input, SDL_SCANCODE_D))
        self->x += speed * dt;
    if (asitch_input_key_down(input, SDL_SCANCODE_LEFT) ||
        asitch_input_key_down(input, SDL_SCANCODE_A))
        self->x -= speed * dt;

    if (self->x > g_engine->width)
        self->x = -self->w;
    if (self->x < -self->w)
        self->x = (float)g_engine->width;
}

/* 自定义实体：只负责监听 ESC 退出，演示非 Sprite 实体。 */
static void quitwatch_update(AsitchEntity *self, float dt)
{
    (void)self;
    (void)dt;
    if (asitch_input_key_pressed(g_engine->input, SDL_SCANCODE_ESCAPE))
        asitch_engine_stop(g_engine);
}

static void quitwatch_destroy(AsitchEntity *self)
{
    free(self);
}

int main(void)
{
    AsitchEngineConfig cfg;
    cfg.title = "Asitch - Minimal Desktop Example";
    cfg.width = 800;
    cfg.height = 600;
    cfg.background = asitch_color_rgb(0x0b, 0x0b, 0x0b);
    cfg.fixed_dt = 0;

    AsitchEngine *engine = asitch_engine_create(&cfg);
    if (!engine)
        return 1;
    g_engine = engine;

    AsitchScene *scene = asitch_scene_create();
    scene->background = asitch_color_rgb(0x0b, 0x0b, 0x0b);

    AsitchSprite *player =
        asitch_sprite_create(0, 260, 64, 64, asitch_color_rgb(0xff, 0xcc, 0x00));
    snprintf(player->tag, sizeof(player->tag), "player");
    player->update = player_update;
    asitch_scene_add(scene, (AsitchEntity *)player);

    /* 尝试加载图片素材；失败则保持纯色矩形回退（与 JS 示例行为一致） */
    SDL_Texture *img = asitch_assets_load_image(
        engine->assets, "player", "examples/minimal-desktop/assets/player.bmp");
    if (img)
        asitch_sprite_set_image(player, img);

    AsitchEntity *quitwatch = calloc(1, sizeof(AsitchEntity));
    quitwatch->update = quitwatch_update;
    quitwatch->destroy = quitwatch_destroy;
    asitch_scene_add(scene, quitwatch);

    printf("Asitch engine demo — Arrow keys / A D to move, ESC to quit\n");

    asitch_engine_run(engine, scene);

    asitch_scene_destroy(scene);
    asitch_engine_destroy(engine);
    return 0;
}
