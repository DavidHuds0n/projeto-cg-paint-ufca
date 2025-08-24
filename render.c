/**
 * @file render.c
 * @brief Implementação da função de renderização principal do projeto.
 *
 * Este módulo serve como uma camada de abstração, delegando o
 * desenho de todos os objetos para o módulo de gerenciamento de objetos.
 */

#include <stdio.h>
#include <GL/glut.h>
#include "render.h"
#include "objects.h"

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

void renderAllObjects() {
    // Delega a responsabilidade de desenhar todos os objetos
    // para a função drawAllObjects() do módulo 'objects'.
    drawAllObjects();
}
