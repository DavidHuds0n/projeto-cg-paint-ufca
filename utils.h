/**
 * @file utils.h
 * @brief Define a interface para fun��es utilit�rias gen�ricas.
 *
 * Este m�dulo cont�m fun��es de apoio que podem ser usadas em m�ltiplos
 * arquivos do projeto, promovendo a reutiliza��o de c�digo.
 */

#ifndef UTILS_H
#define UTILS_H

#include "objects.h" // Para a struct gen�rica Object
#include "point.h"   // Para a struct Point

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Calcula o ponto central (centroide) de um objeto gr�fico.
 *
 * Essencial para realizar transforma��es como rota��es e escalas a partir do
 * centro do pr�prio objeto, em vez de a partir da origem do sistema de coordenadas.
 * @param obj Ponteiro para o objeto gen�rico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj);

#endif // UTILS_H
