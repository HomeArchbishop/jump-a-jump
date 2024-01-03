#include <stdbool.h>
#include <math.h>

#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"
#include "../../../includes/types/game.h"

void createGamepageCompressFrame (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game) {

  float stageHRatio = 1.0f - (
    (double)game->scene.chess.distanceByXY / (double)(GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMECONF__MIN_JUMP_DISTANCE_BY_XY)
    * (double)(game->scene.stages[game->scene.chess.currentStageIndex].height - GAMEPOS__STAGE_MIN_COMPRESSED_HEIGHT)
    / (double)game->scene.stages[game->scene.chess.currentStageIndex].height
  );
  float lengthRatio = 1.0f - (
    (double)game->scene.chess.distanceByXY / (double)(GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMECONF__MIN_JUMP_DISTANCE_BY_XY)
    * (double)(game->scene.chess.height - GAMEPOS__CHESS_MIN_COMPRESSED_HEIGHT) / (double)game->scene.chess.height
  );
  float chessDeltaY = -(1.0f - stageHRatio) * (float)game->scene.stages[game->scene.chess.currentStageIndex].height;

  for (int i = 0; i < VIEW_MAIN_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__BLACK;
    }
  }

  /* create stages */
  for (int i = 0; i < game->scene.stagesCnt; i++) {
    int stageBottomBaseY = GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].y + game->scene.stages[i].height);
    bool isChessCurrentOnThisStage = i == game->scene.chess.currentStageIndex;
    int centerBlockX = game->scene.stages[i].x;
    int thisStageHeight = isChessCurrentOnThisStage ?
      GAMEPOS_TO_VIEW__SCALE_Y_FN(stageHRatio * (float)game->scene.stages[i].height) :
      GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].height);
    for (int deltaXAbs = 0; deltaXAbs <= game->scene.stages[i].radius; deltaXAbs++) {
      int maxBlockDeltaY;
      if (game->scene.stages[i].type == GAME_SCENE_STAGE_TYPE_CIRCLE) /* stage is circle */ {
        maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN(sqrt(pow((float)game->scene.stages[i].radius, 2) - pow(deltaXAbs, 2)));
      } else /* GAME_SCENE_STAGE_TYPE_SQUARE (stage is square) */ {
        maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN((float)game->scene.stages[i].radius - deltaXAbs);
      }
      bool isX1 = centerBlockX - deltaXAbs >= 0 && centerBlockX - deltaXAbs < VIEW_WIDTH;
      bool isX2 = centerBlockX + deltaXAbs >= 0 && centerBlockX + deltaXAbs < VIEW_WIDTH;
      /* create stage body */
      for (
        int bodyDeltaY = 1;
        bodyDeltaY <= thisStageHeight;
        bodyDeltaY++
      ) {
        if (stageBottomBaseY + maxBlockDeltaY - bodyDeltaY < 0) { break; }
        if (stageBottomBaseY + maxBlockDeltaY - bodyDeltaY >= VIEW_MAIN_HEIGHT) { continue; }
        if (isX1) { frame[stageBottomBaseY + maxBlockDeltaY - bodyDeltaY][centerBlockX - deltaXAbs].color = game->scene.stages[i].bodyColor; }
        if (isX2) { frame[stageBottomBaseY + maxBlockDeltaY - bodyDeltaY][centerBlockX + deltaXAbs].color = game->scene.stages[i].bodyColor; }
      }
      /* create stage upper-side */
      for (int blockDeltaY = 0; blockDeltaY <= maxBlockDeltaY; blockDeltaY++) {
        bool isY1 = stageBottomBaseY - thisStageHeight - blockDeltaY >= 0 && stageBottomBaseY - thisStageHeight - blockDeltaY < VIEW_MAIN_HEIGHT;
        bool isY2 = stageBottomBaseY - thisStageHeight + blockDeltaY >= 0 && stageBottomBaseY - thisStageHeight + blockDeltaY < VIEW_MAIN_HEIGHT;
        if (isX1 && isY1) { frame[stageBottomBaseY - thisStageHeight - blockDeltaY][centerBlockX - deltaXAbs].color = game->scene.stages[i].color; }
        if (isX2 && isY1) { frame[stageBottomBaseY - thisStageHeight - blockDeltaY][centerBlockX + deltaXAbs].color = game->scene.stages[i].color; }
        if (isX1 && isY2) { frame[stageBottomBaseY - thisStageHeight + blockDeltaY][centerBlockX - deltaXAbs].color = game->scene.stages[i].color; }
        if (isX2 && isY2) { frame[stageBottomBaseY - thisStageHeight + blockDeltaY][centerBlockX + deltaXAbs].color = game->scene.stages[i].color; }
      }
    }
  }

  /* create chess */
  int chessCenterBlockX = game->scene.chess.x;
  int chessCenterBlockY = GAMEPOS_TO_VIEW__SCALE_Y_FN((float)game->scene.chess.y - chessDeltaY);
  for (int deltaXAbs = 0; deltaXAbs <= game->scene.chess.radius; deltaXAbs++) {
    bool isX1 = chessCenterBlockX - deltaXAbs >= 0 && chessCenterBlockX - deltaXAbs < VIEW_WIDTH;
    bool isX2 = chessCenterBlockX + deltaXAbs >= 0 && chessCenterBlockX + deltaXAbs < VIEW_WIDTH;
    int maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN(
      ((float)game->scene.chess.height - (float)deltaXAbs * (float)game->scene.chess.height / ((float)game->scene.chess.radius + 0.5f)) * lengthRatio
    );
    for (int blockDeltaY = 0; blockDeltaY <= maxBlockDeltaY; blockDeltaY++) {
      if (chessCenterBlockY - blockDeltaY < 0) { break; }
      if (chessCenterBlockY - blockDeltaY >= VIEW_MAIN_HEIGHT) { continue; }
      if (isX1) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX - deltaXAbs].color = game->scene.chess.color; }
      if (isX2) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX + deltaXAbs].color = game->scene.chess.color; }
    }
  }
}
