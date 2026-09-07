#pragma once

#include "raylib.h"

bool DebugExampleBool(void);
bool DebugDrawObjectHitboxes(void);
bool DebugDrawCardsSmall(void);
bool DebugShowObjectStack(void);

// I would prefer if the naming convention communicated it was a Discarded value (set to false after calling once)
bool DebugSpawnFactoryDeck(void);
bool DebugSpawnShuffledDeck(void);
bool DebugClearCards(void);
bool DebugCleanupObjects(void);
