#include <stdio.h>
#include <stdbool.h>

#include "../../includes/global.h"
#include "../../includes/types/frame.h"

const wchar_t LOWER_HALF_BLOCK = 0x2584;

FrameCell formerFrame[VIEW_HEADER_HEIGHT + VIEW_MAIN_HEIGHT][VIEW_WIDTH];

bool compareFrameCell (FrameCell* frame1, FrameCell* frame2) {
  if (frame1->type != frame2->type || frame1->color != frame2->color) { return false; }
  if (frame1->type == FRAME_CELL_TYPE_TEXT) { return frame1->color == frame2->color && frame1->character == frame2->character; }
  return true;
}

int needToJumpBlockCnt = 0;

void printFrame (int lineAt, int height, FrameCell frame[][VIEW_WIDTH]) {
  printf("\33[s"); // save cursor position
  if (lineAt >= 2) { printf("\33[%dB", lineAt / 2); } // move down cursor by `lineAt / 2` blocks

  for (int i = 0; i + 1 < height; i += 2) {
    needToJumpBlockCnt = 0;
    for (int j = 0; j < VIEW_WIDTH; j++) {
      if (
        compareFrameCell(&frame[i][j], &formerFrame[i + lineAt][j]) &&
        compareFrameCell(&frame[i + 1][j], &formerFrame[i + lineAt + 1][j])
      ) {
        needToJumpBlockCnt++;
        continue;
      }
      if (needToJumpBlockCnt > 0) { printf("\33[%dC", needToJumpBlockCnt); needToJumpBlockCnt = 0; } // move cursor to next cursor
      formerFrame[i + lineAt][j].type = frame[i][j].type;
      formerFrame[i + lineAt][j].character = frame[i][j].character;
      formerFrame[i + lineAt][j].background = frame[i][j].background;
      formerFrame[i + lineAt][j].color = frame[i][j].color;
      formerFrame[i + lineAt + 1][j].type = frame[i + 1][j].type;
      formerFrame[i + lineAt + 1][j].character = frame[i + 1][j].character;
      formerFrame[i + lineAt + 1][j].background = frame[i + 1][j].background;
      formerFrame[i + lineAt + 1][j].color = frame[i + 1][j].color;

      wchar_t character;
      int printColor;
      int printBgColor;
      if (frame[i][j].type == FRAME_CELL_TYPE_TEXT) {
        character = frame[i][j].character;
        printColor = frame[i][j].color;
        printBgColor = frame[i][j].background;
      } else if (frame[i + 1][j].type == FRAME_CELL_TYPE_TEXT) {
        character = frame[i + 1][j].character;
        printColor = frame[i + 1][j].color;
        printBgColor = frame[i + 1][j].background;
      } else if (frame[i][j].type == FRAME_CELL_TYPE_PIXEL) {
        character = LOWER_HALF_BLOCK;
        printColor = frame[i + 1][j].color;
        printBgColor = frame[i][j].color;
      }

      printf("\033[%d;%dm%lc\033[39;49m", printColor, printBgColor + 10, character);
    }
    printf("\n");
  }

  printf("\33[u"); // recover cursor position
}
