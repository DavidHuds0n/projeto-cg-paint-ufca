// point.h
// Define a estrutura de dados e a interface pública para o objeto Ponto.
// O Ponto é a primitiva geométrica mais fundamental do projeto.

#ifndef POINT_H
#define POINT_H

// Estrutura para representar um ponto 2D no sistema de coordenadas do mundo.
typedef struct {
    float x; // Coordenada no eixo X.
    float y; // Coordenada no eixo Y.
} Point;


// --- Protótipos das Funções ---

/**
 * @brief Cria (inicializa) uma estrutura Point com as coordenadas fornecidas.
 * @param x A coordenada X do ponto.
 * @param y A coordenada Y do ponto.
 * @return Point Uma nova estrutura Point com os valores especificados.
 */
Point createPoint(float x, float y);

/**
 * @brief Desenha um ponto na tela.
 * A cor do ponto muda se ele estiver selecionado.
 * @param p Ponteiro para a estrutura Point a ser desenhada.
 * @param is_selected Flag (1 ou 0) que indica se o ponto está selecionado.
 */
void drawPoint(Point* p, int is_selected);

/**
 * @brief Libera a memória interna de um Ponto, se houver.
 * Mantida para consistência com a API de gerenciamento de objetos.
 * @param p Ponteiro para a estrutura Point.
 */
void freePoint(Point* p);

#endif // POINT_H
