#ifndef GENUS_GMAINMENUSTATE_H
#define GENUS_GMAINMENUSTATE_H

//#include "Game.h"

class GMainMenuState : public BGameEngine {
public:
  GMainMenuState();
  virtual ~GMainMenuState();

private:
  BFont *mFont16;
};

#endif //GENUS_GMAINMENUSTATE_H
