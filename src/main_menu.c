#include "scenes.h"
#include "stdlib.h"
#include "ui.h"
#include "clay_renderer_raylib.h"
#include "globals.h"
#include "scene_manager.h"
//typedef struct {
//
//} main_menu_data_t;

void CloseApp() {
	appShouldClose = true;
}
void StartGame() {
	scene_t *game = create_game_scene();
	scene_manager_change_scene(&g_scene_mgr, game, 1.0f);
}

void main_menu_init(scene_t *self) {

}
void main_menu_enter(scene_t *self) {

}
void main_menu_exit(scene_t *self) {

}
void main_menu_update(scene_t *self, float dt) {
	Clay_Vector2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };  
	Clay_SetPointerState(mousePosition, IsMouseButtonDown(MOUSE_LEFT_BUTTON));  
	Clay_SetLayoutDimensions((Clay_Dimensions) { GetScreenWidth(), GetScreenHeight() });
}
void main_menu_draw(scene_t *self, float dt) {
	Clay_BeginLayout();
	CLAY(CLAY_ID("Root"), {.layout = {.sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}}}) {
		CLAY(CLAY_ID("Buttons"), { .layout = {.childGap = 8, .layoutDirection= CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_PERCENT(0.25), CLAY_SIZING_PERCENT(0.5)}}}) {
			CLAY_TEXT(CLAY_STRING("Base building game"), TEXT_CONFIG);
			Button(CLAY_STRING("Play"), (__ButtonClickFn)StartGame);
			Button(CLAY_STRING("Settings"), NULL);
			Button(CLAY_STRING("Exit"),(__ButtonClickFn)CloseApp);
		}
	}
	Clay_RenderCommandArray renderCommands = Clay_EndLayout(dt);
	Clay_Raylib_Render(renderCommands, fonts);
	DrawTextEx(fonts[0], "Main menu here", (Vector2){0,0},32,2, WHITE);
}
void main_menu_destroy(scene_t *self) {

}
scene_t *create_main_menu_scene(void) {
    scene_t *s = calloc(1, sizeof(scene_t));
    s->init    = main_menu_init;
    s->enter   = main_menu_enter;
    s->exit    = main_menu_exit;
    s->update  = main_menu_update;
    s->draw    = main_menu_draw;
    s->destroy = main_menu_destroy;
    s->init(s);  // allocates ctx
    return s;
}
