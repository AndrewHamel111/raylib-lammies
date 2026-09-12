#include "animation.h"
#include "constants.h"

void CardFlip(Object* card)
{
	if (card->data.card._animationState != CardStateDefault)
	{
		return;
	}

	if (card->data.card._faceUp)
	{
		card->data.card._animationState = CardStateFlippingDownIn;
		card->data.card._flipTime = CARD_FLIP_TIME;
	}
	else
	{
		card->data.card._animationState = CardStateFlippingUpIn;
		card->data.card._flipTime = CARD_FLIP_TIME;
	}
}
