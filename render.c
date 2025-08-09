#include <stdio.h>     // Para printf (para depura��o, pode ser removido depois)
#include <GL/glut.h>   // Necess�rio para qualquer opera��o OpenGL/GLUT
#include "render.h"    // Inclui o pr�prio cabe�alho
#include "objects.h"   // Precisa incluir objects.h para chamar drawAllObjects()

// Fun��o principal de renderiza��o. � chamada pelo GLUT atrav�s de displayCallback em main.c.
void renderAllObjects() {
    // Chama a fun��o do m�dulo 'objects' para desenhar todos os objetos que est�o na lista.
    // Toda a l�gica de qual objeto desenhar (ponto, segmento, etc.) agora est� em objects.c.
    drawAllObjects();

    printf("[Render] renderAllObjects() chamado.\n"); // Mensagem de depura��o
}
