#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <flecs.h>

void RenderSprite3D(ecs_iter_t *it);
extern ECS_SYSTEM_DECLARE(RenderSprite3D);
#endif
