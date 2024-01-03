#include <stdlib.h>

#include "../../includes/types/game.h"
#include "../../includes/global.h"
#include "../../includes/utils/tool.h"

int _randomStageColor (int not) {
  int color = COLOR__RANDOM();
  if (color == COLOR__BLACK || color == COLOR__RED) { return _randomStageColor(not); }
  if (not != -1 && not == color) { return _randomStageColor(not); }
  return color;
}

void generateNextStage (Game* game) {
  int randomDeltaXY = GAMEPOS__DEFAULT_STAGE_RADIUS * 2 + rand() % (GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMEPOS__DEFAULT_STAGE_RADIUS * 2 + 1);
  int randomDirection = rand() % 2 ? GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT : GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT;
  game->scene.stages[game->scene.stagesCnt].height = GAMEPOS__STAGE_HEIGHT;
  game->scene.stages[game->scene.stagesCnt].radius = MAX(GAMEPOS__MIN_STAGE_RADIUS, GAMEPOS__DEFAULT_STAGE_RADIUS - game->score / 3);
  game->scene.stages[game->scene.stagesCnt].color = _randomStageColor(-1);
  game->scene.stages[game->scene.stagesCnt].bodyColor = _randomStageColor(game->scene.stages[game->scene.stagesCnt].color);
  game->scene.stages[game->scene.stagesCnt].type = rand() % 2 > 0 ? GAME_SCENE_STAGE_TYPE_SQUARE : GAME_SCENE_STAGE_TYPE_CIRCLE;
  game->scene.stages[game->scene.stagesCnt].x = randomDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT ?
    game->scene.stages[game->scene.stagesCnt - 1].x + randomDeltaXY :
    game->scene.stages[game->scene.stagesCnt - 1].x - randomDeltaXY;
  game->scene.stages[game->scene.stagesCnt].y = game->scene.stages[game->scene.stagesCnt - 1].y - randomDeltaXY;

  game->scene.stagesCnt++;
}
