// polygon.h
// Defini��o da estrutura GfxPolygon e prot�tipos de fun��es para manipula��o de pol�gonos.

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"   // Pol�gonos s�o compostos por Pontos
#include "config.h"  // NOVO: Para usar MAX_POLYGON_VERTICES

// Defini��o da estrutura GfxPolygon (renomeado de Polygon)
// Um pol�gono ser� uma cole��o de pontos (v�rtices)
// O n�mero m�ximo de v�rtices � definido em config.h
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES];
    int numVertices;
} GfxPolygon; // RENOMEADO: Agora � GfxPolygon

// Prot�tipos das fun��es para GfxPolygon
// Cria um novo pol�gono vazio (pronto para adicionar v�rtices)
GfxPolygon createPolygon(); // ATUALIZADO: Retorna GfxPolygon

// Adiciona um v�rtice ao pol�gono
void addVertexToPolygon(GfxPolygon* poly, Point p); // ATUALIZADO: Recebe GfxPolygon*

// Desenha o pol�gono na tela
void drawPolygon(GfxPolygon* poly, int is_selected); // ATUALIZADO: Recebe GfxPolygon*

// Libera a mem�ria alocada para um pol�gono
void freePolygon(GfxPolygon* poly); // ATUALIZADO: Recebe GfxPolygon*

#endif // POLYGON_H
