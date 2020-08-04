#include "GGoblinProcess.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 10 * FACTOR;

const TInt IDLE_SPEED = 3 * FACTOR;
const TInt TAUNT_SPEED = 3 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 4 * FACTOR;
const TInt DEATH_SPEED = 1;

const TFloat VELOCITY = 1.4 / FACTOR;

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

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(40, IMG_GOBLIN_IDLE + 0),
  ASTEP(4, IMG_GOBLIN_IDLE + 1),
  ASTEP(40, IMG_GOBLIN_IDLE + 2),
  ASTEP(4, IMG_GOBLIN_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_IDLE + 0),
  AEND,
};

// TODO: Finish death animations for all directions. See GRatProcess.cpp for example
static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ADELTA(0, 1),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 1),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 2),
  ADELTA(0, 1),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT attackComboDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_DOWN + 0),

  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),

  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackQuickDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 20, 24, 32),
  ADELTA(2, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(2, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 0),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ADELTA(-9, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 2),
  ADELTA(-8, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackComboLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_RIGHT + 0),
  /*****/
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  /*****/
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackQuickLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),
  /***/
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),
  /***/
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-16, 4, 40, 16),
  ADELTA(-18, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ASIZE(8, 4, 24, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-17, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(-9, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-6, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
  ADELTA(4, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 1),
  ADELTA(2, 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 2),
  ADELTA(-4, 0),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-6, 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT attackComboRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_RIGHT + 0),

  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackQuickRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),

  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 4, 40, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ADELTA(3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 3),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 0),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_UP + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_UP + 0),
  AEND,
};

static ANIMSCRIPT attackComboUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_UP + 3),
  ADELTA(4, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_UP + 0),

  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_UP + 3),

  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackQuickUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED * 8, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 3),

  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 3),

  ADELTA(2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 4, 24, 32),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 4, 24, 16),
  ADELTA(-2, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 3),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT deathUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_DAMAGE_UP + 1),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 3),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 2),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* attackQuickAnimations[] = {
  attackQuickUpAnimation,
  attackQuickDownAnimation,
  attackQuickLeftAnimation,
  attackQuickRightAnimation,
};
static ANIMSCRIPT* attackComboAnimations[] = {
  attackComboUpAnimation,
  attackComboDownAnimation,
  attackComboLeftAnimation,
  attackComboRightAnimation,
};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GGoblinProcess::GGoblinProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aIp, aX, aY, GOBLIN_SLOT, aParams, VELOCITY, ATTR_GOBLIN) {
  mSprite->Name("ENEMY GOBLIN");
  mSprite->w = 24;
  mSprite->cx = 8;
  mSprite->ResetShadow();
  SetStatMultipliers(2.2, 2.5, 2.5);
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW); // Disable shadow
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_GOBLIN_BMP_SPRITES);

  mTaunt = EFalse;
}

GGoblinProcess::~GGoblinProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GGoblinProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GGoblinProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GGoblinProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(3, 8)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GGoblinProcess::Attack(DIRECTION aDirection) {
  TInt attackType = Random() & TUint8(3);

  // 25% chance for either quick or combo attack
  if (attackType < 2) {
    mSprite->StartAnimationInDirection(attackAnimations, aDirection);
  }
  else if (attackType == 2) {
    mSprite->StartAnimationInDirection(attackQuickAnimations, aDirection);
  }
  else {
    mSprite->StartAnimationInDirection(attackComboAnimations, aDirection);
  }
}

void GGoblinProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GGoblinProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GGoblinProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}