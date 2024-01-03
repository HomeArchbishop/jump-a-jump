#include <math.h>

#include "../../includes/types/game.h"

#include "../../includes/utils/tool.h"

int judgeJumpOnStage (Game* game) {
  int jumpDirection = game->scene.chess.jumpDirection;
  int nextX = jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT ?
    game->scene.chess.x + game->scene.chess.distanceByXY :
    game->scene.chess.x - game->scene.chess.distanceByXY;
  int nextY = game->scene.chess.y - game->scene.chess.distanceByXY;
  double rSquare;
  double dManhattan;
  rSquare = pow(game->scene.stages[game->scene.chess.currentStageIndex + 1].x - nextX, 2) + pow(game->scene.stages[game->scene.chess.currentStageIndex + 1].y - nextY, 2);
  dManhattan = ABS(game->scene.stages[game->scene.chess.currentStageIndex + 1].x - nextX) + ABS(game->scene.stages[game->scene.chess.currentStageIndex + 1].y - nextY);
  /* judge is on next stage */
  if (game->scene.stages[game->scene.chess.currentStageIndex + 1].type == GAME_SCENE_STAGE_TYPE_CIRCLE) {
    if (pow(game->scene.stages[game->scene.chess.currentStageIndex + 1].radius, 2) >= rSquare) { return game->scene.chess.currentStageIndex + 1; }
  } else /* GAME_SCENE_STAGE_TYPE_SQUARE */ {
    if (game->scene.stages[game->scene.chess.currentStageIndex + 1].radius >= dManhattan) { return game->scene.chess.currentStageIndex + 1; }
  }
  /* judge is on current stage */
  rSquare = pow(game->scene.stages[game->scene.chess.currentStageIndex].x - nextX, 2) + pow(game->scene.stages[game->scene.chess.currentStageIndex].y - nextY, 2);
  dManhattan = ABS(game->scene.stages[game->scene.chess.currentStageIndex].x - nextX) + ABS(game->scene.stages[game->scene.chess.currentStageIndex].y - nextY);
  if (game->scene.stages[game->scene.chess.currentStageIndex].type == GAME_SCENE_STAGE_TYPE_CIRCLE) {
    if (pow(game->scene.stages[game->scene.chess.currentStageIndex].radius, 2) >= rSquare) { return game->scene.chess.currentStageIndex; }
  } else /* GAME_SCENE_STAGE_TYPE_SQUARE */ {
    if (game->scene.stages[game->scene.chess.currentStageIndex].radius >= dManhattan) { return game->scene.chess.currentStageIndex; }
  }

  return -1;
}
