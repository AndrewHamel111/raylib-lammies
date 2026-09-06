#pragma once

#include "raylib.h"

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

typedef enum CardAnimationState
{
	CardStateDefault = 0,
	CardStateFlippingUpIn,
	CardStateFlippingUpOut,
	CardStateFlippingDownIn,
	CardStateFlippingDownOut,
} CardAnimationState;

typedef struct Card
{
    Vector2 _position;
	int id;
    bool _locked;
	CardAnimationState _animationState;
	bool _faceUp;
	float _flipTime;

    Suit suit;
    Rank rank;
    Color color;
} Card;

void CardDraw(const Card* card, float alpha);
void CardDrawHighlight(const Card* card, float alpha, Color highlight);
void CardDrawShadowed(const Card* card, float alpha);
void CardDrawCustom(Vector2 position, Rank rank, Suit suit, float rotation, float alpha, Color highlight);

Vector2 CardGetSize(const Card* card);
Rectangle CardGetRect(const Card* card);

const char* SuitName(Suit suit);
const char* SuitName_Lower(Suit suit);
const char* RankName(Rank rank);

unsigned int CardAsInt(Card card);
