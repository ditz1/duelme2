#pragma once
#include <s_protocol.hpp>
#include <physics.hpp>
#include <s_stage.hpp>


void PlayerMoveLeft(CollisionIndex dirs, PlayerBody& body);
void PlayerMoveRight(CollisionIndex dirs, PlayerBody& body);
void PlayerMoveUp(CollisionIndex dirs, PlayerBody& body);
void PlayerMoveDown(CollisionIndex dirs, PlayerBody& body);
void PlayerApplyGravity(CollisionIndex dirs, PlayerBody& body);
void PlayerIdle(PlayerBody& body);
void PlayerApplyPhysics(Vector2int& pos, PlayerBody& body);
