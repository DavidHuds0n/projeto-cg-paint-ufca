// polygon.h
// Define a estrutura de dados e a interface pública para o objeto Polígono.
// Este objeto é construído interativamente e renderizado como uma área preenchida.

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include "config.h"

// Estrutura para representar um polígono 2D.
// Utiliza um array de tamanho fixo para armazenar os vértices.
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES]; // Array para armazenar os vértices do polígono.
    int numVertices;                      // Contador do número atual de vértices no polígono.
} GfxPolygon;


// --- Protótipos das Funções ---

/**
 * @brief Cria (inicializa) uma estrutura de polígono vazia.
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
 * @brief Desenha um polígono preenchido na tela.
 * Inclui lógica para dar feedback visual durante a criação e para
 * destacar o objeto quando selecionado.
 * @param poly Ponteiro para o polígono a ser desenhado.
 * @param is_selected Flag (1 ou 0) que indica se o polígono está selecionado.
 */
void drawPolygon(GfxPolygon* poly, int is_selected);

/**
 * @brief Libera a memória interna de um Polígono, se houver.
 * Mantida para consistência com a API de gerenciamento de objetos.
 * @param poly Ponteiro para a estrutura GfxPolygon.
 */
void freePolygon(GfxPolygon* poly);

#endif // POLYGON_H
