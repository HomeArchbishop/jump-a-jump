#include <stdlib.h>

#include "../../includes/types/game.h"
#include "../../includes/global.h"

void clearInvisibleStages (Game* game) {
  int offset = 0;
  for (int i = 0; i < game->scene.stagesCnt; i++) {
    if (
      game->scene.stages[i].x + game->scene.stages[i].radius < 0 ||
      game->scene.stages[i].x - game->scene.stages[i].radius > VIEW_WIDTH ||
      game->scene.stages[i].y + game->scene.stages[i].radius + game->scene.stages[i].height < 0 ||
      GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].y - game->scene.stages[i].radius) > VIEW_MAIN_HEIGHT
    ) /* invisible */ { offset++; } else { break; }
  }

  if (offset == 0) { return; }

  for (int i = 0; i < game->scene.stagesCnt - offset; i++) {
    game->scene.stages[i].bodyColor = game->scene.stages[i + offset].bodyColor;
    game->scene.stages[i].color = game->scene.stages[i + offset].color;
    game->scene.stages[i].height = game->scene.stages[i + offset].height;
    game->scene.stages[i].radius = game->scene.stages[i + offset].radius;
    game->scene.stages[i].type = game->scene.stages[i + offset].type;
    game->scene.stages[i].x = game->scene.stages[i + offset].x;
    game->scene.stages[i].y = game->scene.stages[i + offset].y;
  }

  game->scene.stagesCnt -= offset;
  game->scene.chess.currentStageIndex -= offset;
}
