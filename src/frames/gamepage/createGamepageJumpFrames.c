#include <stdbool.h>
#include <math.h>

#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"
#include "../../../includes/types/game.h"

bool createGamepageJumpFrames (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game) {
  static int frameIndex = 0;

  static struct FrameInfo {
    float x; float y; int theta; float len; float stageH;
  } frameBlockInfo [ANIMATION__GAMEPAGE_JUMP_FRAMES_CNT] = {
    { .x = 0.0f, .y = 0.0f, .theta = 0, .len = 1.0f, .stageH = 1.0f },
    { .x = 0.124f, .y = 1.1f, .theta = 52, .len = 1.07f, .stageH = 1.0f },
    { .x = 0.518f, .y = 2.038f, .theta = 194, .len = 1.0f, .stageH = 1.0f },
    { .x = 0.830f, .y = 2.098f, .theta = 246, .len = 0.857f, .stageH = 1.0f },
    { .x = 0.938f, .y = 1.472f, .theta = 357, .len = 0.928f, .stageH = 1.0f },
    { .x = 1.0f, .y = 1.0f, .theta = 0, .len = 1.0f, .stageH = 1.0f }
  };

  if (frameIndex == 0) /* each first frame is diverse due to the `distanceByXY` (aka the chess compression) */ {
    frameBlockInfo[0].stageH = (float)(
      1.0 - (
        (double)game->scene.chess.distanceByXY / (double)(GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMECONF__MIN_JUMP_DISTANCE_BY_XY)
        * (double)(game->scene.stages[game->scene.chess.currentStageIndex].height - GAMEPOS__STAGE_MIN_COMPRESSED_HEIGHT)
        / (double)game->scene.stages[game->scene.chess.currentStageIndex].height
      )
    );
    frameBlockInfo[0].len = (float)(
      1.0 - (
        (double)game->scene.chess.distanceByXY / (double)(GAMECONF__MAX_JUMP_DISTANCE_BY_XY - GAMECONF__MIN_JUMP_DISTANCE_BY_XY)
        * (double)(game->scene.chess.height - GAMEPOS__CHESS_MIN_COMPRESSED_HEIGHT) / (double)game->scene.chess.height
      )
    );
    frameBlockInfo[0].y = -(float)(
      (1.0f - frameBlockInfo[0].stageH) * (float)game->scene.stages[game->scene.chess.currentStageIndex].height / (double)(game->scene.chess.distanceByXY)
    );
  }

  for (int i = 0; i < VIEW_MAIN_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__BLACK;
    }
  }

  /* create stages */
  for (int i = 0; i < game->scene.stagesCnt; i++) {
    bool isChessCurrentOnThisStage = i == game->scene.chess.currentStageIndex;
    int stageBottomBaseY = isChessCurrentOnThisStage ?
      GAMEPOS_TO_VIEW__SCALE_Y_FN((float)game->scene.stages[i].y + game->scene.stages[i].height + (1.0f - frameBlockInfo[frameIndex].stageH) * (float)game->scene.stages[i].height) :
      GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].y + game->scene.stages[i].height);
    int centerBlockX = game->scene.stages[i].x;
    int thisStageHeight = isChessCurrentOnThisStage ?
      GAMEPOS_TO_VIEW__SCALE_Y_FN(frameBlockInfo[frameIndex].stageH * (float)game->scene.stages[i].height) :
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
  int chessCenterBlockX = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT ?
    (int)((float)game->scene.chess.x + frameBlockInfo[frameIndex].x * (float)game->scene.chess.distanceByXY) :
    (int)((float)game->scene.chess.x - frameBlockInfo[frameIndex].x * (float)game->scene.chess.distanceByXY);
  int chessCenterBlockY = GAMEPOS_TO_VIEW__SCALE_Y_FN((float)game->scene.chess.y - frameBlockInfo[frameIndex].y * (float)game->scene.chess.distanceByXY);
  for (int deltaXAbs = 0; deltaXAbs <= game->scene.chess.radius; deltaXAbs++) {
    bool isX1 = chessCenterBlockX - deltaXAbs >= 0 && chessCenterBlockX - deltaXAbs < VIEW_WIDTH;
    bool isX2 = chessCenterBlockX + deltaXAbs >= 0 && chessCenterBlockX + deltaXAbs < VIEW_WIDTH;
    int maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN(
      ((float)game->scene.chess.height - (float)deltaXAbs * (float)game->scene.chess.height / ((float)game->scene.chess.radius + 0.5f))
      * frameBlockInfo[frameIndex].len
    );
    for (int blockDeltaY = 0; blockDeltaY <= maxBlockDeltaY; blockDeltaY++) {
      if (chessCenterBlockY - blockDeltaY < 0) { break; }
      if (chessCenterBlockY - blockDeltaY >= VIEW_MAIN_HEIGHT) { continue; }
      if (isX1) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX - deltaXAbs].color = game->scene.chess.color; }
      if (isX2) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX + deltaXAbs].color = game->scene.chess.color; }
    }
  }

  frameIndex = (frameIndex + 1) % ANIMATION__GAMEPAGE_JUMP_FRAMES_CNT;
  return frameIndex == 0;
}
