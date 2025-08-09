// point.c
// Implementa��o das fun��es para manipula��o de pontos.

#include <GL/glut.h> // Para fun��es de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depura��o)
#include "point.h"   // Inclui o pr�prio cabe�alho
#include "config.h"  // Para usar CLICK_TOLERANCE

// Implementa��o das fun��es para Point

// Cria um novo ponto com as coordenadas especificadas.
Point createPoint(float x, float y) {
    Point p = {x, y};
    printf("[Point] Novo ponto criado em (%.1f, %.1f).\n", p.x, p.y);
    return p;
}

// Desenha o ponto na tela. Agora recebe um ponteiro.
void drawPoint(Point* p, int is_selected) { // ATUALIZADO: Recebe Point*
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado
    }
    glPointSize(CLICK_TOLERANCE / 2.0f); // Tamanho do ponto (definido em config.h, ajustado para ser um pouco menor)
    glBegin(GL_POINTS);
    glVertex2f(p->x, p->y); // Usa p->x e p->y pois � um ponteiro
    glEnd();
    glPointSize(1.0f); // Volta ao tamanho padr�o de ponto
}

// Libera a mem�ria alocada para um ponto.
// Para a estrutura Point atual, n�o h� mem�ria din�mica interna para liberar,
// mas a fun��o � mantida para consist�ncia com o gerenciamento de objetos.
void freePoint(Point* p) { // NOVO
    printf("[Point] Fun��o freePoint chamada para ponto em (%.1f, %.1f).\n", p->x, p->y);
    // N�o h� 'free' aqui porque a Point struct n�o cont�m ponteiros alocados dinamicamente
}
