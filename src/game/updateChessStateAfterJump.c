#include "../../includes/types/game.h"
#include "../../includes/global.h"

void updateChessStateAfterJump (Game* game, int jumpToStageIndex) {
  game->scene.chess.x = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT ?
    game->scene.chess.x + game->scene.chess.distanceByXY :
    game->scene.chess.x - game->scene.chess.distanceByXY;
  game->scene.chess.y = game->scene.chess.y - game->scene.chess.distanceByXY;
  game->scene.chess.distanceByXY = GAMECONF__MIN_JUMP_DISTANCE_BY_XY;
  game->scene.chess.currentStageIndex = jumpToStageIndex;
  game->scene.chess.jumpDirection = game->scene.stages[jumpToStageIndex + 1].x > game->scene.stages[jumpToStageIndex].x ?
    GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT :
    GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT;
}
