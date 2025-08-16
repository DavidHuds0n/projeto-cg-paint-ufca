// render.h
// Define a interface p�blica para o m�dulo de renderiza��o.
// Este arquivo exp�e as fun��es que podem ser chamadas por outras partes
// do programa (como o main.c) para iniciar o processo de desenho da cena.

#ifndef RENDER_H
#define RENDER_H

/**
 * @brief Ponto de entrada para o processo de renderiza��o da cena.
 * Esta fun��o � chamada pelo displayCallback do GLUT e � respons�vel
 * por orquestrar o desenho de todos os objetos na tela, delegando
 * a tarefa para o m�dulo 'objects'.
 */
void renderAllObjects();

#endif // RENDER_H
