/**
 * @file segment.h
 * @brief Define a estrutura de dados e a interface para o objeto Segmento de Reta.
 *
 * Um segmento � composto por dois pontos e serve como uma primitiva
 * fundamental para a constru��o de formas mais complexas, como pol�gonos.
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h"

// --- SE��O DE ESTRUTURAS E TIPOS ---

/**
 * @brief Estrutura para representar um segmento de reta 2D.
 */
typedef struct {
    Point p1;   ///< Ponto inicial do segmento.
    Point p2;   ///< Ponto final do segmento.
} Segment;


// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Cria e inicializa uma estrutura Segment com os dois pontos fornecidos.
 * @param p1 O ponto inicial do segmento.
 * @param p2 O ponto final do segmento.
 * @return Segment Uma nova estrutura Segment.
 */
Segment createSegment(Point p1, Point p2);

/**
 * @brief Desenha um segmento de reta na tela.
 *
 * A cor do segmento muda dependendo de seu estado de sele��o.
 * @param s Ponteiro para a estrutura Segment a ser desenhada.
 * @param is_selected Flag (1 ou 0) que indica se o segmento est� selecionado.
 */
void drawSegment(Segment* s, int is_selected);

/**
 * @brief Calcula a menor dist�ncia entre um ponto e um segmento de reta.
 *
 * Este algoritmo encontra o ponto mais pr�ximo no segmento (n�o na reta infinita)
 * e retorna a dist�ncia euclidiana at� ele. � fundamental para a sele��o por proximidade.
 * @param p O ponto de refer�ncia (ex: o clique do mouse).
 * @param s O segmento de reta a ser testado.
 * @return float A menor dist�ncia euclidiana entre o ponto e o segmento.
 */
float distPointSegment(Point p, Segment s);

/**
 * @brief Libera a mem�ria interna de um Segmento, se houver.
 *
 * Esta fun��o � mantida para consist�ncia com a API de gerenciamento de objetos.
 * @param s Ponteiro para a estrutura Segment.
 */
void freeSegment(Segment* s);

#endif // SEGMENT_H
