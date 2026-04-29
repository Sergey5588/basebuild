#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


/* --------------------------------------------------------------------------
   Scene interface – each concrete scene provides these callbacks.
   The `ctx` pointer holds the scene’s private data.
-------------------------------------------------------------------------- */
typedef struct scene_t scene_t;

struct scene_t {
    void (*init)(scene_t *self);
    void (*enter)(scene_t *self);
    void (*exit)(scene_t *self);
    void (*update)(scene_t *self, float dt);
    void (*draw)(scene_t *self, float dt);
    void (*destroy)(scene_t *self);
    void *ctx;
};

/* ---- Transition states ------------------------------------------------- */
typedef enum {
    TRANSITION_NONE,
    TRANSITION_FADING_OUT,
    TRANSITION_WAITING,      /* momentary black screen between scenes */
    TRANSITION_FADING_IN
} transition_state_t;

/* ---- Scene manager – holds one active scene at a time ------------------ */
typedef struct {
    scene_t           *current_scene;
    scene_t           *next_scene;
    float              transition_timer;
    float              transition_duration;   /* total duration; each half is duration/2 */
    transition_state_t trans_state;
    int                paused;                /* 1 = paused, 0 = running */
	float last_dt;
} scene_manager_t;

/* ---- Public API -------------------------------------------------------- */
void scene_manager_init           (scene_manager_t *mgr);
void scene_manager_change_scene   (scene_manager_t *mgr, scene_t *new_scene, float duration);
void scene_manager_update         (scene_manager_t *mgr, float dt);
void scene_manager_draw           (scene_manager_t *mgr);
void scene_manager_set_paused     (scene_manager_t *mgr, int paused);
int  scene_manager_is_paused      (const scene_manager_t *mgr);
void scene_manager_quit           (scene_manager_t *mgr);

#endif /* SCENE_MANAGER_H */
