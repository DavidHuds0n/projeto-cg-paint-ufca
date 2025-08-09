// segment.c
// Implementação das funções para manipulação de segmentos.

#include <GL/glut.h> // Para funções de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depuração)
#include <math.h>    // Para usar sqrt, pow, fmaxf, fminf
#include "segment.h" // Inclui o próprio cabeçalho
#include "point.h"   // Para usar a estrutura Point

// Implementação das funções para Segment

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
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado
    }
    glLineWidth(2.0f); // Largura da linha
    glBegin(GL_LINES);
    glVertex2f(s->p1.x, s->p1.y); // Usa s->p1.x e s->p1.y pois é um ponteiro
    glVertex2f(s->p2.x, s->p2.y);
    glEnd();
    glLineWidth(1.0f); // Volta à largura de linha padrão
}

// Calcula a distância de um ponto a um segmento.
float distPointSegment(Point p, Segment s) {
    float dx = s.p2.x - s.p1.x;
    float dy = s.p2.y - s.p1.y;
    float lengthSq = dx * dx + dy * dy;

    if (lengthSq == 0.0f) { // Segmento é um ponto
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

// Libera a memória alocada para um segmento.
// Para a estrutura Segment atual, não há memória dinâmica interna para liberar.
void freeSegment(Segment* s) { // NOVO
    printf("[Segment] Função freeSegment chamada para segmento de (%.1f, %.1f) para (%.1f, %.1f).\n", s->p1.x, s->p1.y, s->p2.x, s->p2.y);
    // Não há 'free' aqui porque a Segment struct não contém ponteiros alocados dinamicamente
}
