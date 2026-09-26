#include "register.h"
#include "object.h"
#include "vtable.h"

#include "card.h"
#include "card/animation.h"
#include "reserve.h"
#include "discard.h"

void RegisterAllObjects(void)
{
	// Card
	ObjectRegisterFunc(ObjectCard, PurposeTick, (funcPtr)&CardTick);
	ObjectRegisterFunc(ObjectCard, PurposeDraw, (funcPtr)&CardDraw);
	ObjectRegisterFunc(ObjectCard, PurposeRect, (funcPtr)&CardRect);
	ObjectRegisterFunc(ObjectCard, PurposeCombine, (funcPtr)&CardCombine);
	ObjectRegisterFunc(ObjectCard, PurposeFlippable, (funcPtr)&CardFlippable);
	ObjectRegisterFunc(ObjectCard, PurposeFlip, (funcPtr)&CardFlip);
	ObjectRegisterFunc(ObjectCard, PurposeHandlePickup, (funcPtr)&CardHandlePickup);
	ObjectRegisterFunc(ObjectCard, PurposeHandleDrop, (funcPtr)&CardHandleDrop);
	ObjectRegisterFunc(ObjectCard, PurposeHandleInput, (funcPtr)&CardHandleInput);

	// Reserve
	ObjectRegisterFunc(ObjectReserve, PurposeTick, (funcPtr)&ReserveTick);
	ObjectRegisterFunc(ObjectReserve, PurposeDraw, (funcPtr)&ReserveDraw);
	ObjectRegisterFunc(ObjectReserve, PurposeRect, (funcPtr)&ReserveRect);
	ObjectRegisterFunc(ObjectReserve, PurposeCombine, (funcPtr)&ReserveCombine);
	ObjectRegisterFunc(ObjectReserve, PurposeFlippable, (funcPtr)&ReserveFlippable);
	ObjectRegisterFunc(ObjectReserve, PurposeFlip, (funcPtr)&ReserveFlip);
	ObjectRegisterFunc(ObjectReserve, PurposeHandlePickup, (funcPtr)&ReserveHandlePickup);
	ObjectRegisterFunc(ObjectReserve, PurposeHandleDrop, (funcPtr)&ReserveHandleDrop);
	ObjectRegisterFunc(ObjectReserve, PurposeHandleInput, (funcPtr)&ReserveHandleInput);

	// Discard
	ObjectRegisterFunc(ObjectDiscard, PurposeTick, (funcPtr)&DiscardTick);
	ObjectRegisterFunc(ObjectDiscard, PurposeDraw, (funcPtr)&DiscardDraw);
	ObjectRegisterFunc(ObjectDiscard, PurposeRect, (funcPtr)&DiscardRect);
	ObjectRegisterFunc(ObjectDiscard, PurposeCombine, (funcPtr)&DiscardCombine);
	ObjectRegisterFunc(ObjectDiscard, PurposeFlippable, (funcPtr)&DiscardFlippable);
	ObjectRegisterFunc(ObjectDiscard, PurposeFlip, (funcPtr)&DiscardFlip);
	ObjectRegisterFunc(ObjectDiscard, PurposeHandlePickup, (funcPtr)&DiscardHandlePickup);
	ObjectRegisterFunc(ObjectDiscard, PurposeHandleDrop, (funcPtr)&DiscardHandleDrop);
	ObjectRegisterFunc(ObjectDiscard, PurposeHandleInput, (funcPtr)&DiscardHandleInput);
}
