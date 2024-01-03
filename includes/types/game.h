#ifndef __TYPE_GAME_H_
#define __TYPE_GAME_H_

#include <stdbool.h>

enum GameSceneStageType {
  GAME_SCENE_STAGE_TYPE_CIRCLE = 0,
  GAME_SCENE_STAGE_TYPE_SQUARE
};

typedef struct GameSceneStage {
  enum GameSceneStageType type;
  int height;
  int radius;
  int color;
  int bodyColor;
  int x;
  int y;
} GameSceneStage;

enum GameSceneChessJumpDirection {
  GAME_SCENE_CHESS_JUMP_DIRECTION_LEFT = 0,
  GAME_SCENE_CHESS_JUMP_DIRECTION_RIGHT
};

typedef struct GameSceneChess {
  int height;
  int radius;
  int color;
  int x;
  int y;
  int distanceByXY;
  enum GameSceneChessJumpDirection jumpDirection;
  int currentStageIndex;
} GameSceneChess;

typedef struct GameScene {
  GameSceneStage stages[10];
  int stagesCnt;
  GameSceneChess chess;
} GameScene;

typedef struct Game {
  int score;
  bool isNewGame;
  bool isEnd;
  bool isAutomatical;
  GameScene scene;
} Game;

#endif /* __TYPE_GAME_H_ */
