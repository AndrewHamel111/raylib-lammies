#include "animation.h"
#include "constants.h"

void CardFlip(Card* card)
{
	if (card->_animationState != CardStateDefault)
	{
		return;
	}

	if (card->_faceUp)
	{
		card->_animationState = CardStateFlippingDownIn;
		card->_flipTime = CARD_FLIP_TIME;
	}
	else
	{
		card->_animationState = CardStateFlippingUpIn;
		card->_flipTime = CARD_FLIP_TIME;
	}
}
