#pragma once
#include <s_protocol.hpp>
#include <physics.hpp>
#include <s_stage.hpp>


void PlayerMoveLeft(ServerStage& stage, int id);
void PlayerMoveRight(ServerStage& stage, int id);
void PlayerMoveUp(ServerStage& stage, int id);
void PlayerMoveDown(ServerStage& stage, int id);
void PlayerJump();
