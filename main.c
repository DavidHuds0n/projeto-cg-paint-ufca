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


// --- SEÇÃO DE VARIÁVEIS GLOBAIS DA JANELA ---
int g_windowWidth = WINDOW_WIDTH;
int g_windowHeight = WINDOW_HEIGHT;

// --- SEÇÃO DE FUNÇÕES AUXILIARES ---

/**
 * @brief Callback para quando a janela é redimensionada.
 * * Responsável por atualizar o viewport e a projeção ortográfica do OpenGL
 * para corresponderem às novas dimensões da janela.
 * @param w A nova largura da janela.
 * @param h A nova altura da janela.
 */
void reshapeCallback(int w, int h) {
    // Atualiza as variáveis globais com as novas dimensões.
    g_windowWidth = w;
    g_windowHeight = h > 0 ? h : 1; // Evita divisão por zero se a altura for 0.

    // Define a área de desenho do OpenGL para ocupar a janela inteira.
    glViewport(0, 0, g_windowWidth, g_windowHeight);

    // Atualiza o sistema de coordenadas (projeção) para corresponder ao novo tamanho.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, g_windowWidth, 0, g_windowHeight);

    // Volta para a matriz de modelo-vista para as operações de desenho.
    glMatrixMode(GL_MODELVIEW);
}

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
    glutReshapeFunc(reshapeCallback);

    // 4. Bloco de instrucoes para o console (crucial para o usuario)
    printf("\n======================= Rabisquim - Instrucoes =======================\n\n");
    printf("--- Criacao de Objetos ---\n");
    printf("  'p' -> Modo Criar Ponto: Clique com o botao esquerdo para criar.\n");
    printf("  'l' -> Modo Criar Segmento: Clique 2 vezes para definir o inicio e o fim.\n");
    printf("  'o' -> Modo Criar Poligono: Clique para adicionar vertices. Botao direito para finalizar.\n\n");
    printf("--- Selecao e Edicao Basica ---\n");
    printf("  's' -> Modo de Selecao: Clique em um objeto para seleciona-lo.\n");
    printf("  'DEL' -> Excluir: Apaga o objeto que estiver selecionado.\n\n");
    printf("--- Transformacoes (com um objeto selecionado) ---\n");
    printf("  't' -> Transladar (Mover): Arraste o objeto selecionado com o mouse.\n");
    printf("  'r' -> Rotacao: Use as SETAS ESQUERDA/DIREITA para girar.\n");
    printf("  'e' -> Escala: Use as teclas '+' e '-' para aumentar ou diminuir.\n");
    printf("  'm' -> Reflexao (Espelho): Pressione 'x' ou 'y' para refletir no eixo.\n");
    printf("  'h' -> Cisalhamento (Shear): Use as SETAS do teclado para deformar.\n\n");
    printf("--- Algoritmos Geometricos ---\n");
    printf("  'c' -> Fecho Convexo: Com um poligono selecionado, transforma-o em seu fecho convexo.\n\n");
    printf("--- Animacoes (com um objeto selecionado) ---\n");
    printf("  'b' -> Alterna o modo de 'pular' (bounce).\n");
    printf("  'v' -> Aplica uma velocidade inicial na direcao do mouse.\n");
    printf("  'g' -> Alterna a gravidade.\n");
    printf("  '[' e ']' -> Diminui ou aumenta a velocidade.\n\n");
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
