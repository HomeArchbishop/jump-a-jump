#include "../../includes/types/game.h"
#include "../../includes/global.h"

void updateGameScore (Game* game) {
  /* now the chess is on the exact next stage, use the state values directly */
  game->score++;
}
