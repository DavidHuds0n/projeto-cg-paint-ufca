/**
 * @file point.h
 * @brief Define a estrutura de dados e a interface para o objeto Ponto.
 *
 * O Ponto é a primitiva geométrica mais fundamental do projeto e serve como
 * base para a criação de outros objetos, como segmentos e polígonos.
 */

#ifndef POINT_H
#define POINT_H

// --- SEÇÃO DE ESTRUTURAS E TIPOS ---

/**
 * @brief Estrutura para representar um ponto 2D.
 */
typedef struct {
    float x;    ///< Coordenada no eixo X.
    float y;    ///< Coordenada no eixo Y.
} Point;


// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

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
 * A cor do ponto muda dependendo de seu estado de seleção.
 * @param p Ponteiro para a estrutura Point a ser desenhada.
 * @param is_selected Flag (1 ou 0) que indica se o ponto está selecionado.
 */
void drawPoint(Point* p, int is_selected);

/**
 * @brief Libera a memória interna de um Ponto, se houver.
 *
 * Esta função é mantida para consistência com a API de gerenciamento de objetos,
 * mesmo que a estrutura Point não exija liberação de memória interna.
 * @param p Ponteiro para a estrutura Point.
 */
void freePoint(Point* p);

#endif // POINT_H
