// segment.h
// Define a estrutura de dados e a interface pública para o objeto Segmento de Reta.
// Um Segmento é composto por dois Pontos.

#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h" // Dependência necessária, pois um segmento é definido por pontos.

// Estrutura para representar um segmento de reta 2D.
typedef struct {
    Point p1; // Ponto inicial do segmento.
    Point p2; // Ponto final do segmento.
} Segment;


// --- Protótipos das Funções ---

/**
 * @brief Cria (inicializa) uma estrutura Segment com os dois pontos fornecidos.
 * @param p1 O ponto inicial do segmento.
 * @param p2 O ponto final do segmento.
 * @return Segment Uma nova estrutura Segment.
 */
Segment createSegment(Point p1, Point p2);

/**
 * @brief Desenha um segmento de reta na tela.
 * A cor do segmento muda se ele estiver selecionado.
 * @param s Ponteiro para a estrutura Segment a ser desenhada.
 * @param is_selected Flag (1 ou 0) que indica se o segmento está selecionado.
 */
void drawSegment(Segment* s, int is_selected);

/**
 * @brief Calcula a menor distância entre um ponto e um segmento de reta.
 * Este algoritmo encontra o ponto mais próximo no *segmento* (não na reta infinita)
 * e retorna a distância até ele. É fundamental para a seleção por proximidade.
 * @param p O ponto de referência (ex: o clique do mouse).
 * @param s O segmento de reta a ser testado.
 * @return float A menor distância euclidiana entre o ponto e o segmento.
 */
float distPointSegment(Point p, Segment s);

/**
 * @brief Libera a memória interna de um Segmento, se houver.
 * Mantida para consistência com a API de gerenciamento de objetos.
 * @param s Ponteiro para a estrutura Segment.
 */
void freeSegment(Segment* s);

#endif // SEGMENT_H
