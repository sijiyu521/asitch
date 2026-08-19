#include "asitch/camera.h"

#include <string.h>

void asitch_camera_init(AsitchCamera *cam, int viewport_w, int viewport_h)
{
    memset(cam, 0, sizeof(*cam));
    cam->zoom = 1.0f;
    cam->lerp = 0.1f;
    cam->viewport_w = viewport_w;
    cam->viewport_h = viewport_h;
}

void asitch_camera_follow(AsitchCamera *cam, AsitchVec2 *target)
{
    cam->target = target;
}

void asitch_camera_update(AsitchCamera *cam, float dt)
{
    (void)dt; /* 保留参数，便于扩展帧率无关缓动 */
    if (!cam->target)
        return;
    cam->x += (cam->target->x - cam->x) * cam->lerp;
    cam->y += (cam->target->y - cam->y) * cam->lerp;
}

void asitch_camera_set_zoom(AsitchCamera *cam, float zoom)
{
    cam->zoom = zoom;
}

void asitch_camera_resize(AsitchCamera *cam, int w, int h)
{
    cam->viewport_w = w;
    cam->viewport_h = h;
}

AsitchVec2 asitch_camera_to_screen(const AsitchCamera *cam, AsitchVec2 world)
{
    AsitchVec2 p;
    float z = cam->zoom;
    p.x = (world.x - cam->x) * z + (float)cam->viewport_w * z * 0.5f;
    p.y = (world.y - cam->y) * z + (float)cam->viewport_h * z * 0.5f;
    return p;
}

AsitchRect asitch_camera_transform_rect(const AsitchCamera *cam, AsitchRect world)
{
    AsitchVec2 p = { world.x, world.y };
    AsitchVec2 s = asitch_camera_to_screen(cam, p);
    AsitchRect r;
    r.x = s.x;
    r.y = s.y;
    r.w = world.w * cam->zoom;
    r.h = world.h * cam->zoom;
    return r;
}
