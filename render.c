// render.c
// Módulo de renderização principal do projeto.
// A função deste módulo é servir como uma camada de abstração, delegando
// a responsabilidade de desenhar cada objeto para o módulo 'objects'.

#include <stdio.h>      // Para printf (usado para depuração).
#include <GL/glut.h>    // Necessário para qualquer operação OpenGL/GLUT.

#include "render.h"     // Inclui o próprio cabeçalho (declaração de renderAllObjects).
#include "objects.h"    // Inclui para poder chamar a função drawAllObjects().

/**
 * @brief Ponto de entrada para o processo de renderização da cena.
 * Esta função é chamada pelo displayCallback do GLUT e é responsável
 * por orquestrar o desenho de todos os objetos na tela.
 */
void renderAllObjects() {
    // Delega o trabalho de desenhar cada objeto individualmente para o módulo 'objects'.
    // Esta abordagem mantém o módulo de renderização simples e desacoplado da lógica
    // específica de cada tipo de objeto.
    drawAllObjects();

    // Log de depuração para confirmar que a função de renderização foi chamada.
    // Pode ser removido na versão final do projeto.
    // printf("[Render] renderAllObjects() chamado.\n");
}
