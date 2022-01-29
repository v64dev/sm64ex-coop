#pragma once
#include "djui.h"

#pragma pack(1)
struct DjuiProgressBar {
    struct DjuiBase base;
    struct DjuiRect* rect;
    struct DjuiRect* rectValue;
    float* value;
    float smoothValue;
    float min;
    float max;
};

struct DjuiProgressBar* djui_progress_bar_create(struct DjuiBase* parent, float* value, float min, float max);
