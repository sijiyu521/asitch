# Asitch — 轻量级 2D 游戏引擎（C / SDL2 版）

Asitch 是一个轻量级 2D 游戏引擎，采用 C11 编写，基于 SDL2 渲染，适合桌面端开发与原型制作。

> 本项目前身为浏览器 Canvas 版（JavaScript），已于 `dev/c-rewrite` 起重写为 C 语言。

---

## 前置要求

- GCC / Clang（支持 C11）
- GNU Make
- SDL2 与 SDL2_image 开发库

Ubuntu / Debian 安装依赖：

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

---

## 构建与运行

```bash
make          # 编译静态库 build/libasitch.a 和示例 build/minimal-desktop
make run      # 运行示例
make clean    # 清理构建产物
```

示例操作：方向键或 A/D 移动精灵，ESC 退出。

---

## 项目结构

```
include/asitch/   引擎公开头文件（asitch.h 为统一入口）
src/              引擎实现
examples/minimal-desktop/   最小桌面示例
```

## 模块一览

| 模块 | 文件 | 职责 |
| ---- | ---- | ---- |
| Engine | `engine.h/.c` | 窗口 / 渲染器 / 主循环 / 场景切换 |
| Scene | `scene.h/.c` | 实体容器，驱动 update / render |
| Entity | `scene.h` | 实体基类（函数指针式虚方法） |
| Sprite | `sprite.h/.c` | 精灵：纹理或纯色矩形、AABB 碰撞 |
| Camera | `camera.h/.c` | 世界→屏幕变换（平移 + 缩放）、lerp 跟随 |
| Input | `input.h/.c` | 键盘 / 鼠标状态与单帧事件 |
| AssetManager | `assets.h/.c` | 图片（纹理）与文本资源缓存 |

## 快速上手

```c
#include "asitch/asitch.h"

int main(void) {
    AsitchEngineConfig cfg = { .title = "My Game", .width = 800, .height = 600 };
    AsitchEngine *engine = asitch_engine_create(&cfg);

    AsitchScene *scene = asitch_scene_create();
    AsitchSprite *player = asitch_sprite_create(100, 100, 64, 64,
                                                asitch_color_rgb(0xff, 0xcc, 0x00));
    asitch_scene_add(scene, (AsitchEntity *)player);

    asitch_engine_run(engine, scene);  /* 阻塞直到退出 */

    asitch_scene_destroy(scene);
    asitch_engine_destroy(engine);
    return 0;
}
```

自定义实体行为：设置 `AsitchSprite.update` 函数指针（精灵），
或自行填充 `AsitchEntity` 的 `update` / `render` / `destroy`（任意实体）。

## 与 JS 版的差异

- 主循环：JS 版基于 `requestAnimationFrame`，C 版为阻塞式 `asitch_engine_run`。
- 按键标识：JS 用 `KeyboardEvent.code` 字符串，C 用 `SDL_Scancode`。
- 图片资源：JS 可加载 SVG，C 版通过 SDL2_image 支持 PNG/JPG/BMP 等（示例素材已转为 BMP）。
- 相机：JS 版需手动对 ctx 调用 `apply/reset`，C 版由场景把相机传给实体渲染函数自动变换。
- 尚未移植：audio 资源加载（可接入 SDL_mixer）、JSON 解析（`load_text` 只读原文）、文字 HUD（可接入 SDL_ttf）。

## License

MIT
