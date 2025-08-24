/**
 * @file polygon.h
 * @brief Define a estrutura de dados e a interface para o objeto Polígono.
 *
 * Um polígono é um objeto interativo, construído por uma sequência de
 * vértices e renderizado como uma forma fechada.
 */

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include "config.h"

// --- SEÇÃO DE ESTRUTURAS E TIPOS ---

/**
 * @brief Estrutura para representar um polígono 2D.
 *
 * Utiliza um array de tamanho fixo para armazenar os vértices.
 */
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES];   ///< Array que armazena os vértices do polígono.
    int numVertices;                        ///< Contador do número atual de vértices.
} GfxPolygon;

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

/**
 * @brief Cria e inicializa uma estrutura de polígono vazia.
 * @return GfxPolygon Uma nova estrutura GfxPolygon com numVertices = 0.
 */
GfxPolygon createPolygon();

/**
 * @brief Adiciona um novo vértice a um polígono existente.
 * @param poly Ponteiro para o polígono que será modificado.
 * @param p O Ponto (vértice) a ser adicionado.
 */
void addVertexToPolygon(GfxPolygon* poly, Point p);

/**
 * @brief Desenha um polígono na tela como uma forma fechada.
 *
 * A função desenha o contorno do polígono e, opcionalmente, os vértices e
 * o centroide, dependendo da configuração.
 * @param poly Ponteiro para o polígono a ser desenhado.
 * @param is_selected Flag (1 ou 0) que indica se o polígono está selecionado.
 */
void drawPolygon(GfxPolygon* poly, int is_selected);

/**
 * @brief Libera a memória interna de um Polígono, se houver.
 *
 * Esta função é mantida para consistência com a API de gerenciamento de objetos.
 * @param poly Ponteiro para a estrutura GfxPolygon.
 */
void freePolygon(GfxPolygon* poly);

#endif // POLYGON_H
