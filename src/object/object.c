#include <string.h>
#include "object.h"

static Color next_draw_highlight = {0};
static bool next_draw_shadowed = false;

void ObjectSetDrawHighlight(Color highlight)
{
	next_draw_highlight = highlight;
}

void ObjectSetDrawShadowed(void)
{
	next_draw_shadowed = true;
}

Color ObjectGetDrawHighlight(void)
{
	Color value = next_draw_highlight;
	next_draw_highlight = BLANK;
	return value;
}

bool ObjectGetDrawShadowed(void)
{
	bool value = next_draw_shadowed;
	next_draw_shadowed = false;
	return value;
}
