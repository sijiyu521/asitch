#include "asitch/input.h"

#include <stdlib.h>
#include <string.h>

AsitchInput *asitch_input_create(void)
{
    AsitchInput *in = calloc(1, sizeof(AsitchInput));
    return in;
}

void asitch_input_destroy(AsitchInput *in)
{
    free(in);
}

void asitch_input_handle_event(AsitchInput *in, const SDL_Event *e)
{
    switch (e->type) {
    case SDL_KEYDOWN: {
        SDL_Scancode code = e->key.keysym.scancode;
        if (code < 0 || code >= SDL_NUM_SCANCODES)
            break;
        if (!e->key.repeat && !in->keys_down[code])
            in->just_pressed[code] = true;
        in->keys_down[code] = true;
        break;
    }
    case SDL_KEYUP: {
        SDL_Scancode code = e->key.keysym.scancode;
        if (code < 0 || code >= SDL_NUM_SCANCODES)
            break;
        in->keys_down[code] = false;
        in->just_released[code] = true;
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
        if (e->button.button > 0 && e->button.button < 8)
            in->mouse_down[e->button.button] = true;
        break;
    case SDL_MOUSEBUTTONUP:
        if (e->button.button > 0 && e->button.button < 8)
            in->mouse_down[e->button.button] = false;
        break;
    case SDL_MOUSEMOTION:
        in->mouse_x = e->motion.x;
        in->mouse_y = e->motion.y;
        break;
    default:
        break;
    }
}

bool asitch_input_key_down(const AsitchInput *in, SDL_Scancode code)
{
    if (code < 0 || code >= SDL_NUM_SCANCODES)
        return false;
    return in->keys_down[code];
}

bool asitch_input_key_pressed(const AsitchInput *in, SDL_Scancode code)
{
    if (code < 0 || code >= SDL_NUM_SCANCODES)
        return false;
    return in->just_pressed[code];
}

bool asitch_input_key_released(const AsitchInput *in, SDL_Scancode code)
{
    if (code < 0 || code >= SDL_NUM_SCANCODES)
        return false;
    return in->just_released[code];
}

bool asitch_input_mouse_down(const AsitchInput *in, int btn)
{
    if (btn <= 0 || btn >= 8)
        return false;
    return in->mouse_down[btn];
}

void asitch_input_consume_frame(AsitchInput *in)
{
    memset(in->just_pressed, 0, sizeof(in->just_pressed));
    memset(in->just_released, 0, sizeof(in->just_released));
}
