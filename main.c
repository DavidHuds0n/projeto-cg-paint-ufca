// main.c
// Ponto de entrada e orquestrador principal do programa "Rabisquim".
// Este arquivo é responsável por:
// - Inicializar a biblioteca GLUT e criar a janela da aplicação.
// - Configurar o ambiente de renderização 2D do OpenGL.
// - Registrar as funções de callback para eventos de display, teclado e mouse.
// - Gerenciar o loop principal de eventos e a limpeza de recursos ao final.


/* IGNORE
cd /home/vic/Documentos/GitHub/projeto-cg-paint-ufca/
gcc *.c -o Rabisquim -lGL -lGLU -lglut -lm
./Rabisquim
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "render.h"
#include "input.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "transformations.h"
#include "animation.h""

void displayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderAllObjects();
    glutSwapBuffers();
}

void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

static void timerCallback(int value){
    static int last = 0;
    int now = glutGet(GLUT_ELAPSED_TIME);
    if(last==0) last = now;
    float dt = (now - last) / 1000.0f;
    last = now;

    anim_step(dt);          // avança as animações
    glutPostRedisplay();    // redesenha
    glutTimerFunc(16, timerCallback, 0); // ~60 FPS
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    initOpenGL();
    initObjectList();
    anim_init_all();
    // Registro de Callbacks
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutPassiveMotionFunc(passiveMotionCallback);

    // Adicionado para capturar teclas especiais como as setas
    glutSpecialFunc(specialKeysCallback);
    glutTimerFunc(16, timerCallback, 0); // inicia o loop das animações

    // --- INSTRUÇÕES ATUALIZADAS E COMPLETAS ---
    printf("\n======================= Rabisquim - Instrucoes =======================\n\n");

    printf("--- Criacao de Objetos ---\n");
    printf("  'p' -> Modo Criar Ponto: Clique com o botao esquerdo para criar.\n");
    printf("  'l' -> Modo Criar Segmento: Clique 2 vezes para definir o inicio e o fim.\n");
    printf("  'o' -> Modo Criar Poligono: Clique para adicionar vertices. Botao direito para finalizar.\n\n");

    printf("--- Selecao e Edicao Basica ---\n");
    printf("  's' -> Modo de Selecao: Clique em um objeto para seleciona-lo.\n");
    printf("  't' -> Transladar (Mover): Com um objeto selecionado, entre neste modo e arraste-o.\n");
    printf("  'DEL' -> Excluir: Apaga o objeto que estiver selecionado.\n\n");

    printf("--- Transformacoes (com um objeto selecionado) ---\n");
    printf("  'r' -> Entrar no modo Rotacao.\n");
    printf("         -> No modo Rotacao, use as SETAS ESQUERDA/DIREITA do teclado para girar.\n");
    printf("  'e' -> Entrar no modo Escala.\n");
    printf("         -> No modo Escala, use as teclas '+' e '-' para aumentar ou diminuir.\n");
    printf("  'm' -> Entrar no modo Reflexao (Espelho).\n");
    printf("         -> No modo Reflexao, pressione 'x' ou 'y' para refletir no eixo.\n");
    printf("  'h' -> Entrar no modo Cisalhamento (Shear).\n\n");

    printf("--- Animações (com objetos selecionados) ---\n");
    printf("  'b' -> Entrar no modo boing.\n");
    printf("  'v' -> Entrar no modo perceguição.\n");
    printf("  'g' -> Entrar no modo gravidade.\n");
    printf("  ']' -> Aumenta a velocidade da animação.\n");
    printf("  '[' -> Diminui a velocidade da animação.\n");
    printf("--- Controles Gerais ---\n");
    printf("  'ESC' -> Sair do programa.\n\n");

    printf("======================================================================\n");


    glutMainLoop();

    clearAllObjects();
    return 0;
}
