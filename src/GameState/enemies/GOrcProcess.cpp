#include "GOrcProcess.h"
#include "GStatProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 120;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 1.5 / FACTOR;

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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(40, IMG_ORC_IDLE),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ASTEP(40, IMG_ORC_IDLE + 2),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED*4, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 20),
  ASIZE(0,24,32,48),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,0,32,32),
  ADELTA(0, 24),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_DOWN + 0),
  AEND,
}; // 20 24 18

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),//
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(-22, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-32, 0, 40, 32),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(-22, 0),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 32),
  ATYPE(STYPE_ENEMY),
  ADELTA(-22, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_ORC_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 0),//
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ADELTA(20, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 64, 32),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ADELTA(20, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 32),
  ATYPE(STYPE_ENEMY),
  ADELTA(20, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_ORC_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_ORC_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ADELTA(0, 6),
  ASTEP(ATTACK_SPEED*4, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(0, 10),
  ASIZE(0,0,32,48),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,0,32,32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  ASTEP(ATTACK_SPEED, IMG_ORC_WALK_UP + 0),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AEND,
};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GOrcProcess::GOrcProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParams)
  : GEnemyProcess(aGameState, ORC_SLOT, aParams) {
  mStateTimer = 0;
  mSprite->Name("ORC SPRITE");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mHitStrength = HIT_HARD;

  mAttackTimer = 1;
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GOrcProcess::~GOrcProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GOrcProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mSprite->type = STYPE_ENEMY;
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;

  switch (aState) {

    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStateTimer = IDLE_TIMEOUT;
      break;

    case WALK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;

      if (mStateTimer <= 0) {
        mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
      }

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
          //          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;

    case ATTACK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
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
      break;

    case HIT_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
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
      break;

    case DEATH_STATE:
      mSprite->StartAnimation(deathAnimation);
      break;

    default:
      break;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GOrcProcess::CanWalk(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      return mSprite->IsFloor(DIRECTION_UP, 0, -VELOCITY);
    case DIRECTION_DOWN:
      return mSprite->IsFloor(DIRECTION_DOWN, 0, VELOCITY);
    case DIRECTION_LEFT:
      return mSprite->IsFloor(DIRECTION_LEFT, -VELOCITY, 0);
    case DIRECTION_RIGHT:
    default:
      return mSprite->IsFloor(DIRECTION_RIGHT, VELOCITY, 0);
  }
}

static DIRECTION random_direction() {
  TInt dir = Random() & TUint8(3);
  switch (dir) {
    case 0:
      return DIRECTION_UP;
    case 1:
      return DIRECTION_DOWN;
    case 2:
      return DIRECTION_LEFT;
    case 3:
    default:
      return DIRECTION_RIGHT;
  }
}

TBool GOrcProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 0) {
    // Set distance to walk for WALK_STATE
    mStateTimer = TInt16(TFloat(Random(1, 8)) * 32 / VELOCITY);

    for (TInt retries = 0; retries < 8; retries++) {
      // Don't go the same direction
      DIRECTION direction = random_direction();
//      while (direction == mSprite->mDirection) {
//        direction = random_direction();
//      }

      if (CanWalk(direction)) {
        NewState(WALK_STATE, direction);
        return ETrue;
      }
    }

    // after 8 tries, we couldn't find a direction to walk.
    NewState(IDLE_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GOrcProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }
  if (--mStateTimer < 0) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (!mSprite->IsFloor(mSprite->mDirection, mSprite->vx, mSprite->vy)) {
    printf("ORC hit wall\n");
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->cType & STYPE_PLAYER) {
    printf("ORC hit player\n");
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mSprite->mDirection);
  }

  return ETrue;
}

