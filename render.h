/**
 * @file render.h
 * @brief Define a interface p�blica para o m�dulo de renderiza��o.
 *
 * Este arquivo exp�e a fun��o principal de desenho que � chamada pela
 * biblioteca GLUT para renderizar a cena.
 */

#ifndef RENDER_H
#define RENDER_H

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Ponto de entrada para o processo de renderiza��o da cena.
 *
 * Esta fun��o � chamada pelo displayCallback do GLUT e � respons�vel
 * por orquestrar o desenho de todos os objetos na tela, delegando
 * a tarefa para o m�dulo 'objects'.
 */
void renderAllObjects();

#endif // RENDER_H
