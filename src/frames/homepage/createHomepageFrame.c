#include <stddef.h>
#include <string.h>

#include "../../../includes/global.h"
#include "../../../includes/types/frame.h"

void createHomepageFrame (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH]) {
  for (int i = 0; i < VIEW_MAIN_HEIGHT; i++) {
    for (int j = 0; j < VIEW_WIDTH; j++) {
      frame[i][j].type = FRAME_CELL_TYPE_PIXEL;
      frame[i][j].color = COLOR__BLACK;
    }
  }

  char str1[] = "JUMP A JUMP";
  int length1 = strlen(str1);
  int halfCol1 = (VIEW_WIDTH - length1) / 2;
  for (int j = 0; j < length1; j++) {
    frame[VIEW_MAIN_HEIGHT / 2 - 6][j + halfCol1].type = FRAME_CELL_TYPE_TEXT;
    frame[VIEW_MAIN_HEIGHT / 2 - 6][j + halfCol1].character = (int)str1[j];
    frame[VIEW_MAIN_HEIGHT / 2 - 6][j + halfCol1].color = COLOR__RED;
    frame[VIEW_MAIN_HEIGHT / 2 - 6][j + halfCol1].background = COLOR__BLACK;
  }

  char str2[] = "--------- TYPE TO CONTINUE ---------";
  int length2 = strlen(str2);
  int halfCol2 = (VIEW_WIDTH - length2) / 2;
  for (int j = 0; j < length2; j++) {
    frame[VIEW_MAIN_HEIGHT / 2 - 2][j + halfCol2].type = FRAME_CELL_TYPE_TEXT;
    frame[VIEW_MAIN_HEIGHT / 2 - 2][j + halfCol2].character = (int)str2[j];
    frame[VIEW_MAIN_HEIGHT / 2 - 2][j + halfCol2].color = COLOR__WHITE;
    frame[VIEW_MAIN_HEIGHT / 2 - 2][j + halfCol2].background = COLOR__BLACK;
  }

  char str3[] = "(s)tart (a)uto-gaming (q)uit";
  int length3 = strlen(str3);
  int halfCol3 = (VIEW_WIDTH - length3) / 2;
  for (int j = 0; j < length3; j++) {
    frame[VIEW_MAIN_HEIGHT / 2][j + halfCol3].type = FRAME_CELL_TYPE_TEXT;
    frame[VIEW_MAIN_HEIGHT / 2][j + halfCol3].character = (int)str3[j];
    frame[VIEW_MAIN_HEIGHT / 2][j + halfCol3].color = COLOR__WHITE;
    frame[VIEW_MAIN_HEIGHT / 2][j + halfCol3].background = COLOR__BLACK;
  }
}
