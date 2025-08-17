// polygon.h
// Define a estrutura de dados e a interface p�blica para o objeto Pol�gono.
// Este objeto � constru�do interativamente e renderizado como uma �rea preenchida.

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include "config.h"

// Estrutura para representar um pol�gono 2D.
// Utiliza um array de tamanho fixo para armazenar os v�rtices.
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES]; // Array para armazenar os v�rtices do pol�gono.
    int numVertices;                      // Contador do n�mero atual de v�rtices no pol�gono.
} GfxPolygon;


// --- Prot�tipos das Fun��es ---

/**
 * @brief Cria (inicializa) uma estrutura de pol�gono vazia.
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
 * @brief Desenha um pol�gono preenchido na tela.
 * Inclui l�gica para dar feedback visual durante a cria��o e para
 * destacar o objeto quando selecionado.
 * @param poly Ponteiro para o pol�gono a ser desenhado.
 * @param is_selected Flag (1 ou 0) que indica se o pol�gono est� selecionado.
 */
void drawPolygon(GfxPolygon* poly, int is_selected);

/**
 * @brief Libera a mem�ria interna de um Pol�gono, se houver.
 * Mantida para consist�ncia com a API de gerenciamento de objetos.
 * @param poly Ponteiro para a estrutura GfxPolygon.
 */
void freePolygon(GfxPolygon* poly);

#endif // POLYGON_H
