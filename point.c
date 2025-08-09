// point.c
// Implementação das funções para manipulação de pontos.

#include <GL/glut.h> // Para funções de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depuração)
#include "point.h"   // Inclui o próprio cabeçalho
#include "config.h"  // Para usar CLICK_TOLERANCE

// Implementação das funções para Point

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
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado
    }
    glPointSize(CLICK_TOLERANCE / 2.0f); // Tamanho do ponto (definido em config.h, ajustado para ser um pouco menor)
    glBegin(GL_POINTS);
    glVertex2f(p->x, p->y); // Usa p->x e p->y pois é um ponteiro
    glEnd();
    glPointSize(1.0f); // Volta ao tamanho padrão de ponto
}

// Libera a memória alocada para um ponto.
// Para a estrutura Point atual, não há memória dinâmica interna para liberar,
// mas a função é mantida para consistência com o gerenciamento de objetos.
void freePoint(Point* p) { // NOVO
    printf("[Point] Função freePoint chamada para ponto em (%.1f, %.1f).\n", p->x, p->y);
    // Não há 'free' aqui porque a Point struct não contém ponteiros alocados dinamicamente
}
