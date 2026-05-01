#include <raylib.h>
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include <flecs.h>


#undef ECS_ALIGNOF  
#define ECS_ALIGNOF(T) ((int64_t)_Alignof(T))
#define RES_PATH "resources/"
#include "globals.h"

#include "clay_renderer_raylib.h"
#include "scenes.h"
scene_manager_t g_scene_mgr;
Font fonts[1];
bool appShouldClose = false;
int main(int argc, char* argv[]) {
	Clay_Raylib_Initialize(GetMonitorWidth(0),GetMonitorHeight(0),"Hello world!", FLAG_WINDOW_HIGHDPI|FLAG_FULLSCREEN_MODE);
	uint32_t memorySize = Clay_MinMemorySize();
	void* memory = malloc(memorySize);
	Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memorySize, memory);
	Clay_Initialize(arena, (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()}, (Clay_ErrorHandler){0});
	fonts[0] = LoadFontEx(RES_PATH"RobotoMono.ttf", 48, 0, 400);
	SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
	Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

	
	scene_manager_init(&g_scene_mgr);
	scene_t *menu = create_main_menu_scene();
	scene_manager_change_scene(&g_scene_mgr, menu, 0.0f);
	
	SetTargetFPS(180);
	while(!WindowShouldClose() && !appShouldClose) {
		float dt = GetFrameTime();
		scene_manager_update(&g_scene_mgr,dt);
		BeginDrawing();
		ClearBackground(BLACK);
		scene_manager_draw(&g_scene_mgr);
		EndDrawing();
	}
	scene_manager_quit(&g_scene_mgr);
	Clay_Raylib_Close();
	return 0;
}
void handle_clay_error(Clay_ErrorData e) {
	TraceLog(LOG_ERROR, "Clay: %s\n", e.errorText.chars);
}
