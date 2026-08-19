#include "asitch/assets.h"

#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSET_KEY_MAX 128

typedef enum { ASSET_IMAGE, ASSET_TEXT } AssetType;

typedef struct {
    char key[ASSET_KEY_MAX];
    AssetType type;
    SDL_Texture *texture; /* ASSET_IMAGE */
    char *text;           /* ASSET_TEXT */
} AssetEntry;

struct AsitchAssets {
    SDL_Renderer *renderer;
    AssetEntry *entries;
    size_t count;
    size_t capacity;
};

static AssetEntry *assets_find(AsitchAssets *a, const char *key)
{
    for (size_t i = 0; i < a->count; i++) {
        if (strncmp(a->entries[i].key, key, ASSET_KEY_MAX) == 0)
            return &a->entries[i];
    }
    return NULL;
}

static AssetEntry *assets_insert(AsitchAssets *a, const char *key, AssetType type)
{
    if (a->count == a->capacity) {
        size_t cap = a->capacity ? a->capacity * 2 : 8;
        AssetEntry *next = realloc(a->entries, cap * sizeof(AssetEntry));
        if (!next)
            return NULL;
        a->entries = next;
        a->capacity = cap;
    }
    AssetEntry *e = &a->entries[a->count++];
    memset(e, 0, sizeof(*e));
    strncpy(e->key, key, ASSET_KEY_MAX - 1);
    e->type = type;
    return e;
}

AsitchAssets *asitch_assets_create(SDL_Renderer *renderer)
{
    AsitchAssets *a = calloc(1, sizeof(AsitchAssets));
    if (!a)
        return NULL;
    a->renderer = renderer;
    return a;
}

void asitch_assets_destroy(AsitchAssets *a)
{
    if (!a)
        return;
    asitch_assets_clear(a);
    free(a->entries);
    free(a);
}

SDL_Texture *asitch_assets_load_image(AsitchAssets *a, const char *key, const char *path)
{
    if (!a || !key || !path)
        return NULL;

    AssetEntry *cached = assets_find(a, key);
    if (cached && cached->type == ASSET_IMAGE)
        return cached->texture;

    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "AssetManager: failed to load image: %s (%s)\n", path,
                IMG_GetError());
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(a->renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        fprintf(stderr, "AssetManager: failed to create texture: %s (%s)\n", path,
                SDL_GetError());
        return NULL;
    }

    AssetEntry *e = assets_insert(a, key, ASSET_IMAGE);
    if (!e) {
        SDL_DestroyTexture(tex);
        return NULL;
    }
    e->texture = tex;
    return tex;
}

SDL_Texture *asitch_assets_get_image(AsitchAssets *a, const char *key)
{
    AssetEntry *e = assets_find(a, key);
    return (e && e->type == ASSET_IMAGE) ? e->texture : NULL;
}

char *asitch_assets_load_text(AsitchAssets *a, const char *key, const char *path)
{
    if (!a || !key || !path)
        return NULL;

    AssetEntry *cached = assets_find(a, key);
    if (cached && cached->type == ASSET_TEXT)
        return cached->text;

    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "AssetManager: failed to open file: %s\n", path);
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);

    char *buf = malloc((size_t)size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t nread = fread(buf, 1, (size_t)size, f);
    fclose(f);
    buf[nread] = '\0';

    AssetEntry *e = assets_insert(a, key, ASSET_TEXT);
    if (!e) {
        free(buf);
        return NULL;
    }
    e->text = buf;
    return buf;
}

const char *asitch_assets_get_text(AsitchAssets *a, const char *key)
{
    AssetEntry *e = assets_find(a, key);
    return (e && e->type == ASSET_TEXT) ? e->text : NULL;
}

bool asitch_assets_has(const AsitchAssets *a, const char *key)
{
    return assets_find((AsitchAssets *)a, key) != NULL;
}

void asitch_assets_clear(AsitchAssets *a)
{
    for (size_t i = 0; i < a->count; i++) {
        if (a->entries[i].type == ASSET_IMAGE && a->entries[i].texture)
            SDL_DestroyTexture(a->entries[i].texture);
        if (a->entries[i].type == ASSET_TEXT)
            free(a->entries[i].text);
    }
    a->count = 0;
}
