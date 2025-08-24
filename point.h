/**
 * @file point.h
 * @brief Define a estrutura de dados e a interface para o objeto Ponto.
 *
 * O Ponto � a primitiva geom�trica mais fundamental do projeto e serve como
 * base para a cria��o de outros objetos, como segmentos e pol�gonos.
 */

#ifndef POINT_H
#define POINT_H

// --- SE��O DE ESTRUTURAS E TIPOS ---

/**
 * @brief Estrutura para representar um ponto 2D.
 */
typedef struct {
    float x;    ///< Coordenada no eixo X.
    float y;    ///< Coordenada no eixo Y.
} Point;


// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Cria e inicializa uma estrutura Point com as coordenadas fornecidas.
 * @param x A coordenada X do ponto.
 * @param y A coordenada Y do ponto.
 * @return Point Uma nova estrutura Point com os valores especificados.
 */
Point createPoint(float x, float y);

/**
 * @brief Desenha um ponto na tela usando as primitivas do OpenGL.
 *
 * A cor do ponto muda dependendo de seu estado de sele��o.
 * @param p Ponteiro para a estrutura Point a ser desenhada.
 * @param is_selected Flag (1 ou 0) que indica se o ponto est� selecionado.
 */
void drawPoint(Point* p, int is_selected);

/**
 * @brief Libera a mem�ria interna de um Ponto, se houver.
 *
 * Esta fun��o � mantida para consist�ncia com a API de gerenciamento de objetos,
 * mesmo que a estrutura Point n�o exija libera��o de mem�ria interna.
 * @param p Ponteiro para a estrutura Point.
 */
void freePoint(Point* p);

#endif // POINT_H
