#include "GFinalBossDeathProcess.h"
#include "GGameState.h"

const TInt16 DEATH_SPEED = 8;

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_DEATH_SLOT),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 0),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 1),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 2),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 3),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 4),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 5),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 6),
  ASTEP(DEATH_SPEED, IMG_WIZARD_DEATH + 7),
  AEND,
};

GFinalBossDeathProcess::GFinalBossDeathProcess(GGameState *aGameState, GFinalBossProcess *aFinalBoss, TFloat aX, TFloat aY, TInt aDelay)
    : GProcess(ATTR_GONE) {
  mGameState = aGameState;
  mFinalBoss = aFinalBoss;
  mX = aX - 16 + Random(0, 18 / 2);
  mY = aY + Random(0, 75 / 2);
  mTimer = aDelay * DEATH_SPEED * 3;
  mSprite = ENull;
}

GFinalBossDeathProcess::~GFinalBossDeathProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  mFinalBoss->DeathAnimationDone();
}

TBool GFinalBossDeathProcess::RunBefore() {
  if (--mTimer <= 0 && !mSprite) {
    mSprite = new BAnimSprite(ENEMY_DEATH_PRIORITY, ENEMY_DEATH_SLOT, 0, STYPE_DEFAULT);
    mSprite->x = mX;
    mSprite->y = mY;
    mSprite->SetFlags(SFLAG_BELOW);
    mGameState->AddSprite(mSprite);
    mSprite->StartAnimation(deathAnimation);
  }
  return ETrue;
}

TBool GFinalBossDeathProcess::RunAfter() {
  if (mSprite && mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}
