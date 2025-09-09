/**
 * @file convexhull.h
 * @brief Define a interface p�blica para os algoritros de fecho convexo.
 */
#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "polygon.h"

/**
 * @brief Calcula o fecho convexo de um pol�gono usando o algoritmo Marcha de Jarvis.
 * @param poly O pol�gono original cujos v�rtices ser�o analisados.
 * @return GfxPolygon* Um ponteiro para um novo pol�gono convexo alocado dinamicamente.
 */
GfxPolygon* jarvisMarch(GfxPolygon* poly);

#endif // CONVEXHULL_H
