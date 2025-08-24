/**
 * @file animation.h
 * @brief Define a interface e as estruturas para o sistema de anima��o baseado em f�sica.
 *
 * Este m�dulo gerencia o estado da anima��o de cada objeto, permitindo
 * movimento, colis�es com as bordas da tela e efeitos como gravidade.
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include "config.h"

// --- SE��O DE ESTRUTURAS DE DADOS ---

/**
 * @brief Armazena o estado de anima��o de um �nico objeto.
 *
 * Cada objeto na cena pode ter um 'AnimSlot' correspondente que dita
 * seu comportamento f�sico a cada passo de tempo.
 */
typedef struct {
    int   active;     ///< Flag que indica se a anima��o est� ativa (1) ou inativa (0).
    float vx, vy;     ///< Componentes da velocidade do objeto (pixels por segundo).
    float e;          ///< Coeficiente de restitui��o (elasticidade) para colis�es (0.0 a 1.0).
    int   gravity;    ///< Flag que indica se a gravidade deve ser aplicada (1) ou n�o (0).
    float vmax;       ///< Velocidade m�xima permitida para o objeto (pixels por segundo).
} AnimSlot;

// --- SE��O DE VARI�VEIS GLOBAIS (DECLARA��ES) ---

extern AnimSlot g_anim[MAX_OBJECTS]; ///< Array que armazena o estado da anima��o de cada objeto.

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

// Fun��es de inicializa��o e controle de estado
void anim_init_all(void);
void anim_enable(int idx, float vx, float vy, float e, int gravity);
void anim_disable(int idx);
void anim_toggle_selected(void);

// Fun��es de intera��o
void anim_kick_towards_mouse(float speed);

// Fun��es do loop principal e de manuten��o
void anim_step(float dt);
void anim_on_remove_compact(int removed_idx);

#endif // ANIMATION_H
