/**
 * @file render.h
 * @brief Define a interface pública para o módulo de renderização.
 *
 * Este arquivo expõe a função principal de desenho que é chamada pela
 * biblioteca GLUT para renderizar a cena.
 */

#ifndef RENDER_H
#define RENDER_H

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

/**
 * @brief Ponto de entrada para o processo de renderização da cena.
 *
 * Esta função é chamada pelo displayCallback do GLUT e é responsável
 * por orquestrar o desenho de todos os objetos na tela, delegando
 * a tarefa para o módulo 'objects'.
 */
void renderAllObjects();

#endif // RENDER_H
