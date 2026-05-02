#include "scenes.h"
#include "stdlib.h"
#include "ui.h"
#include "clay_renderer_raylib.h"
#include "scene_manager.h"
#include <flecs.h>
#include "components.h"
#include "systems.h"
#include "globals.h"
typedef struct {
	ecs_world_t *ecs;
	Camera3D cam;
} game_data_t;



ECS_COMPONENT_DECLARE(Sprite3D);  
ECS_COMPONENT_DECLARE(Position);  
ECS_SYSTEM_DECLARE(RenderSprite3D);

void game_init(scene_t *self) {
	self->ctx = calloc(1, sizeof(game_data_t));

	game_data_t *ctx = self->ctx;
	ctx->cam = (Camera3D){
		.fovy = 90.0f,
		.position = (Vector3){5,5,5},
		.target = (Vector3){0},
		.projection = CAMERA_PERSPECTIVE,
		.up = (Vector3){0,1,0}
	};
	ctx->ecs = ecs_init();
	ECS_COMPONENT_DEFINE(ctx->ecs, Sprite3D);
	ECS_COMPONENT_DEFINE(ctx->ecs, Position);

	ECS_SYSTEM_DEFINE(ctx->ecs,RenderSprite3D, EcsOnUpdate, [in] Position, [in] Sprite3D);
	ecs_set_ctx(ctx->ecs, &ctx->cam, NULL);
	ecs_entity_t e = ecs_new(ctx->ecs);
	ecs_set(ctx->ecs, e, Sprite3D, { .texture = LoadTexture(RES_PATH"cat.png") });
	ecs_set(ctx->ecs, e, Position, { 0, 0 });

}
void game_enter(scene_t *self) {

}
void game_exit(scene_t *self) {

}
void game_update(scene_t *self, float dt) {
	game_data_t *ctx = self->ctx;
	Clay_Vector2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };  
	Clay_SetPointerState(mousePosition, IsMouseButtonDown(MOUSE_LEFT_BUTTON));  
	Clay_SetLayoutDimensions((Clay_Dimensions) { GetScreenWidth(), GetScreenHeight() });
	UpdateCamera(&ctx->cam, CAMERA_FREE);
}
void game_draw(scene_t *self, float dt) {
	game_data_t *ctx = self->ctx;
	BeginMode3D(ctx->cam);
	ecs_run(ctx->ecs, ecs_id(RenderSprite3D), dt,NULL);
	DrawGrid(10.0f, 1.0f);
	EndMode3D();

	Clay_BeginLayout();
	CLAY(CLAY_ID("Root"), {.layout = {.sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}}}) {
	}
	Clay_RenderCommandArray renderCommands = Clay_EndLayout(dt);
	Clay_Raylib_Render(renderCommands, fonts);
	DrawTextEx(fonts[0], "Game here!", (Vector2){0,0},32,2, WHITE);
}
void game_destroy(scene_t *self) {
	game_data_t *ctx = self->ctx;
	if(ctx->ecs) ecs_fini(ctx->ecs);
	if(ctx) free(ctx);
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
