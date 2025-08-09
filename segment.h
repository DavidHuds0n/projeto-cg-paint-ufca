// segment.h
// Defini��o da estrutura Segment e prot�tipos de fun��es para manipula��o de segmentos.

#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h" // Segmentos s�o compostos por Pontos

// Defini��o da estrutura Segment
typedef struct {
    Point p1;
    Point p2;
} Segment;

// Prot�tipos das fun��es para Segment
// Cria um novo segmento com dois pontos.
Segment createSegment(Point p1, Point p2);

// Desenha o segmento na tela. Agora recebe um ponteiro.
void drawSegment(Segment* s, int is_selected); // ATUALIZADO: Recebe Segment*

// Calcula a dist�ncia de um ponto a um segmento.
float distPointSegment(Point p, Segment s);

// Libera a mem�ria alocada para um segmento. Adicionado para consist�ncia.
void freeSegment(Segment* s); // NOVO PROT�TIPO

#endif // SEGMENT_H
