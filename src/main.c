#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "../includes/types/frame.h"
#include "../includes/types/game.h"
#include "../includes/types/input.h"

#include "../includes/global.h"

#include "../includes/frames/createEmptyFrame.h"
#include "../includes/frames/homepage/createHomepageFrame.h"
#include "../includes/frames/homepage/createHeaderFrame.h"
#include "../includes/frames/gamepage/createGamepageNormalFrame.h"
#include "../includes/frames/gamepage/createGamepageJumpFrames.h"
#include "../includes/frames/gamepage/createGamepageMoveFrames.h"
#include "../includes/frames/gamepage/createGamepageFallFrames.h"
#include "../includes/frames/gamepage/createGamepageHeaderFrame.h"
#include "../includes/frames/gamepage/createGamepageEndFrame.h"
#include "../includes/frames/gamepage/createGamepageCompressFrame.h"

#include "../includes/print/printHeaderFrame.h"
#include "../includes/print/printMainFrame.h"

#include "../includes/input/readCertainChar.h"
#include "../includes/input/sustainedReadCertainChar.h"

#include "../includes/game/generateNextStage.h"
#include "../includes/game/initializeGame.h"
#include "../includes/game/judgeJumpOnStage.h"
#include "../includes/game/updateChessStateAfterJump.h"
#include "../includes/game/updateStateAfterMove.h"
#include "../includes/game/clearInvisibleStages.h"
#include "../includes/game/updateGameScore.h"

#include "../includes/utils/sleep.h"
#include "../includes/utils/tool.h"

int _processFlag = 1;

typedef struct AllGameInfo {
  int highestScore;
} AllGameInfo;

AllGameInfo allGameInfo = {
  .highestScore = 0
};

