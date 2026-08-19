#ifndef ASITCH_CAMERA_H
#define ASITCH_CAMERA_H

#include "types.h"

/**
 * Camera — 世界坐标到屏幕坐标的变换（平移 + 缩放），
 * 并可选地平滑跟随某个目标点。
 *
 * 与 JS 版 Camera 对应：update() 做 lerp 跟随，
 * to_screen()/transform_rect() 对应 apply() 施加到 ctx 的变换。
 */
typedef struct AsitchCamera {
    float x, y;            /* 摄像机世界坐标 */
    float zoom;            /* 缩放倍率（>1 放大，<1 缩小） */
    float lerp;            /* 跟随平滑插值系数（0~1，越大跟随越快） */
    int viewport_w;
    int viewport_h;
    AsitchVec2 *target;    /* 跟随目标，NULL 表示不跟随 */
} AsitchCamera;

/** 初始化摄像机，zoom=1，lerp=0.1（对应 JS 默认值）。 */
void asitch_camera_init(AsitchCamera *cam, int viewport_w, int viewport_h);

/** 设置要跟随的目标点；传 NULL 取消跟随。 */
void asitch_camera_follow(AsitchCamera *cam, AsitchVec2 *target);

/** 每帧调用：若设置了目标，用 lerp 平滑对齐目标。dt 保留以便扩展。 */
void asitch_camera_update(AsitchCamera *cam, float dt);

void asitch_camera_set_zoom(AsitchCamera *cam, float zoom);

/** 更新视口尺寸（窗口 resize 时调用）。 */
void asitch_camera_resize(AsitchCamera *cam, int w, int h);

/**
 * 世界坐标 → 屏幕坐标，与 JS apply() 的变换一致：
 * screen = (world - cam) * zoom + viewport * zoom / 2
 */
AsitchVec2 asitch_camera_to_screen(const AsitchCamera *cam, AsitchVec2 world);

/** 矩形版本的世界 → 屏幕变换（尺寸同时乘以 zoom）。 */
AsitchRect asitch_camera_transform_rect(const AsitchCamera *cam, AsitchRect world);

#endif /* ASITCH_CAMERA_H */
