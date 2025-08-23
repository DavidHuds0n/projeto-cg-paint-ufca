// animation.h
// Define a interface para as funções de animação do projeto.

#ifndef ANIMATION_H
#define ANIMATION_H

#include "config.h"

typedef struct {
    int   active;
    float vx, vy;
    float e;
    int   gravity;      // 1=aplica gravidade
    float vmax;         // clamp de velocidade (px/s)
} AnimSlot;

extern AnimSlot g_anim[MAX_OBJECTS];

void anim_init_all(void);
void anim_enable(int idx, float vx, float vy, float e, int gravity);
void anim_disable(int idx);
void anim_toggle_selected(void); // liga/desliga no objeto selecionado
void startAnimation();

void anim_kick_towards_mouse(float speed);
void anim_step(float dt);
void anim_on_remove_compact(int removed_idx); // manter alinhado com g_objects


#endif // ANIMATION_H
