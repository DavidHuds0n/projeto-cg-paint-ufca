// render.c
// M�dulo de renderiza��o principal do projeto.
// A fun��o deste m�dulo � servir como uma camada de abstra��o, delegando
// a responsabilidade de desenhar cada objeto para o m�dulo 'objects'.

#include <stdio.h>      // Para printf (usado para depura��o).
#include <GL/glut.h>    // Necess�rio para qualquer opera��o OpenGL/GLUT.

#include "render.h"     // Inclui o pr�prio cabe�alho (declara��o de renderAllObjects).
#include "objects.h"    // Inclui para poder chamar a fun��o drawAllObjects().

/**
 * @brief Ponto de entrada para o processo de renderiza��o da cena.
 * Esta fun��o � chamada pelo displayCallback do GLUT e � respons�vel
 * por orquestrar o desenho de todos os objetos na tela.
 */
void renderAllObjects() {
    // Delega o trabalho de desenhar cada objeto individualmente para o m�dulo 'objects'.
    // Esta abordagem mant�m o m�dulo de renderiza��o simples e desacoplado da l�gica
    // espec�fica de cada tipo de objeto.
    drawAllObjects();

    // Log de depura��o para confirmar que a fun��o de renderiza��o foi chamada.
    // Pode ser removido na vers�o final do projeto.
    // printf("[Render] renderAllObjects() chamado.\n");
}
