#include <GResources.h>
#include "GPlayerBulletProcess.h"
#include "GPlayer.h"

const TInt VELOCITY = 4;
const TInt VELOCITY_WITH_GLOVES = VELOCITY + 5;

class BulletSprite : public GAnchorSprite {
public:
    EXPLICIT BulletSprite(GGameState *aGameState) : GAnchorSprite(aGameState, 0, PLAYER_SLOT) {
      Name("PLAYER BULLET");
      type = STYPE_PBULLET;
      SetCMask(STYPE_ENEMY | STYPE_OBJECT | STYPE_EBULLET);
      SetFlags(SFLAG_CHECK);
      ClearFlags(SFLAG_RENDER | SFLAG_ANCHOR);
    }
};

GPlayerBulletProcess::GPlayerBulletProcess(GGameState *aGameState, DIRECTION aDirection, TFloat aMultiplier)
    : GProcess(ATTR_GONE) {
  mSprite = new BulletSprite(aGameState);
  mSprite->mAttackStrength = GPlayer::mAttackStrength * aMultiplier;

  mSprite->MoveInDirection(GPlayer::mEquipped.mGloves ? VELOCITY_WITH_GLOVES : VELOCITY, aDirection, ETrue);
  mSprite->mDirection = aDirection;
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->w = 30;
      mSprite->h = 8;
      mRange = GPlayer::mEquipped.mGloves ? 20 : 42;
      break;
    case DIRECTION_DOWN:
      mSprite->w = 30;
      mSprite->h = 8;
      mRange = GPlayer::mEquipped.mGloves ? 14 : 27;
      break;
    case DIRECTION_LEFT:
    case DIRECTION_RIGHT:
      mSprite->w = 8;
      mSprite->h = 30;
      mSprite->cy = -8;
      mRange = GPlayer::mEquipped.mGloves ? 19 : 39;
      break;
    default:
      mRange = 0;
      break;
  }

  mSprite->x = GPlayer::mSprite->x - mSprite->w / 2 + 32;
  mSprite->y = GPlayer::mSprite->y - mSprite->h / 2 - 8;
  mAge = 0;
  aGameState->AddSprite(mSprite);
}

GPlayerBulletProcess::~GPlayerBulletProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPlayerBulletProcess::RunBefore() {
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
  if (!mSprite->mGameState->mGamePlayfield->IsFloor(mSprite->x + mSprite->w / 2, mSprite->y + mSprite->h / 2)) {
    return EFalse;
  }
  if (mSprite->TestAndClearCType(STYPE_ENEMY | STYPE_EBULLET)) {
    return EFalse;
  }
  mAge++;
  if (mAge * VELOCITY > mRange) {
    return EFalse;
  }
  if (mSprite->TestAndClearCType(STYPE_OBJECT)) {
    mSprite->ClearCMask(STYPE_OBJECT);
  }
  return ETrue;
}
