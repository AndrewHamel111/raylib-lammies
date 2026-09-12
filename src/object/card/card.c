#include "object/card.h"
#include "resources.h"
#include "raymath.h"

#include "constants.h"
#include "utility.h"
#include "external/easings.h"
#include "debug.h"
#include "resources.h"
#include "object/management.h"

Object* ObjectCreateCard(Vector2 position, int value)
{
	Object* object = ObjectConstruct();
	object->type = ObjectCard;
	object->_position = position;

	object->data.card.value = value;

	return object;
}

void CardTick(Object* card, float ft)
{
	if (card->data.card._animationState != CardStateDefault)
	{
		// TODO: cards are not consistently flipping at the same speed.. why?
		card->data.card._flipTime -= ft;
		if (card->data.card._flipTime < 0)
		{
			if (card->data.card._animationState == CardStateFlippingDownIn || card->data.card._animationState == CardStateFlippingUpIn)
			{
				card->data.card._faceUp = ! card->data.card._faceUp;
				card->data.card._flipTime = CARD_FLIP_TIME;
				card->data.card._animationState++;
			}
			else
			{
				card->data.card._animationState = CardStateDefault;
			}
		}
	}
}

static void CardDrawInternal(const Object* card, Color highlight, bool shadowed)
{
	Rectangle dest = CardGetRect(card);
	float fullWidth = dest.width;
	float t = 1.0f - (card->data.card._flipTime / CARD_FLIP_TIME);

	switch (card->data.card._animationState)
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
			// May need to use one continuous easing for the whole motion
			dest.width = EaseCubicOut(t, 0, fullWidth, 1.0f);
			dest.x -= 0.5f * (dest.width - fullWidth);
			break;
	}

	Rectangle highlightDest = dest;
	highlightDest.x -= CARD_HIGHLIGHT_EXTENT;
	highlightDest.y -= CARD_HIGHLIGHT_EXTENT;
	highlightDest.width += 2* CARD_HIGHLIGHT_EXTENT;
	highlightDest.height += 2* CARD_HIGHLIGHT_EXTENT;

	Rectangle shadowDest = dest;
	if (shadowed)
	{
		dest.x -= CARD_SHADOW_OFFSET;
		dest.y -= CARD_SHADOW_OFFSET;
		DrawRectangleRounded(shadowDest, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, Fade(BLACK, CARD_SHADOW_DARKNESS));
	}

	bool small = DebugDrawCardsSmall();
	if (!ColorIsEqual(highlight, BLANK))
	{
		DrawRectangleRounded(highlightDest, 0.1f, 6, highlight);
	}
	Texture2D tex = card->data.card._faceUp ? GetCardValue(card->data.card.value, small) : GetCardBack(small);
	DrawTexturePro(tex, GetCardSource(small), dest, V(0.5f, 0.5f), 0.0f, card->_locked ? LIGHTGRAY : WHITE);
}

void CardDraw(const Object* card)
{
	CardDrawInternal(card, BLANK, false);
}

void CardDrawHighlight(const Object* card, Color highlight)
{
	CardDrawInternal(card, highlight, false);
}

void CardDrawShadowed(const Object* card)
{
	CardDrawInternal(card, BLANK, true);
}

void CardDrawCustom(Vector2 position, int value, float rotation, Color highlight)
{
	bool small = DebugDrawCardsSmall();
	Vector2 sz = small ? CARD_SIZE_SMALL : CARD_SIZE;
	Vector2 origin = Vector2Scale(sz, 0.5f);
	Rectangle dest = R(position.x + origin.x, position.y + origin.y, sz.x, sz.y);

	Rectangle highlightDest = dest;
	highlightDest.x -= CARD_HIGHLIGHT_EXTENT;
	highlightDest.y -= CARD_HIGHLIGHT_EXTENT;
	highlightDest.width += 2* CARD_HIGHLIGHT_EXTENT;
	highlightDest.height += 2* CARD_HIGHLIGHT_EXTENT;

	if (!ColorIsEqual(highlight, BLANK))
	{
		DrawRectangleRounded(highlightDest, 0.1f, 6, highlight);
	}
	Texture2D tex = GetCardValue(value, small);
	Rectangle src = GetCardSource(small);
	DrawTexturePro(tex, src, dest, origin, rotation, WHITE);
}

Vector2 CardGetSize(void)
{
	return DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
}

Rectangle CardGetRect(const Object* card)
{
	Vector2 sz = CardGetSize();
	return R(card->_position.x, card->_position.y, sz.x, sz.y);
}

Suit CardSuit(const Object* card)
{
	return GetSuit(card->data.card.value);
}

Rank CardRank(const Object* card)
{
	return GetRank(card->data.card.value);
}

Suit GetSuit(int value)
{
	return value / 13;
}

Rank GetRank(int value)
{
	return value % 13;
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
