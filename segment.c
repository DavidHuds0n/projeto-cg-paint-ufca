/**
 * @file segment.c
 * @brief Implementa��o das fun��es para criar, desenhar e liberar objetos do tipo Segmento de Reta.
 *
 * Este m�dulo gerencia todas as opera��es de baixo n�vel para a primitiva
 * geom�trica "Segmento".
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "segment.h"
#include "point.h"

// --- SE��O DE FUN��ES P�BLICAS ---

Segment createSegment(Point p1, Point p2) {
    Segment s = {p1, p2};
    return s;
}

void drawSegment(Segment* s, int is_selected) {
    // Define a cor do segmento com base no estado de sele��o.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado.
    }

    // Aumenta a espessura da linha para melhor visualiza��o.
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        glVertex2f(s->p1.x, s->p1.y);
        glVertex2f(s->p2.x, s->p2.y);
    glEnd();

    // Reseta a espessura da linha para o padr�o.
    glLineWidth(1.0f);
}

float distPointSegment(Point p, Segment s) {
    float dx = s.p2.x - s.p1.x;
    float dy = s.p2.y - s.p1.y;

    // Se o segmento tem comprimento zero, calcula a dist�ncia at� o ponto.
    if (dx == 0 && dy == 0) {
        float dist_x = p.x - s.p1.x;
        float dist_y = p.y - s.p1.y;
        return sqrtf(dist_x * dist_x + dist_y * dist_y);
    }

    // Calcula a proje��o do ponto sobre a reta infinita.
    float lengthSq = dx * dx + dy * dy;
    float t = ((p.x - s.p1.x) * dx + (p.y - s.p1.y) * dy) / lengthSq;

    // Restringe o par�metro 't' ao intervalo [0, 1] para o segmento de reta.
    t = fmaxf(0.0f, fminf(1.0f, t));

    // Encontra o ponto mais pr�ximo no segmento.
    Point closest = {s.p1.x + t * dx, s.p1.y + t * dy};

    // Retorna a dist�ncia euclidiana.
    float dist_x = p.x - closest.x;
    float dist_y = p.y - closest.y;
    return sqrtf(dist_x * dist_x + dist_y * dist_y);
}

void freeSegment(Segment* s) {
    // A estrutura 'Segment' n�o possui aloca��o de mem�ria din�mica interna.
    // Esta fun��o existe para manter uma API consistente com o m�dulo 'objects'.
}
