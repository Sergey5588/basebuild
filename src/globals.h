#ifndef GLOBALS_H
#define GLOBALS_H

#include "scene_manager.h"
#include "clay.h"
#include "raylib.h"
#undef ECS_ALIGNOF  
#define ECS_ALIGNOF(T) ((int64_t)_Alignof(T))
#define RES_PATH "resources/"
extern scene_manager_t g_scene_mgr;
extern Font fonts[1];
extern bool appShouldClose;
#endif
