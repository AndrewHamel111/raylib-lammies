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

// Object interface
void CardTick(Object* card, float ft);
void CardDraw(const Object* card);
Rectangle CardRect(const Object* card);
ObjectCombinationResult CardCombine(Object* source, Object* destination);
bool CardFlippable(const Object* object);
//void CardFlip(Object* object); // defined in object/card/animation.h
void CardHandlePickup(Object* object);
void CardHandleDrop(Object* object);
ObjectInteractionResult CardHandleInput(Object* object, Input input);
void CardHandlePicked(Object* object, bool picked);

void CardDrawHighlight(const Object* object, Color highlight);
void CardDrawShadowed(const Object* object);
void CardDrawCustom(Vector2 position, int value, float rotation, Color highlight);
void CardDrawBack(Vector2 position, float rotation, Color highlight);

Vector2 CardGetSize(void);

Suit CardSuit(const Object* object);
Rank CardRank(const Object* object);

Suit GetSuit(int value);
Rank GetRank(int value);

const char* SuitName(Suit suit);
const char* SuitName_Lower(Suit suit);
const char* RankName(Rank rank);
