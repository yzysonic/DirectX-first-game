#pragma once
#include "Collider.h"
#include "Rigidbody.h"

#define Gravity (10.0f);

void UpdatePhysics();
void Physics_AddCollider(Collider* thiz);
void Physics_RemoveCollider(Collider* thiz);
void Physics_AddRigidbody(Rigidbody* thiz);
void Physics_RemoveRigidbody(Rigidbody* thiz);
void Physics_SetGravity(Vector3 value);