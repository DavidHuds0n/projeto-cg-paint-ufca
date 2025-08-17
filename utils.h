// utils.h
// Define a interface para funções utilitárias genéricas que podem ser
// usadas em múltiplos módulos do projeto.

#ifndef UTILS_H
#define UTILS_H

#include "objects.h" // Para a struct genérica Object
#include "point.h"   // Para a struct Point

/**
 * @brief Calcula o ponto central (centroide) de um objeto gráfico.
 * Essencial para realizar rotações e escalas a partir do centro do objeto.
 * @param obj Ponteiro para o objeto genérico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj);

#endif // UTILS_H
