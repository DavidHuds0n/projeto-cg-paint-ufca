// animation.c
// Implementação da lógica de animação.

#include "animation.h"
#include "objects.h"
#include "transformations.h"
#include <GL/glut.h>
#include <stdio.h>

#define TIMER_MS 16 // Tempo para o timer em milissegundos (aprox. 60 FPS)

void startAnimation() {
    // Registra a função updateAnimation para ser chamada pela primeira vez
    // após TIMER_MS milissegundos.
    glutTimerFunc(TIMER_MS, updateAnimation, 0);
    printf("Animação iniciada.\n");
}

void updateAnimation(int value) {
    // TODO: Implementar a lógica da animação aqui.
    // Exemplo: Mover o primeiro objeto da lista um pouco para a direita.
    // if (g_numObjects > 0) {
    //     translateObject(0, 0.5f, 0.0f);
    // }

    // Força o redesenho da tela para exibir a mudança.
    glutPostRedisplay();

    // Reagenda a função para ser chamada novamente, criando um loop infinito.
    glutTimerFunc(TIMER_MS, updateAnimation, 0);
}
