/**
 * @file animation.h
 * @brief Define a interface e as estruturas para o sistema de animação baseado em física.
 *
 * Este módulo gerencia o estado da animação de cada objeto, permitindo
 * movimento, colisões com as bordas da tela e efeitos como gravidade.
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include "config.h"

// --- SEÇÃO DE ESTRUTURAS DE DADOS ---

/**
 * @brief Armazena o estado de animação de um único objeto.
 *
 * Cada objeto na cena pode ter um 'AnimSlot' correspondente que dita
 * seu comportamento físico a cada passo de tempo.
 */
typedef struct {
    int   active;     ///< Flag que indica se a animação está ativa (1) ou inativa (0).
    float vx, vy;     ///< Componentes da velocidade do objeto (pixels por segundo).
    float e;          ///< Coeficiente de restituição (elasticidade) para colisões (0.0 a 1.0).
    int   gravity;    ///< Flag que indica se a gravidade deve ser aplicada (1) ou não (0).
    float vmax;       ///< Velocidade máxima permitida para o objeto (pixels por segundo).
} AnimSlot;

// --- SEÇÃO DE VARIÁVEIS GLOBAIS (DECLARAÇÕES) ---

extern AnimSlot g_anim[MAX_OBJECTS]; ///< Array que armazena o estado da animação de cada objeto.

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

// Funções de inicialização e controle de estado
void anim_init_all(void);
void anim_enable(int idx, float vx, float vy, float e, int gravity);
void anim_disable(int idx);
void anim_toggle_selected(void);

// Funções de interação
void anim_kick_towards_mouse(float speed);

// Funções do loop principal e de manutenção
void anim_step(float dt);
void anim_on_remove_compact(int removed_idx);

#endif // ANIMATION_H
