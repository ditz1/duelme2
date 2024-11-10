#pragma once
#include <s_protocol.hpp>
#include <physics.hpp>
#include <s_stage.hpp>

void PlayerMoveLeft(CollisionIndex dirs, Vector2int& pos);
void PlayerMoveRight(CollisionIndex dirs, Vector2int& pos);
void PlayerMoveUp(CollisionIndex dirs, Vector2int& pos);
void PlayerMoveDown(CollisionIndex dirs, Vector2int& pos);
