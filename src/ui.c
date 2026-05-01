#include "clay.h"
#include <stdbool.h>   // for bool, true, false
#include <string.h>    // for memcmp (optional, but safe)
#include "ui.h"
typedef void (*__ButtonClickFn)(void);

// Internal list of element IDs that have already been clicked.
// Increase CAPACITY if you have many buttons.
#define CLICKED_ID_CAPACITY 16
static Clay_ElementId clickedIds[CLICKED_ID_CAPACITY];
static int clickedCount = 0;

// Helper: check if an element ID is already in the "clicked" set.
static bool isAlreadyClicked(Clay_ElementId id) {
    for (int i = 0; i < clickedCount; i++) {
        if (memcmp(&clickedIds[i], &id, sizeof(Clay_ElementId)) == 0) {
            return true;
        }
    }
    return false;
}

// Helper: add an element ID to the "clicked" set.
static void markAsClicked(Clay_ElementId id) {
    if (clickedCount < CLICKED_ID_CAPACITY) {
        clickedIds[clickedCount++] = id;
    }
}

void OnButtonHover(Clay_ElementId elementId,
                   Clay_PointerData pointerData,
                   void *userData) {
    // Only react to the initial press frame.
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        // Skip if this button was already clicked before.
        if (isAlreadyClicked(elementId)) {
            return;
        }

        // Mark as clicked so it won't fire again.
        markAsClicked(elementId);

        // Cast and call the user callback.
        __ButtonClickFn onClick = (__ButtonClickFn)userData;
        if (onClick) {
            onClick();
        }
    }
}

void Button(Clay_String label, __ButtonClickFn onClick) {
    CLAY_AUTO_ID({
        .layout = {
            .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}
        },
        .backgroundColor = Clay_Hovered()
                               ? (Clay_Color){100, 160, 210, 255}
                               : (Clay_Color){70,  130, 180, 255},
    }) {
        Clay_OnHover(OnButtonHover, (void *)onClick);
        CLAY_TEXT(label, CLAY_TEXT_CONFIG({
            .fontSize  = 36,
            .textColor = {255, 255, 255, 255},
        }));
    }
}
