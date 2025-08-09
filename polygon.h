// polygon.h
// Definição da estrutura GfxPolygon e protótipos de funções para manipulação de polígonos.

#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"   // Polígonos são compostos por Pontos
#include "config.h"  // NOVO: Para usar MAX_POLYGON_VERTICES

// Definição da estrutura GfxPolygon (renomeado de Polygon)
// Um polígono será uma coleção de pontos (vértices)
// O número máximo de vértices é definido em config.h
typedef struct {
    Point vertices[MAX_POLYGON_VERTICES];
    int numVertices;
} GfxPolygon; // RENOMEADO: Agora é GfxPolygon

// Protótipos das funções para GfxPolygon
// Cria um novo polígono vazio (pronto para adicionar vértices)
GfxPolygon createPolygon(); // ATUALIZADO: Retorna GfxPolygon

// Adiciona um vértice ao polígono
void addVertexToPolygon(GfxPolygon* poly, Point p); // ATUALIZADO: Recebe GfxPolygon*

// Desenha o polígono na tela
void drawPolygon(GfxPolygon* poly, int is_selected); // ATUALIZADO: Recebe GfxPolygon*

// Libera a memória alocada para um polígono
void freePolygon(GfxPolygon* poly); // ATUALIZADO: Recebe GfxPolygon*

#endif // POLYGON_H
