// segment.h
// Definição da estrutura Segment e protótipos de funções para manipulação de segmentos.

#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h" // Segmentos são compostos por Pontos

// Definição da estrutura Segment
typedef struct {
    Point p1;
    Point p2;
} Segment;

// Protótipos das funções para Segment
// Cria um novo segmento com dois pontos.
Segment createSegment(Point p1, Point p2);

// Desenha o segmento na tela. Agora recebe um ponteiro.
void drawSegment(Segment* s, int is_selected); // ATUALIZADO: Recebe Segment*

// Calcula a distância de um ponto a um segmento.
float distPointSegment(Point p, Segment s);

// Libera a memória alocada para um segmento. Adicionado para consistência.
void freeSegment(Segment* s); // NOVO PROTÓTIPO

#endif // SEGMENT_H
