#include <stdio.h>     // Para printf (para depuração, pode ser removido depois)
#include <GL/glut.h>   // Necessário para qualquer operação OpenGL/GLUT
#include "render.h"    // Inclui o próprio cabeçalho
#include "objects.h"   // Precisa incluir objects.h para chamar drawAllObjects()

// Função principal de renderização. É chamada pelo GLUT através de displayCallback em main.c.
void renderAllObjects() {
    // Chama a função do módulo 'objects' para desenhar todos os objetos que estão na lista.
    // Toda a lógica de qual objeto desenhar (ponto, segmento, etc.) agora está em objects.c.
    drawAllObjects();

    printf("[Render] renderAllObjects() chamado.\n"); // Mensagem de depuração
}
