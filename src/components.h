#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <raylib.h>
#include <flecs.h>

typedef struct {
	float x,y;
} Position;

typedef struct {
	Texture2D texture;
} Sprite3D;
extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Sprite3D);
#endif
