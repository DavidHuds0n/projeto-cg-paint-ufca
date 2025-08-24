/**
 * @file main.c
 * @brief Ponto de entrada e orquestrador principal do programa "Rabisquim".
 *
 * Este arquivo eh responsavel por inicializar a aplicacao, configurar o ambiente
 * grafico e registrar as funcoes de callback para gerenciar eventos do usuario.
 */

// --- SEÇÃO DE CABEÇALHOS ---
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
#include "animation.h"
#include "file_io.h"

// --- SEÇÃO DE FUNÇÕES AUXILIARES ---

/**
 * @brief Funcao de callback para redesenhar a tela.
 *
 * Limpa o buffer de cor e chama a funcao de renderizacao da cena.
 */
void displayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderAllObjects();
    glutSwapBuffers();
}

/**
 * @brief Configura o ambiente de renderizacao 2D do OpenGL.
 *
 * Define a cor de fundo, o modo de matriz e o sistema de coordenadas ortogonal.
 */
void initOpenGL() {
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f); // Cor de fundo: cinza claro
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

/**
 * @brief Callback do temporizador para controlar a animacao e redesenhar a tela.
 *
 * Chamado repetidamente em intervalos fixos para criar um loop de animacao.
 */
static void timerCallback(int value) {
    static int last = 0;
    int now = glutGet(GLUT_ELAPSED_TIME);
    if (last == 0) last = now;
    float dt = (now - last) / 1000.0f;
    last = now;

    anim_step(dt);
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
}

/**
 * @brief Funcao de limpeza de memoria a ser chamada ao sair do programa.
 *
 * Registrada com atexit() para garantir a liberacao de recursos.
 */
static void cleanup_on_exit(void) {
    clearAllObjects();
    printf("[INFO] Recursos alocados liberados.\n");
}


// --- SEÇÃO DE FUNÇÃO PRINCIPAL ---

int main(int argc, char **argv) {
    // 1. Inicializa o GLUT e a janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    // 2. Inicializa os modulos do projeto
    initOpenGL();
    initObjectList();
    anim_init_all();
    atexit(cleanup_on_exit); // Registra a funcao de limpeza

    // 3. Registra as funcoes de callback para gerenciar eventos
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutPassiveMotionFunc(passiveMotionCallback);
    glutSpecialFunc(specialKeysCallback);
    glutTimerFunc(16, timerCallback, 0);

    // 4. Bloco de instrucoes para o console (crucial para o usuario)
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
    printf("          -> No modo Rotacao, use as SETAS ESQUERDA/DIREITA do teclado para girar.\n");
    printf("  'e' -> Entrar no modo Escala.\n");
    printf("          -> No modo Escala, use as teclas '+' e '-' para aumentar ou diminuir.\n");
    printf("  'm' -> Entrar no modo Reflexao (Espelho).\n");
    printf("          -> No modo Reflexao, pressione 'x' ou 'y' para refletir no eixo.\n");
    printf("  'h' -> Entrar no modo Cisalhamento (Shear).\n\n");
    printf("--- Animacoes (com objetos selecionados) ---\n");
    printf("  'b' -> Alterna o modo de 'pular' (bounce) no objeto selecionado.\n");
    printf("  'v' -> Aplica uma velocidade inicial ao objeto na direcao do mouse.\n");
    printf("  'g' -> Alterna a gravidade no objeto selecionado.\n");
    printf("  '[' -> Diminui a velocidade da animacao do objeto selecionado.\n");
    printf("  ']' -> Aumenta a velocidade da animacao do objeto selecionado.\n\n");
    printf("--- Controles de Arquivo ---\n");
    printf("  'F5' -> Salva a cena atual no arquivo 'scene.txt'.\n");
    printf("  'F9' -> Carrega a cena do arquivo 'scene.txt'.\n\n");
    printf("--- Controles Gerais ---\n");
    printf("  'ESC' -> Sair do programa.\n\n");
    printf("======================================================================\n");

    // 5. Inicia o loop principal de eventos do GLUT
    glutMainLoop();

    // 6. Apos o glutMainLoop(), o programa termina.
    return 0;
}
