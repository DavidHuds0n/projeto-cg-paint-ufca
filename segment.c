// segment.c
// Implementa��o das fun��es para criar, desenhar e liberar objetos do tipo Segmento de Reta.

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "segment.h"
#include "point.h"


Segment createSegment(Point p1, Point p2) {
    Segment s = {p1, p2};
    // O printf abaixo � para fins de depura��o.
    // printf("[Segment] Novo segmento criado.\n");
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

    // Desenha a linha na tela usando as primitivas do OpenGL.
    glBegin(GL_LINES);
        glVertex2f(s->p1.x, s->p1.y);
        glVertex2f(s->p2.x, s->p2.y);
    glEnd();

    // Reseta a espessura da linha para o padr�o para n�o afetar outros desenhos.
    glLineWidth(1.0f);
}

float distPointSegment(Point p, Segment s) {
    // Vetor que representa o segmento de reta (de p1 para p2).
    float dx = s.p2.x - s.p1.x;
    float dy = s.p2.y - s.p1.y;

    // Se o segmento for, na verdade, um ponto (comprimento zero).
    if (dx == 0 && dy == 0) {
        float dist_x = p.x - s.p1.x;
        float dist_y = p.y - s.p1.y;
        return sqrtf(dist_x * dist_x + dist_y * dist_y);
    }

    // Calcula o par�metro 't', que � a proje��o do vetor (p - p1) sobre o vetor do segmento.
    // 't' representa o qu�o "longe" ao longo da linha infinita o ponto mais pr�ximo est�.
    float lengthSq = dx * dx + dy * dy;
    float t = ((p.x - s.p1.x) * dx + (p.y - s.p1.y) * dy) / lengthSq;

    // "Clampa" (restringe) o valor de 't' para o intervalo [0, 1].
    // Se t < 0, o ponto mais pr�ximo � p1. Se t > 1, � p2. Se 0 <= t <= 1, est� no meio.
    // Esta � a l�gica crucial que faz o c�lculo funcionar para um SEGMENTO, n�o para uma RETA.
    t = fmaxf(0.0f, fminf(1.0f, t));

    // Calcula as coordenadas do ponto mais pr�ximo no segmento de reta.
    Point closest = {s.p1.x + t * dx, s.p1.y + t * dy};

    // Retorna a dist�ncia euclidiana entre o ponto original e o ponto mais pr�ximo encontrado.
    float dist_x = p.x - closest.x;
    float dist_y = p.y - closest.y;
    return sqrtf(dist_x * dist_x + dist_y * dist_y);
}

void freeSegment(Segment* s) {
    // A estrutura 'Segment' n�o possui ponteiros internos, ent�o n�o h� o que liberar.
    // A fun��o existe para manter a consist�ncia com a API do 'objects.c'.
    // printf("[Segment] Fun��o freeSegment chamada.\n");
}
