#ifndef UI_H
#define UI_H
#include <clay.h>
typedef void (*__ButtonClickFn)(void);


void Button(Clay_String label, __ButtonClickFn onClick);
#endif
