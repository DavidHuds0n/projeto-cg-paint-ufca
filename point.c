/**
 * @file point.c
 * @brief Implementação das funções para criar, desenhar e liberar objetos do tipo Ponto.
 *
 * Este módulo gerencia todas as operações de baixo nível para a primitiva
 * geométrica "Ponto".
 */

#include <GL/glut.h>
#include <stdio.h>
#include "point.h"
#include "config.h"

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

Point createPoint(float x, float y) {
    Point p = {x, y};
    return p;
}

void drawPoint(Point* p, int is_selected) {
    // Define a cor do ponto com base no estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // Aumenta o tamanho do ponto para torná-lo mais visível e fácil de selecionar.
    glPointSize(CLICK_TOLERANCE / 2.0f);

    glBegin(GL_POINTS);
        glVertex2f(p->x, p->y);
    glEnd();

    // Reseta o tamanho do ponto para o padrão.
    glPointSize(1.0f);
}

void freePoint(Point* p) {
    // A estrutura 'Point' não possui alocação de memória dinâmica interna.
    // Esta função existe para manter uma API consistente com o módulo 'objects',
    // que chama uma função de liberação para cada tipo de objeto.
}
