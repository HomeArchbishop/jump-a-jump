#include <stdbool.h>
#include <math.h>

#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"
#include "../../../includes/types/game.h"

bool createGamepageMoveFrames (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game) {
  static int frameIndex = 0;

  static int anchorX;
  static int anchorY;
  static float moveOffsetStepX;
  static float moveOffsetStepY;

  if (frameIndex == 0) /* each time the animation refreshs, re-judge the target anchor */ {
    anchorX = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT ?
      GAMEPOS__CHESS_ANCHOR_RIGHT_X:
      GAMEPOS__CHESS_ANCHOR_LEFT_X;
    anchorY = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT ?
      GAMEPOS__CHESS_ANCHOR_RIGHT_Y:
      GAMEPOS__CHESS_ANCHOR_LEFT_Y;
    moveOffsetStepX = (float)(anchorX - game->scene.chess.x) / (ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT - 1);
    moveOffsetStepY = (float)(anchorY - game->scene.chess.y) / (ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT - 1);
  }

  float moveOffsetX = moveOffsetStepX * (float)frameIndex + 0.5f;
  float moveOffsetY = moveOffsetStepY * (float)frameIndex + 0.5f;

  for (int i = 0; i < VIEW_MAIN_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__BLACK;
    }
  }

  /* create stages */
  for (int i = 0; i < game->scene.stagesCnt; i++) {
    int stageBottomBaseY = frameIndex == ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT - 1 /* last frame */ ?
      GAMEPOS_TO_VIEW__SCALE_Y_FN(anchorY + game->scene.stages[i].y - game->scene.chess.y + game->scene.stages[i].height) :
      GAMEPOS_TO_VIEW__SCALE_Y_FN((float)(game->scene.stages[i].y + game->scene.stages[i].height) + moveOffsetY);
    int centerBlockX = frameIndex == ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT - 1 /* last frame */ ?
      anchorX + game->scene.stages[i].x - game->scene.chess.x :
      (int)((float)game->scene.stages[i].x + moveOffsetX + 0.5f);
    int thisStageHeight = GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].height);
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
  int chessCenterBlockX = game->scene.chess.x + moveOffsetX;
  int chessCenterBlockY = GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.chess.y + moveOffsetY);
  for (int deltaXAbs = 0; deltaXAbs <= game->scene.chess.radius; deltaXAbs++) {
    bool isX1 = chessCenterBlockX - deltaXAbs >= 0 && chessCenterBlockX - deltaXAbs < VIEW_WIDTH;
    bool isX2 = chessCenterBlockX + deltaXAbs >= 0 && chessCenterBlockX + deltaXAbs < VIEW_WIDTH;
    int maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN((float)game->scene.chess.height - (float)deltaXAbs * (float)game->scene.chess.height / ((float)game->scene.chess.radius + 0.5f));
    for (int blockDeltaY = 0; blockDeltaY <= maxBlockDeltaY; blockDeltaY++) {
      if (chessCenterBlockY - blockDeltaY < 0) { break; }
      if (chessCenterBlockY - blockDeltaY >= VIEW_MAIN_HEIGHT) { continue; }
      if (isX1) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX - deltaXAbs].color = game->scene.chess.color; }
      if (isX2) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX + deltaXAbs].color = game->scene.chess.color; }
    }
  }

  frameIndex = (frameIndex + 1) % ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT;
  return frameIndex == 0;
}
