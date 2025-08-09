// point.h
// Definição da estrutura Point e protótipos de funções para manipulação de pontos.

#ifndef POINT_H
#define POINT_H

// Definição da estrutura Point
typedef struct {
    float x;
    float y;
} Point;

// Protótipos das funções para Point
// Cria um novo ponto com as coordenadas especificadas.
Point createPoint(float x, float y);

// Desenha o ponto na tela. Agora recebe um ponteiro.
void drawPoint(Point* p, int is_selected); // ATUALIZADO: Recebe Point*

// Libera a memória alocada para um ponto. Adicionado para consistência.
void freePoint(Point* p); // NOVO PROTÓTIPO

#endif // POINT_H
