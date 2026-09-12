#pragma once

#include "raylib.h"
#include "object.h"

typedef enum Suit
{
	Clubs = 0,
	Diamonds,
	Hearts,
	Spades
} Suit;

typedef enum Rank
{
	Ace = 0,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Joker = -1
} Rank;

void CardTick(Object* card, float ft);
void CardDraw(const Object* card);
void CardDrawHighlight(const Object* object, Color highlight);
void CardDrawShadowed(const Object* object);
void CardDrawCustom(Vector2 position, int value, float rotation, Color highlight);

Vector2 CardGetSize(void);
Rectangle CardGetRect(const Object* object);

Suit CardSuit(const Object* object);
Rank CardRank(const Object* object);

Suit GetSuit(int value);
Rank GetRank(int value);

const char* SuitName(Suit suit);
const char* SuitName_Lower(Suit suit);
const char* RankName(Rank rank);
