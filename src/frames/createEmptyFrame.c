#include <stddef.h>

#include "../../includes/types/frame.h"

void createEmptyFrame (FrameCell frame[][VIEW_WIDTH], int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].character = '\0';
      frame[i][j].color = COLOR__BLACK;
      frame[i][j].background = COLOR__BLACK;
    }
  }
}
