// animation.c
// Implementa��o da l�gica de anima��o.

#include "animation.h"
#include "objects.h"
#include "transformations.h"
#include <GL/glut.h>
#include <stdio.h>

#define TIMER_MS 16 // Tempo para o timer em milissegundos (aprox. 60 FPS)

void startAnimation() {
    // Registra a fun��o updateAnimation para ser chamada pela primeira vez
    // ap�s TIMER_MS milissegundos.
    glutTimerFunc(TIMER_MS, updateAnimation, 0);
    printf("Anima��o iniciada.\n");
}

void updateAnimation(int value) {
    // TODO: Implementar a l�gica da anima��o aqui.
    // Exemplo: Mover o primeiro objeto da lista um pouco para a direita.
    // if (g_numObjects > 0) {
    //     translateObject(0, 0.5f, 0.0f);
    // }

    // For�a o redesenho da tela para exibir a mudan�a.
    glutPostRedisplay();

    // Reagenda a fun��o para ser chamada novamente, criando um loop infinito.
    glutTimerFunc(TIMER_MS, updateAnimation, 0);
}
