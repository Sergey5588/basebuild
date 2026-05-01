#ifndef UI_H
#define UI_H
#include <clay.h>
typedef void (*__ButtonClickFn)(void);
#define TEXT_CONFIG CLAY_TEXT_CONFIG({ \
            .fontSize  = 36,\
            .textColor = {255, 255, 255, 255},\
        })\

void Button(Clay_String label, __ButtonClickFn onClick);
#endif
