#include "../../includes/types/game.h"
#include "../../includes/global.h"

void updateStateAfterMove (Game* game) {
  int anchorX = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT ?
    GAMEPOS__CHESS_ANCHOR_RIGHT_X:
    GAMEPOS__CHESS_ANCHOR_LEFT_X;
  int anchorY = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT ?
    GAMEPOS__CHESS_ANCHOR_RIGHT_Y:
    GAMEPOS__CHESS_ANCHOR_LEFT_Y;

  for (int i = 0; i < game->scene.stagesCnt; i++) {
    game->scene.stages[i].x += anchorX - game->scene.chess.x;
    game->scene.stages[i].y += anchorY - game->scene.chess.y;
  }

  game->scene.chess.x = anchorX;
  game->scene.chess.y = anchorY;
}
