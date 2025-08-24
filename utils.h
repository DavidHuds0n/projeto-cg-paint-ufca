/**
 * @file utils.h
 * @brief Define a interface para funções utilitárias genéricas.
 *
 * Este módulo contém funções de apoio que podem ser usadas em múltiplos
 * arquivos do projeto, promovendo a reutilização de código.
 */

#ifndef UTILS_H
#define UTILS_H

#include "objects.h" // Para a struct genérica Object
#include "point.h"   // Para a struct Point

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

/**
 * @brief Calcula o ponto central (centroide) de um objeto gráfico.
 *
 * Essencial para realizar transformações como rotações e escalas a partir do
 * centro do próprio objeto, em vez de a partir da origem do sistema de coordenadas.
 * @param obj Ponteiro para o objeto genérico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj);

#endif // UTILS_H
