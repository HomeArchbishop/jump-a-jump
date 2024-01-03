#include "../../includes/global.h"
#include "../../includes/types/frame.h"
#include "../../includes/print/printFrame.h"

void printMainFrame (FrameCell frame[VIEW_MAIN_HEIGHT][VIEW_WIDTH]) {
  printFrame(VIEW_HEADER_HEIGHT, VIEW_MAIN_HEIGHT, frame);
}
