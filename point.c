/**
 * @file point.c
 * @brief Implementa��o das fun��es para criar, desenhar e liberar objetos do tipo Ponto.
 *
 * Este m�dulo gerencia todas as opera��es de baixo n�vel para a primitiva
 * geom�trica "Ponto".
 */

#include <GL/glut.h>
#include <stdio.h>
#include "point.h"
#include "config.h"

// --- SE��O DE FUN��ES P�BLICAS ---

Point createPoint(float x, float y) {
    Point p = {x, y};
    return p;
}

void drawPoint(Point* p, int is_selected) {
    // Define a cor do ponto com base no estado de sele��o.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado.
    }

    // Aumenta o tamanho do ponto para torn�-lo mais vis�vel e f�cil de selecionar.
    glPointSize(CLICK_TOLERANCE / 2.0f);

    glBegin(GL_POINTS);
        glVertex2f(p->x, p->y);
    glEnd();

    // Reseta o tamanho do ponto para o padr�o.
    glPointSize(1.0f);
}

void freePoint(Point* p) {
    // A estrutura 'Point' n�o possui aloca��o de mem�ria din�mica interna.
    // Esta fun��o existe para manter uma API consistente com o m�dulo 'objects',
    // que chama uma fun��o de libera��o para cada tipo de objeto.
}
