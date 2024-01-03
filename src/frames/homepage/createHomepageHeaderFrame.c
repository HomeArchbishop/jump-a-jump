#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"

void createHomepageHeaderFrame (FrameCell frame[VIEW_HEADER_HEIGHT][VIEW_WIDTH], int highestScore) {
  for (int i = 0; i < VIEW_HEADER_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__YELLOW;
    }
  }
  int j = VIEW_WIDTH - 2;
  do {
    frame[0][j].type = FRAME_CELL_TYPE_TEXT;
    frame[0][j].character = highestScore % 10 + '0';
    frame[0][j].color = COLOR__BLACK;
    frame[0][j].background = COLOR__YELLOW;
    j--; highestScore /= 10;
  } while (highestScore > 0);
  char str[] = " :EROCS TSEHGIH";
  for (int i = 0; i < 15; i++) {
    frame[0][j - i].type = FRAME_CELL_TYPE_TEXT;
    frame[0][j - i].character = str[i];
    frame[0][j - i].color = COLOR__BLACK;
    frame[0][j - i].background = COLOR__YELLOW;
  }
}
