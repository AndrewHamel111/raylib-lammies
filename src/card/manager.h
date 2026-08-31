#pragma once

#include "card.h"

const Card* GetHeldCard(void);
void CardManagerInit(void);
void CardManagerUpdate(float ft);
void CardManagerDrawAllCards(void);
