#include "scene_manager.h"
#include "raylib.h"      /* for DrawRectangle, Fade, GetScreenWidth/Height */
#include <stdlib.h>      /* for NULL */

/* --------------------------------------------------------------------------
   Internal helper – finishes the switch after fade‑out is complete.
-------------------------------------------------------------------------- */
static void finish_transition(scene_manager_t *mgr) {
    /* Exit and destroy the old scene */
    if (mgr->current_scene) {
        mgr->current_scene->exit(mgr->current_scene);
        mgr->current_scene->destroy(mgr->current_scene);
    }

    /* Activate the new scene */
    mgr->current_scene = mgr->next_scene;
    mgr->next_scene = NULL;
    if (mgr->current_scene) {
        mgr->current_scene->enter(mgr->current_scene);
    }

    /* Start fading in */
    mgr->trans_state = TRANSITION_FADING_IN;
    mgr->transition_timer = 0.0f;
}

/* --------------------------------------------------------------------------
   Public functions
-------------------------------------------------------------------------- */

void scene_manager_init(scene_manager_t *mgr) {
    mgr->current_scene      = NULL;
    mgr->next_scene         = NULL;
    mgr->transition_timer   = 0.0f;
    mgr->transition_duration= 0.0f;
    mgr->trans_state        = TRANSITION_NONE;
    mgr->paused             = 0;
}

void scene_manager_change_scene(scene_manager_t *mgr, scene_t *new_scene, float duration) {
    if (duration <= 0.0f) {
        /* Instant switch */
        if (mgr->current_scene) {
            mgr->current_scene->exit(mgr->current_scene);
            mgr->current_scene->destroy(mgr->current_scene);
        }
        mgr->current_scene = new_scene;
        if (mgr->current_scene) {
            mgr->current_scene->enter(mgr->current_scene);
        }
        mgr->trans_state = TRANSITION_NONE;
        mgr->next_scene  = NULL;
    } else {
        /* Begin fade‑out */
        mgr->next_scene = new_scene;
        mgr->transition_timer     = 0.0f;
        mgr->transition_duration  = duration / 2.0f;
        mgr->trans_state          = TRANSITION_FADING_OUT;
    }
}

void scene_manager_update(scene_manager_t *mgr, float dt) {
	mgr->last_dt = dt;
    if (!mgr->current_scene) return;
    switch (mgr->trans_state) {
        case TRANSITION_NONE:
            if (!mgr->paused) {
                mgr->current_scene->update(mgr->current_scene, dt);
            }
            break;

        case TRANSITION_FADING_OUT:
            mgr->transition_timer += dt;
            if (mgr->transition_timer >= mgr->transition_duration) {
                finish_transition(mgr);
            }
            break;

        case TRANSITION_WAITING:
            /* Never actually used – finish_transition() moves straight to FADING_IN */
            break;

        case TRANSITION_FADING_IN:
            mgr->transition_timer += dt;
            if (!mgr->paused) {
                mgr->current_scene->update(mgr->current_scene, dt);
            }
            if (mgr->transition_timer >= mgr->transition_duration) {
                mgr->trans_state = TRANSITION_NONE;
            }
            break;
    }
}

void scene_manager_draw(scene_manager_t *mgr) {
    if (!mgr->current_scene) return;

    /* 1. Draw the active scene (even when paused, we want to see it) */
    mgr->current_scene->draw(mgr->current_scene, mgr->last_dt);

    /* 2. Overlay the transition fade rectangle */
    float alpha = 0.0f;
    if (mgr->trans_state == TRANSITION_FADING_OUT) {
        alpha = mgr->transition_timer / mgr->transition_duration;
    } else if (mgr->trans_state == TRANSITION_FADING_IN) {
        alpha = 1.0f - (mgr->transition_timer / mgr->transition_duration);
    } else if (mgr->trans_state == TRANSITION_WAITING) {
        alpha = 1.0f;
    }

    if (alpha > 0.0f) {
        Color overlay = Fade(BLACK, alpha);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), overlay);
    }
}

void scene_manager_set_paused(scene_manager_t *mgr, int paused) {
    mgr->paused = paused;
}

int scene_manager_is_paused(const scene_manager_t *mgr) {
    return mgr->paused;
}

void scene_manager_quit(scene_manager_t *mgr) {
    if (mgr->current_scene) {
        mgr->current_scene->exit(mgr->current_scene);
        mgr->current_scene->destroy(mgr->current_scene);
        mgr->current_scene = NULL;
    }
    mgr->next_scene = NULL;
    mgr->trans_state = TRANSITION_NONE;
}
