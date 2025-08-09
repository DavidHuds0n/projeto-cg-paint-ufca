// point.h
// Defini��o da estrutura Point e prot�tipos de fun��es para manipula��o de pontos.

#ifndef POINT_H
#define POINT_H

// Defini��o da estrutura Point
typedef struct {
    float x;
    float y;
} Point;

// Prot�tipos das fun��es para Point
// Cria um novo ponto com as coordenadas especificadas.
Point createPoint(float x, float y);

// Desenha o ponto na tela. Agora recebe um ponteiro.
void drawPoint(Point* p, int is_selected); // ATUALIZADO: Recebe Point*

// Libera a mem�ria alocada para um ponto. Adicionado para consist�ncia.
void freePoint(Point* p); // NOVO PROT�TIPO

#endif // POINT_H
