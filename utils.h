// utils.h
// Define a interface para fun��es utilit�rias gen�ricas que podem ser
// usadas em m�ltiplos m�dulos do projeto.

#ifndef UTILS_H
#define UTILS_H

#include "objects.h" // Para a struct gen�rica Object
#include "point.h"   // Para a struct Point

/**
 * @brief Calcula o ponto central (centroide) de um objeto gr�fico.
 * Essencial para realizar rota��es e escalas a partir do centro do objeto.
 * @param obj Ponteiro para o objeto gen�rico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj);

#endif // UTILS_H
