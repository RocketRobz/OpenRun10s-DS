#ifndef PLAYER_H
#define PLAYER_H

extern void initPlayers(void);
extern void setPlayerPosition(int x, int y);
extern void playerGraphicLoad(void);
extern void playerLoop(int pressed, int held);
extern void renderPlayer(bool top);

#endif	// PLAYER_H