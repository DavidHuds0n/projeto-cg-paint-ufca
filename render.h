// render.h
// Define a interface pública para o módulo de renderização.
// Este arquivo expõe as funções que podem ser chamadas por outras partes
// do programa (como o main.c) para iniciar o processo de desenho da cena.

#ifndef RENDER_H
#define RENDER_H

/**
 * @brief Ponto de entrada para o processo de renderização da cena.
 * Esta função é chamada pelo displayCallback do GLUT e é responsável
 * por orquestrar o desenho de todos os objetos na tela, delegando
 * a tarefa para o módulo 'objects'.
 */
void renderAllObjects();

#endif // RENDER_H