int main () {
  /* Preparation */
  setlocale(LC_ALL, "");
  printf("\33[2J\33[0;0H\33[?25l"); // clear screen; move cursor to (0, 0); hide cursor
  srand((unsigned)time(NULL));
  /* Preparation END */

  bool isGameStart = false;

  FrameCell headerFrame[VIEW_HEADER_HEIGHT][VIEW_WIDTH];
  createEmptyFrame(headerFrame, VIEW_HEADER_HEIGHT);
  FrameCell mainFrame[VIEW_MAIN_HEIGHT][VIEW_WIDTH];
  createEmptyFrame(mainFrame, VIEW_MAIN_HEIGHT);

  char inputChar;
  while (_processFlag) {
    createHomepageHeaderFrame(headerFrame, allGameInfo.highestScore);
    createHomepageFrame(mainFrame);
    printHeaderFrame(headerFrame);
    printMainFrame(mainFrame);

    inputChar = readCertainChar(3, KEYCONF__BEGIN_AUTOMATIC_GAME, KEYCONF__BEGIN_GAME, KEYCONF__TERMINATE_PROCESS);
    if (inputChar == KEYCONF__TERMINATE_PROCESS) { _processFlag = 0; break; }
    /* then inputChar == KEYCONF__BEGIN_GAME or KEYCONF__BEGIN_AUTOMATIC_GAME */

    /* A game main logic */
    Game game;
    initializeGame(&game, inputChar == KEYCONF__BEGIN_AUTOMATIC_GAME);

    SustainedReadCertainCharResult sustainedReadCertainCharResult;

    while (!game.isEnd) {
      if (game.isNewGame) {
        createGamepageNormalFrame(mainFrame, &game);
        printMainFrame(mainFrame);
        game.isNewGame = false;
      }

      createGamepageHeaderFrame(headerFrame, game.score);
      printHeaderFrame(headerFrame);

      if (!game.isAutomatical) {
        sustainedReadCertainCharResult.isInputStart = false;
        sustainedReadCertainCharResult.isInputEnd = false;
        sustainedReadCertainCharResult.character = '\0';
        startSustainedReadCertainChar(&sustainedReadCertainCharResult, 2, KEYCONF__IN_GAME__JUMP, KEYCONF__TERMINATE_PROCESS);
        while (!sustainedReadCertainCharResult.isInputEnd) {
          if (
            sustainedReadCertainCharResult.isInputStart && sustainedReadCertainCharResult.character == KEYCONF__IN_GAME__JUMP
          ) /* player is holding the key for jump */ {
            game.scene.chess.distanceByXY = MIN(GAMECONF__MAX_JUMP_DISTANCE_BY_XY, game.scene.chess.distanceByXY + GAMECONF__JUMP_ADD_DISTANCE_PER_HOLD_CHECK);
            createGamepageCompressFrame(mainFrame, &game);
            printMainFrame(mainFrame);
          }
          UNIVERSAL_UTILS__SLEEP(GAMECONF__JUMP_HOLD_CHECK_INTERVAL);
        }
        endSustainedReadCertainChar();
        if (sustainedReadCertainCharResult.character == KEYCONF__TERMINATE_PROCESS) { _processFlag = 0; break; }
        /* then sustainedReadCertainCharResult.character == KEYCONF__IN_GAME__JUMP */
      } else {
        game.scene.chess.distanceByXY = game.scene.stages[game.scene.chess.currentStageIndex].y - game.scene.stages[game.scene.chess.currentStageIndex + 1].y;
      }

      bool isJumpAnimationEnd = false;
      while (!isJumpAnimationEnd) {
        isJumpAnimationEnd = createGamepageJumpFrames(mainFrame, &game);
        printMainFrame(mainFrame);
        if (!isJumpAnimationEnd) { UNIVERSAL_UTILS__SLEEP(ANIMATION__GAMEPAGE_JUMP_LAST_MILLISECOND / ANIMATION__GAMEPAGE_JUMP_FRAMES_CNT); }
      }

      int jumpToStageIndex = judgeJumpOnStage(&game);
      if (jumpToStageIndex == -1) {
        game.isEnd = true;

        bool isFallAnimationEnd = false;
        while (!isFallAnimationEnd) {
          isFallAnimationEnd = createGamepageFallFrames(mainFrame, &game);
          printMainFrame(mainFrame);
          if (!isFallAnimationEnd) { UNIVERSAL_UTILS__SLEEP(ANIMATION__GAMEPAGE_FALL_LAST_MILLISECOND / ANIMATION__GAMEPAGE_FALL_FRAMES_CNT); }
        }

        if (allGameInfo.highestScore < game.score) {
          allGameInfo.highestScore = game.score;
        }

        createGamepageEndFrame(mainFrame);
        printMainFrame(mainFrame);

        inputChar = readCertainChar(4, KEYCONF__GAME_END_RESTART_AUTOMATIC_GAME, KEYCONF__GAME_END_BACK_TO_HOMEPAGE, KEYCONF__TERMINATE_PROCESS, KEYCONF__GAME_END_RESTART);
        if (inputChar == KEYCONF__TERMINATE_PROCESS) { _processFlag = 0; break; }
        if (inputChar == KEYCONF__GAME_END_BACK_TO_HOMEPAGE) { break; }
        if (inputChar == KEYCONF__GAME_END_RESTART) { initializeGame(&game, false); continue; }
        if (inputChar == KEYCONF__GAME_END_RESTART_AUTOMATIC_GAME) { initializeGame(&game, true); continue; }
      }
      /* then jumpToStageIndex != -1, aka the chess will be on `stages[jumpToStageIndex]` */

      if (jumpToStageIndex == game.scene.chess.currentStageIndex) /* jump to same stage */ {
        updateChessStateAfterJump(&game, jumpToStageIndex);
        continue;
      }

      /* then jumpToStageIndex == <nextStageIndex>, aka the chess will be on next stage */

      generateNextStage(&game);
      updateChessStateAfterJump(&game, jumpToStageIndex);

      updateGameScore(&game);

      bool isMoveAnimationEnd = false;
      while (!isMoveAnimationEnd) {
        isMoveAnimationEnd = createGamepageMoveFrames(mainFrame, &game);
        printMainFrame(mainFrame);
        if (!isMoveAnimationEnd) { UNIVERSAL_UTILS__SLEEP(ANIMATION__GAMEPAGE_MOVE_LAST_MILLISECOND / ANIMATION__GAMEPAGE_MOVE_FRAMES_CNT); }
      }

      updateStateAfterMove(&game);

      clearInvisibleStages(&game);

      // game.isEnd = true; // FOR DEV ONLY
    }

    // _processFlag = 0; // FOR DEV ONLY
  }

  printf("\33[?25h"); // show cursor

  return 0;
}
