#ifndef ASITCH_ASSETS_H
#define ASITCH_ASSETS_H

#include <SDL.h>
#include <stdbool.h>

/**
 * AssetManager — 轻量资源管理器（对应 JS 版 AssetManager）。
 *
 * - images：通过 SDL2_image 加载（PNG/JPG/BMP 等），缓存为 SDL_Texture*
 * - text：整文件读入字符串，可用于 JSON 等内容（解析由使用方负责）
 * - 同一 key 重复加载直接返回缓存
 *
 * 注：JS 版的 audio 资源尚未移植（C 版可后续接入 SDL_mixer）。
 */
typedef struct AsitchAssets AsitchAssets;

AsitchAssets *asitch_assets_create(SDL_Renderer *renderer);
void asitch_assets_destroy(AsitchAssets *a);

/** 加载图片为纹理并按 key 缓存；失败返回 NULL。 */
SDL_Texture *asitch_assets_load_image(AsitchAssets *a, const char *key, const char *path);
/** 取已缓存的图片纹理；不存在返回 NULL。 */
SDL_Texture *asitch_assets_get_image(AsitchAssets *a, const char *key);

/** 读取整个文本文件并按 key 缓存；失败返回 NULL。 */
char *asitch_assets_load_text(AsitchAssets *a, const char *key, const char *path);
/** 取已缓存的文本；不存在返回 NULL。 */
const char *asitch_assets_get_text(AsitchAssets *a, const char *key);

/** 判断指定 key 是否已加载。 */
bool asitch_assets_has(const AsitchAssets *a, const char *key);

/** 清空所有缓存（纹理会被销毁，文本会被释放）。 */
void asitch_assets_clear(AsitchAssets *a);

#endif /* ASITCH_ASSETS_H */
