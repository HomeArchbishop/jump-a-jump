#ifndef __TYPE_FRAME_H_
#define __TYPE_FRAME_H_

#include "../global.h"

enum FrameCellType {
  FRAME_CELL_TYPE_PIXEL = 0, FRAME_CELL_TYPE_TEXT
};

typedef struct FrameCell {
  enum FrameCellType type;
  char character;
  int color;
  int background;
} FrameCell;

#endif /* __TYPE_FRAME_H_ */
