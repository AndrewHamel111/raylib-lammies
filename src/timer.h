#pragma once

typedef void (*void_callback)(void);

int SetTimer(float time, void_callback on_complete);
int SetTimerBlocking(float time, void_callback on_complete);
void TickTimer(float time);

/// Actions which meaningfully advance game or program state may want to be nullified if this function returns true
int HasBlockingTimer(void);
