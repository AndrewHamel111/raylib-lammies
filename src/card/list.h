#pragma once

#include "card.h"

Card* GetCards(int* outCount);
Card** GetCardsOrdered(int* outCount);
Card* CreateCard(Vector2 position, Suit suit, Rank rank);
Card* AddCardValue(unsigned int value);
void DeleteCard(Card* card);
Card* RemoveCardAt(int index);
void DeleteAllCards(void);
Card* MoveCardToTop(Card* card);
Card* MoveCardAtToTop(int index);

Card* MousePickCard(Vector2 mpos);
Card* MousePickCardExcluding(Vector2 mpos, Card* excluded);
