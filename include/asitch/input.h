#ifndef ASITCH_INPUT_H
#define ASITCH_INPUT_H

#include <SDL.h>
#include <stdbool.h>

/**
 * Input — 轻量键盘 / 鼠标输入管理器（对应 JS 版 Input）。
 *
 * JS 版以 KeyboardEvent.code 字符串（'KeyW'、'ArrowLeft'）标识按键，
 * C 版对应使用 SDL_Scancode（SDL_SCANCODE_W、SDL_SCANCODE_LEFT）。
 */
typedef struct AsitchInput {
    bool keys_down[SDL_NUM_SCANCODES];
    bool just_pressed[SDL_NUM_SCANCODES];
    bool just_released[SDL_NUM_SCANCODES];
    bool mouse_down[8]; /* 以 SDL 按键号索引：SDL_BUTTON_LEFT/MIDDLE/RIGHT */
    int mouse_x, mouse_y;
    float world_x, world_y; /* 由使用方按需填充（如经相机反变换） */
} AsitchInput;

AsitchInput *asitch_input_create(void);
void asitch_input_destroy(AsitchInput *in);

/** 把一个 SDL 事件喂给输入管理器（由引擎主循环调用）。 */
void asitch_input_handle_event(AsitchInput *in, const SDL_Event *e);

/** 按键当前是否按住。 */
bool asitch_input_key_down(const AsitchInput *in, SDL_Scancode code);
/** 按键是否仅在本帧按下。 */
bool asitch_input_key_pressed(const AsitchInput *in, SDL_Scancode code);
/** 按键是否仅在本帧抬起。 */
bool asitch_input_key_released(const AsitchInput *in, SDL_Scancode code);
/** 鼠标按键是否按住（btn: SDL_BUTTON_LEFT / SDL_BUTTON_MIDDLE / SDL_BUTTON_RIGHT）。 */
bool asitch_input_mouse_down(const AsitchInput *in, int btn);

/** 清除单帧状态（just_pressed / just_released），每帧末由引擎调用。 */
void asitch_input_consume_frame(AsitchInput *in);

#endif /* ASITCH_INPUT_H */
