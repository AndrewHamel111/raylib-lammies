#include "utility.h"

Rectangle RectangleInflate(Rectangle rec, float extents)
{
	return RectangleInflateV(rec, V(extents, extents));
}

Rectangle RectangleInflateV(Rectangle rec, Vector2 extents)
{
	return R(rec.x - extents.x, rec.y - extents.y, rec.width + extents.x, rec.height + extents.y);
}
