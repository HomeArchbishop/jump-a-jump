#include <stdbool.h>
#include <math.h>

#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"
#include "../../../includes/types/game.h"

/* create stages */
void _createStages (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game) {
  for (int i = 0; i < game->scene.stagesCnt; i++) {
    int stageBottomBaseY = GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.stages[i].y + game->scene.stages[i].height);
    int centerBlockX = game->scene.stages[i].x;
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
}

/* create chess */
void _createChess (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game, int chessOffsetY) {
  int chessCenterBlockX = game->scene.chess.jumpDirection == GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT ?
    game->scene.chess.x + game->scene.chess.distanceByXY:
    game->scene.chess.x - game->scene.chess.distanceByXY;
  int chessCenterBlockY = GAMEPOS_TO_VIEW__SCALE_Y_FN(game->scene.chess.y - game->scene.chess.distanceByXY) + chessOffsetY;
  for (int deltaXAbs = 0; deltaXAbs <= game->scene.chess.radius; deltaXAbs++) {
    bool isX1 = chessCenterBlockX - deltaXAbs >= 0 && chessCenterBlockX - deltaXAbs < VIEW_WIDTH;
    bool isX2 = chessCenterBlockX + deltaXAbs >= 0 && chessCenterBlockX + deltaXAbs < VIEW_WIDTH;
    int maxBlockDeltaY = GAMEPOS_TO_VIEW__SCALE_Y_FN(
      (float)game->scene.chess.height - (float)deltaXAbs * (float)game->scene.chess.height / ((float)game->scene.chess.radius + 0.5f)
    );
    for (int blockDeltaY = 0; blockDeltaY <= maxBlockDeltaY; blockDeltaY++) {
      if (chessCenterBlockY - blockDeltaY < 0) { break; }
      if (chessCenterBlockY - blockDeltaY >= VIEW_MAIN_HEIGHT) { continue; }
      if (isX1) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX - deltaXAbs].color = game->scene.chess.color; }
      if (isX2) { frame[chessCenterBlockY - blockDeltaY][chessCenterBlockX + deltaXAbs].color = game->scene.chess.color; }
    }
  }
}

bool createGamepageFallFrames (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH], Game* game) {
  static int frameIndex = 0;

  int chessOffsetY = GAMEPOS_TO_VIEW__SCALE_Y_FN(
    (float)game->scene.stages[game->scene.chess.currentStageIndex + 1].height / (float)(ANIMATION__GAMEPAGE_FALL_FRAMES_CNT - 1) * frameIndex
  );

  for (int i = 0; i < VIEW_MAIN_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__BLACK;
    }
  }

  /* notes: `currentStageIndex` has not been changed, it is still the jump-from stage  */
  int _ymin1 = game->scene.stages[game->scene.chess.currentStageIndex].y;
  int _ymax1 = game->scene.stages[game->scene.chess.currentStageIndex].y + game->scene.stages[game->scene.chess.currentStageIndex].radius;
  int _ymin2 = game->scene.stages[game->scene.chess.currentStageIndex + 1].y;
  int _ymax2 = game->scene.stages[game->scene.chess.currentStageIndex + 1].y + game->scene.stages[game->scene.chess.currentStageIndex + 1].radius;
  int _yChess = game->scene.chess.y + game->scene.chess.distanceByXY;
  if (
    (_ymax1 >= _yChess && _ymin1 <= _yChess) || (_ymax2 >= _yChess && _ymin2 <= _yChess)
  ) /* the chess will fall behind the stage, then draw chess fisrt */ {
    _createChess(frame, game, chessOffsetY);
    _createStages(frame, game);
  } else /* otherwise draw stages fisrt */ {
    _createStages(frame, game);
    _createChess(frame, game, chessOffsetY);
  }

  frameIndex = (frameIndex + 1) % ANIMATION__GAMEPAGE_FALL_FRAMES_CNT;
  return frameIndex == 0;
}
