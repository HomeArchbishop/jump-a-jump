#include "../../includes/global.h"
#include "../../includes/types/frame.h"
#include "../../includes/print/printFrame.h"

void printHeaderFrame (FrameCell frame[VIEW_HEADER_HEIGHT][VIEW_WIDTH]) {
  printFrame(0, VIEW_HEADER_HEIGHT, frame);
}
