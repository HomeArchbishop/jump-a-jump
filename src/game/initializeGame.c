#include <stdlib.h>
#include <stdbool.h>

#include "../../includes/types/game.h"
#include "../../includes/global.h"

void initializeGame (Game* game, bool isAutomatical) {
  game->score = 0;
  game->isNewGame = true;
  game->isEnd = false;
  game->isAutomatical = isAutomatical;

  game->scene.chess.distanceByXY = GAMECONF__MIN_JUMP_DISTANCE_BY_XY;
  game->scene.chess.height = GAMEPOS__CHESS_HEIGHT;
  game->scene.chess.radius = GAMEPOS__CHESS_RADIUS;
  game->scene.chess.color = COLOR__RED;
  game->scene.chess.x = GAMEPOS__CHESS_ANCHOR_LEFT_X;
  game->scene.chess.y = GAMEPOS__CHESS_ANCHOR_LEFT_Y;
  game->scene.chess.jumpDirection = GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT;
  game->scene.chess.currentStageIndex = 0;

  game->scene.stagesCnt = 2;
  game->scene.stages[0].height = GAMEPOS__STAGE_HEIGHT;
  game->scene.stages[0].radius = GAMEPOS__DEFAULT_STAGE_RADIUS;
  game->scene.stages[0].color = COLOR__GREEN;
  game->scene.stages[0].bodyColor = COLOR__BLUE;
  game->scene.stages[0].type = GAME_SCENE_STAGE_TYPE_SQUARE;
  game->scene.stages[0].x = GAMEPOS__DEFAULT_STAGE_ANCHOR_X;
  game->scene.stages[0].y = GAMEPOS__DEFAULT_STAGE_ANCHOR_Y;

  int randomDeltaXY = GAMEPOS__DEFAULT_STAGE_RADIUS * 2 + rand() % (GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMEPOS__DEFAULT_STAGE_RADIUS * 2 + 1);
  game->scene.stages[1].height = GAMEPOS__STAGE_HEIGHT;
  game->scene.stages[1].radius = GAMEPOS__DEFAULT_STAGE_RADIUS;
  game->scene.stages[1].color = COLOR__SKY_BLUE;
  game->scene.stages[1].bodyColor = COLOR__PURPLE;
  game->scene.stages[1].type = GAME_SCENE_STAGE_TYPE_SQUARE;
  game->scene.stages[1].x = GAMEPOS__DEFAULT_STAGE_ANCHOR_X + randomDeltaXY;
  game->scene.stages[1].y = GAMEPOS__DEFAULT_STAGE_ANCHOR_Y - randomDeltaXY;
}
