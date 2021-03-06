#include "GStairsProcess.h"
#include "GPlayer.h"

GStairsProcess::GStairsProcess(GGameState *aGameState, TInt aIp, DIRECTION aDirection, TInt aParams, TFloat aX,
                               TFloat aY, const char *aKind)
  : GEnvironmentProcess(aGameState, aIp, aParams, aX, aY) {
  mSprite = mSprite2 = ENull;
  mDirection = aDirection;
  mGameState = aGameState;
  mLevel = aParams;

  TBool isWood = strcmp(aKind, "WOOD") == 0;
  if (mDirection == DIRECTION_UP) {
    mAttribute = isWood ? ATTR_WOOD_STAIRS_UP : ATTR_STONE_STAIRS_UP;
    const int img = isWood ? IMG_WOOD_STAIRS_UP : IMG_STONE_STAIRS_UP;
    mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img, STYPE_OBJECT);
    mSprite->Name(isWood ? "ENVIRONMENT WOOD STAIRS UP" : "ENVIRONMENT STONE STAIRS UP");
//    mSprite->w = mSprite->h = 32;
    mSprite->h = 32;
    mSprite->w = 32;
    mSprite->cx = -16;
//    mSprite->cy = 0;

    mSprite->x = aX;
    mSprite->y = aY + 32;
//    mSprite->w = mSprite->h = 32;
    mSprite->SetCMask(STYPE_PLAYER | STYPE_ENEMY);
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
    mGameState->AddSprite(mSprite);

    mSprite2 = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img - 10);
    mSprite2->Name(isWood ? "IGNORE WOOD STAIRS UP 2" : "IGNORE STONE STAIRS UP 2");
    mSprite2->SetFlags(SFLAG_BELOW);
    mSprite2->SetCMask(STYPE_PLAYER | STYPE_ENEMY);
    mSprite2->cx = -16;
//    mSprite2->cy = -16;
    mSprite2->w = mSprite2->h = 32;
    mSprite2->x = aX;
    mSprite2->y = aY;
    mSprite2->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
    mGameState->AddSprite(mSprite2);
  }
  else if (!strcasecmp(aKind, "DUNGEON")) {
    const int img = isWood ? IMG_STONE_STAIRS_DOWN : IMG_WOOD_STAIRS_DOWN;
    mAttribute = ATTR_DUNGEON_ENTRANCE;
    mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img, STYPE_OBJECT);
    mSprite->Name("DUNGEON ENTRANCE");

    mSprite->h = 8;
    mSprite->w = 8;
    mSprite->cx = 8;
    mSprite->cy = -8;

    mSprite->x = aX;
    mSprite->y = aY + 32;
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->cMask = STYPE_PLAYER;
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
    mGameState->AddSprite(mSprite);
    mSprite->ClearFlags(SFLAG_RENDER);
  }
  else {
    // DOWN //
    const int img = isWood ? IMG_WOOD_STAIRS_DOWN : IMG_STONE_STAIRS_DOWN;
    mAttribute = isWood ? ATTR_WOOD_STAIRS_DOWN : ATTR_STONE_STAIRS_DOWN;
    mSprite = new GAnchorSprite(mGameState, STAIRS_PRIORITY, ENVIRONMENT_SLOT, img, STYPE_OBJECT);
    mSprite->Name(isWood ? "OVERWORLD WOOD STAIRS DOWN" : "OVERWORLD STONE STAIRS DOWN");
//    mSprite->cx = -16;
//    mSprite->w = mSprite->h = 32;
    mSprite->h = 8;
    mSprite->w = 8;
    mSprite->cx = 8;
    mSprite->cy = -12;
    mSprite->x = aX;
    mSprite->y = aY + 32;
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->SetCMask(STYPE_PLAYER | STYPE_ENEMY);
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
    mGameState->AddSprite(mSprite);
  }

  if (mAttribute == ATTR_DUNGEON_EXIT_FROM_LEVEL_1) {
    mSprite->ClearFlags(SFLAG_RENDER);
  }
}

GStairsProcess::~GStairsProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GStairsProcess::RunBefore() {
  return ETrue;
}

TBool GStairsProcess::RunAfter() {
  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->type = STYPE_DEFAULT;
    mSprite->cType = 0;
    mSprite->cMask = 0;
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->ClearFlags(SFLAG_CHECK);
    if (mSprite2) {
      mSprite2->type = STYPE_DEFAULT;
      mSprite2->cType = 0;
      mSprite2->cMask = 0;
      mSprite2->ClearFlags(SFLAG_CHECK);
    }

    TInt16 dungeon = TUint16(mLevel >> 8) & 0xff,
      level = TUint16(mLevel & 0xff);


    if (level == 10) {
#ifdef DEBUGME
      printf("USE STAIRS up to OVERWOLD\n");
#endif
      mGameState->NextLevel(OVERWORLD_DUNGEON, mGameState->LastOverworldLevel());
    }
    else {
#ifdef DEBUGME
      printf("USE STAIRS down to dungeon %d level %d\n", dungeon, level);
#endif
      mGameState->NextLevel(dungeon == 0 ? -1 : dungeon,level);
    }
    GPlayer::mSprite->ClearCType(STYPE_OBJECT);
  }
  mSprite->cType = 0;
  return ETrue;
}
