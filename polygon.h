/**
 * @file polygon.h
 * @brief Define a estrutura de dados e a interface para o objeto Pol�gono.
 *
 * Um pol�gono � um objeto interativo, constru�do por uma sequ�ncia de
 * v�rtices e renderizado como uma forma fechada.
 */

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include "config.h"

// --- SE��O DE ESTRUTURAS E TIPOS ---

/**
 * @brief Estrutura para representar um pol�gono 2D.
 *
 * Utiliza um array de tamanho fixo para armazenar os v�rtices.
 */
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES];   ///< Array que armazena os v�rtices do pol�gono.
    int numVertices;                        ///< Contador do n�mero atual de v�rtices.
} GfxPolygon;

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Cria e inicializa uma estrutura de pol�gono vazia.
 * @return GfxPolygon Uma nova estrutura GfxPolygon com numVertices = 0.
 */
GfxPolygon createPolygon();

/**
 * @brief Adiciona um novo v�rtice a um pol�gono existente.
 * @param poly Ponteiro para o pol�gono que ser� modificado.
 * @param p O Ponto (v�rtice) a ser adicionado.
 */
void addVertexToPolygon(GfxPolygon* poly, Point p);

/**
 * @brief Desenha um pol�gono na tela como uma forma fechada.
 *
 * A fun��o desenha o contorno do pol�gono e, opcionalmente, os v�rtices e
 * o centroide, dependendo da configura��o.
 * @param poly Ponteiro para o pol�gono a ser desenhado.
 * @param is_selected Flag (1 ou 0) que indica se o pol�gono est� selecionado.
 */
void drawPolygon(GfxPolygon* poly, int is_selected);

/**
 * @brief Libera a mem�ria interna de um Pol�gono, se houver.
 *
 * Esta fun��o � mantida para consist�ncia com a API de gerenciamento de objetos.
 * @param poly Ponteiro para a estrutura GfxPolygon.
 */
void freePolygon(GfxPolygon* poly);

#endif // POLYGON_H
