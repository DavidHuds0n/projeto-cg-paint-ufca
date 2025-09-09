/**
 * @file convexhull.h
 * @brief Define a interface pública para os algoritros de fecho convexo.
 */
#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "polygon.h"

/**
 * @brief Calcula o fecho convexo de um polígono usando o algoritmo Marcha de Jarvis.
 * @param poly O polígono original cujos vértices serão analisados.
 * @return GfxPolygon* Um ponteiro para um novo polígono convexo alocado dinamicamente.
 */
GfxPolygon* jarvisMarch(GfxPolygon* poly);

#endif // CONVEXHULL_H
