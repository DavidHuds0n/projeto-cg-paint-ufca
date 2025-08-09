// segment.c
// Implementa��o das fun��es para manipula��o de segmentos.

#include <GL/glut.h> // Para fun��es de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depura��o)
#include <math.h>    // Para usar sqrt, pow, fmaxf, fminf
#include "segment.h" // Inclui o pr�prio cabe�alho
#include "point.h"   // Para usar a estrutura Point

// Implementa��o das fun��es para Segment

// Cria um novo segmento com dois pontos.
Segment createSegment(Point p1, Point p2) {
    Segment s = {p1, p2};
    printf("[Segment] Novo segmento criado de (%.1f, %.1f) para (%.1f, %.1f).\n", p1.x, p1.y, p2.x, p2.y);
    return s;
}

// Desenha o segmento na tela. Agora recebe um ponteiro.
void drawSegment(Segment* s, int is_selected) { // ATUALIZADO: Recebe Segment*
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado
    }
    glLineWidth(2.0f); // Largura da linha
    glBegin(GL_LINES);
    glVertex2f(s->p1.x, s->p1.y); // Usa s->p1.x e s->p1.y pois � um ponteiro
    glVertex2f(s->p2.x, s->p2.y);
    glEnd();
    glLineWidth(1.0f); // Volta � largura de linha padr�o
}

// Calcula a dist�ncia de um ponto a um segmento.
float distPointSegment(Point p, Segment s) {
    float dx = s.p2.x - s.p1.x;
    float dy = s.p2.y - s.p1.y;
    float lengthSq = dx * dx + dy * dy;

    if (lengthSq == 0.0f) { // Segmento � um ponto
        float dist_x = p.x - s.p1.x;
        float dist_y = p.y - s.p1.y;
        return sqrtf(dist_x * dist_x + dist_y * dist_y);
    }

    float t = ((p.x - s.p1.x) * dx + (p.y - s.p1.y) * dy) / lengthSq;
    t = fmaxf(0.0f, fminf(1.0f, t)); // Clampa t entre 0 e 1

    Point closest = {s.p1.x + t * dx, s.p1.y + t * dy};

    float dist_x = p.x - closest.x;
    float dist_y = p.y - closest.y;
    return sqrtf(dist_x * dist_x + dist_y * dist_y);
}

// Libera a mem�ria alocada para um segmento.
// Para a estrutura Segment atual, n�o h� mem�ria din�mica interna para liberar.
void freeSegment(Segment* s) { // NOVO
    printf("[Segment] Fun��o freeSegment chamada para segmento de (%.1f, %.1f) para (%.1f, %.1f).\n", s->p1.x, s->p1.y, s->p2.x, s->p2.y);
    // N�o h� 'free' aqui porque a Segment struct n�o cont�m ponteiros alocados dinamicamente
}
