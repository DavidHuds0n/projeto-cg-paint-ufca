/**
 * @file render.c
 * @brief Implementa��o da fun��o de renderiza��o principal do projeto.
 *
 * Este m�dulo serve como uma camada de abstra��o, delegando o
 * desenho de todos os objetos para o m�dulo de gerenciamento de objetos.
 */

#include <stdio.h>
#include <GL/glut.h>
#include "render.h"
#include "objects.h"

// --- SE��O DE FUN��ES P�BLICAS ---

void renderAllObjects() {
    // Delega a responsabilidade de desenhar todos os objetos
    // para a fun��o drawAllObjects() do m�dulo 'objects'.
    drawAllObjects();
}
