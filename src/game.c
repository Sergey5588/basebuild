#include "scenes.h"
#include "stdlib.h"
#include "ui.h"
#include "clay_renderer_raylib.h"
#include "globals.h"
#include "scene_manager.h"
#include <flecs.h>
typedef struct {
	ecs_world_t *ecs;
} game_data_t;



void game_init(scene_t *self) {
	self->ctx = calloc(1, sizeof(game_data_t));

	game_data_t *ctx = self->ctx;
	ctx->ecs = ecs_init();
}
void game_enter(scene_t *self) {

}
void game_exit(scene_t *self) {

}
void game_update(scene_t *self, float dt) {
	Clay_Vector2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };  
	Clay_SetPointerState(mousePosition, IsMouseButtonDown(MOUSE_LEFT_BUTTON));  
	Clay_SetLayoutDimensions((Clay_Dimensions) { GetScreenWidth(), GetScreenHeight() });
}
void game_draw(scene_t *self, float dt) {
	Clay_BeginLayout();
	CLAY(CLAY_ID("Root"), {.layout = {.sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}}}) {
	}
	Clay_RenderCommandArray renderCommands = Clay_EndLayout(dt);
	Clay_Raylib_Render(renderCommands, fonts);
	DrawTextEx(fonts[0], "Game here!", (Vector2){0,0},32,2, WHITE);
}
void game_destroy(scene_t *self) {

}
scene_t *create_game_scene(void) {
    scene_t *s = calloc(1, sizeof(scene_t));
    s->init    = game_init;
    s->enter   = game_enter;
    s->exit    = game_exit;
    s->update  = game_update;
    s->draw    = game_draw;
    s->destroy = game_destroy;
    s->init(s);  // allocates ctx
    return s;
}
