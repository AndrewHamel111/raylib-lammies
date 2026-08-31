#pragma once

#include "card.h"

Card* GetCards(int* outCount);
Card* CreateCard(Vector2 position, Suit suit, Rank rank);
Card* AddCard(Card card);
Card* AddCardValue(unsigned int value);
void DeleteCard(Card* card);
void DeleteCardAt(int index);
void DeleteAllCards(void);
Card* MoveCardToTop(Card* card);
Card* MoveCardAtToTop(int index);

Card* MousePickCard(Vector2 mpos);
