#include "../card.h"
#include "utility.h"
#include "resources.h"
#include "constants.h"
#include "external/easings.h"

extern bool debugDrawCardsSmall;

void CardDraw(const Card* card, float alpha)
{
	Rectangle dest = CardGetRect(card);
	float fullWidth = dest.width;
	float t = 1.0f - (card->_flipTime / CARD_FLIP_TIME);

	switch (card->_animationState)
	{
		case CardStateDefault:
			// do nothing
			break;
		case CardStateFlippingDownIn:
		case CardStateFlippingUpIn:
			dest.width = EaseCubicOut(t, fullWidth, -fullWidth, 1.0f);
			dest.x -= 0.5f * (dest.width - fullWidth);
			break;
		case CardStateFlippingDownOut:
		case CardStateFlippingUpOut:
            // TODO: folks say to look into different easings, aight
			dest.width = EaseCubicOut(t, 0, fullWidth, 1.0f);
			dest.x -= 0.5f * (dest.width - fullWidth);
			break;
	}

	if (debugDrawCardsSmall)
	{
		Texture2D tex = card->_faceUp ? GetCardSmall(card) : GetCardBackSmall();
		DrawTexturePro(tex, GetCardSourceSmall(), dest, V(0.5f, 0.5f), 0.0f, Fade(WHITE,alpha));
	}
	else
	{
		Texture2D tex = card->_faceUp ? GetCardLarge(card) : GetCardBackLarge();
		DrawTexturePro(tex, GetCardSourceLarge(), dest, V(0.5f, 0.5f), 0.0f, Fade(WHITE,alpha));
	}
}

Vector2 CardGetSize(const Card* card)
{
    return debugDrawCardsSmall ? (Vector2){80, 116} : (Vector2){168, 240};
}

Rectangle CardGetRect(const Card* card)
{
    Vector2 sz = CardGetSize(card);
	return R(card->_position.x, card->_position.y, sz.x, sz.y);
}

static const char* suit_name_lower[] = {"clubs", "diamonds", "hearts", "spades"};
static const char* suit_name[] = {"Clubs", "Diamonds", "Hearts", "Spades"};

const char* SuitName(Suit suit)
{
	if (suit < 0 || suit > 3)
	{
		return "UNDEFINED";
	}

	return suit_name[suit];
}

const char* SuitName_Lower(Suit suit)
{
	if (suit < 0 || suit > 3)
	{
		return "undefined";
	}

	return suit_name_lower[suit];
}

const char* RankName(Rank rank)
{
	if (rank < -1 || rank > 13)
	{
		return "UNDEFINED";
	}

	if (rank == -1) return "JOKER";
	if (rank == Jack) return "Jack";
	if (rank == Queen) return "Queen";
	if (rank == King) return "King";
	if (rank == Ace) return "A";
	if (rank == Ten) return "10";
	return TextFormat("%c", '1' + rank);
}

unsigned int CardAsInt(Card card)
{
	return (card.suit * 13) + card.rank;
}
