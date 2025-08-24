/**
 * @file segment.c
 * @brief Implementação das funções para criar, desenhar e liberar objetos do tipo Segmento de Reta.
 *
 * Este módulo gerencia todas as operações de baixo nível para a primitiva
 * geométrica "Segmento".
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "segment.h"
#include "point.h"

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

Segment createSegment(Point p1, Point p2) {
    Segment s = {p1, p2};
    return s;
}

void drawSegment(Segment* s, int is_selected) {
    // Define a cor do segmento com base no estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // Aumenta a espessura da linha para melhor visualização.
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        glVertex2f(s->p1.x, s->p1.y);
        glVertex2f(s->p2.x, s->p2.y);
    glEnd();

    // Reseta a espessura da linha para o padrão.
    glLineWidth(1.0f);
}

float distPointSegment(Point p, Segment s) {
    float dx = s.p2.x - s.p1.x;
    float dy = s.p2.y - s.p1.y;

    // Se o segmento tem comprimento zero, calcula a distância até o ponto.
    if (dx == 0 && dy == 0) {
        float dist_x = p.x - s.p1.x;
        float dist_y = p.y - s.p1.y;
        return sqrtf(dist_x * dist_x + dist_y * dist_y);
    }

    // Calcula a projeção do ponto sobre a reta infinita.
    float lengthSq = dx * dx + dy * dy;
    float t = ((p.x - s.p1.x) * dx + (p.y - s.p1.y) * dy) / lengthSq;

    // Restringe o parâmetro 't' ao intervalo [0, 1] para o segmento de reta.
    t = fmaxf(0.0f, fminf(1.0f, t));

    // Encontra o ponto mais próximo no segmento.
    Point closest = {s.p1.x + t * dx, s.p1.y + t * dy};

    // Retorna a distância euclidiana.
    float dist_x = p.x - closest.x;
    float dist_y = p.y - closest.y;
    return sqrtf(dist_x * dist_x + dist_y * dist_y);
}

void freeSegment(Segment* s) {
    // A estrutura 'Segment' não possui alocação de memória dinâmica interna.
    // Esta função existe para manter uma API consistente com o módulo 'objects'.
}
