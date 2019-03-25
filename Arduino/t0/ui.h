#pragma once
#include <Arduino.h> 

bool robotEnable = false;
NexButton bStart = NexButton(0, 1, "bStart");
NexButton bStop = NexButton(1, 1, "bStop");
NexTouch *nex_listen_list[] = { &bStart, &bStop, NULL};

void bStartPopCallback(void *ptr) { robotEnable = true; }
void bStopPopCallback(void *ptr) { robotEnable = false; }

void UIInitialize() {
    nexInit();
    bStart.attachPop(bStartPopCallback);
    bStop.attachPop(bStopPopCallback);
}
