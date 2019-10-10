#include "GBatProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 5 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 1.5 / TFloat(FACTOR);

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */

ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(40, IMG_BAT_IDLE),
  ASTEP(4, IMG_BAT_IDLE + 1),
  ASTEP(40, IMG_BAT_IDLE + 2),
  ASTEP(4, IMG_BAT_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_DOWN + 0),
  AEND,
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 2),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 2),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  AEND,
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 2), ALOOP,};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AEND,
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */

static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 2), ALOOP,};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AEND,
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */

static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(BAT_SLOT), ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_IDLE + 2), ALOOP,};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 2),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 3),
  AEND,
};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GBatProcess::GBatProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParams)
  : GEnemyProcess(aGameState, BAT_SLOT, aParams, VELOCITY) {
  mStateTimer = 0;
  mSprite->Name("BAT SPRITE");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mHitPoints = mHitPoints;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GBatProcess::~GBatProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/


void GBatProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
}

void GBatProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      mSprite->vy = -VELOCITY;
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(
        mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(
        mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
  }
}

void GBatProcess::Attack(DIRECTION aDirection) {
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(attackUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(attackDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(attackLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(attackRightAnimation);
      break;
  }
}

void GBatProcess::Hit(DIRECTION aDirection) {
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(hitUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(hitDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(hitLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(hitRightAnimation);
      break;
  }
}

void GBatProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}
