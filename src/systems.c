#include <flecs.h>
#include <raylib.h>
#include "systems.h"
#include "components.h"

void RenderSprite3D(ecs_iter_t *it) {
	Camera3D *cam = (Camera3D*)(ecs_get_ctx(it->world));
	Position *p = ecs_field(it, Position, 0);
	Sprite3D *s = ecs_field(it, Sprite3D, 1);
	for(int i = 0; i < it->count; ++i) {
		DrawBillboard(*cam, s[i].texture, (Vector3){p[i].x,0.5f, p[i].y}, 1.0f, WHITE);

	}
	
}
