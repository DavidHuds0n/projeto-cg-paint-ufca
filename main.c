// main.c
// Ponto de entrada e orquestrador principal do programa "Rabisquim".
// Este arquivo é responsável por:
// - Inicializar a biblioteca GLUT e criar a janela da aplicação.
// - Configurar o ambiente de renderização 2D do OpenGL.
// - Registrar as funções de callback para eventos de display, teclado e mouse.
// - Gerenciar o loop principal de eventos e a limpeza de recursos ao final.

// --- Bibliotecas ---
#include <GL/glut.h>        // Biblioteca GLUT para gerenciamento de janelas e eventos.
#include <stdio.h>          // Para funções de entrada/saída (printf, fprintf).
#include <stdlib.h>         // Para funções de alocação de memória (malloc, free).

#include "config.h"         // Configurações globais do projeto (tamanho da janela, etc.).

// --- Módulos do Projeto ---
#include "render.h"         // Funções de desenho.
#include "input.h"          // Gerenciamento de entrada do usuário.
#include "objects.h"        // Gerenciamento genérico de objetos gráficos.
#include "point.h"          // Funções específicas para Pontos.
#include "segment.h"        // Funções específicas para Segmentos.
#include "polygon.h"        // Funções específicas para Polígonos.
#include "transformations.h"  // Funções para transformações geométricas.


// --- Funções de Callback do GLUT ---

/**
 * @brief Função de callback do GLUT para renderização da cena.
 * É chamada sempre que o GLUT determina que a janela precisa ser redesenhada.
 * Limpa a tela, chama a rotina de desenho de todos os objetos e troca os buffers.
 */
void displayCallback() {
    // Limpa o buffer de cores, preenchendo-o com a cor de fundo definida em initOpenGL().
    glClear(GL_COLOR_BUFFER_BIT);

    // Delega a responsabilidade de desenhar para o módulo de renderização.
    renderAllObjects();

    // Com o double buffering, o desenho ocorre em um buffer de fundo.
    // glutSwapBuffers() troca o buffer de fundo pelo de exibição, mostrando a cena.
    glutSwapBuffers();
}

/**
 * @brief Configura o estado inicial do OpenGL para renderização 2D.
 * Define a cor de fundo da janela e estabelece o sistema de coordenadas
 * ortogonal 2D que mapeia para as dimensões da janela em pixels.
 */
void initOpenGL() {
    // Define a cor de fundo da janela como branco (RGBA).
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Seleciona a matriz de projeção para configurar o sistema de coordenadas.
    glMatrixMode(GL_PROJECTION);
    // Reseta a matriz de projeção para a matriz identidade.
    glLoadIdentity();
    // Configura um sistema de coordenadas 2D (mundo) que corresponde exatamente
    // às dimensões da janela em pixels (com 0,0 no canto inferior esquerdo).
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

/**
 * @brief Função principal e ponto de entrada do programa.
 * @param argc Número de argumentos da linha de comando.
 * @param argv Vetor de strings com os argumentos da linha de comando.
 * @return int Retorna 0 em caso de sucesso, 1 em caso de erro.
 */
int main(int argc, char **argv) {
    // 1. Inicialização do GLUT e da Janela
    glutInit(&argc, argv);
    // Define o modo de exibição: double buffer para evitar cintilação, e cores RGB.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);
    printf("[main.c] Janela '%s' criada com sucesso!\n", WINDOW_TITLE);

    // 2. Configurações Iniciais do Projeto
    initOpenGL();     // Aplica as configurações iniciais de renderização.
    initObjectList(); // Inicializa a lista global que armazenará os objetos.
    printf("[main.c] OpenGL e lista de objetos inicializados.\n");

    // 3. (OPCIONAL) Adição de Objetos de Teste para depuração inicial.
    Point* testPoint = (Point*)malloc(sizeof(Point));
    if (testPoint == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na alocação de memória para o ponto de teste!\n");
        return 1;
    }
    *testPoint = createPoint(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    addObject(OBJECT_TYPE_POINT, testPoint);
    printf("[main.c] Ponto de teste adicionado.\n");

    Segment* testSegment = (Segment*)malloc(sizeof(Segment));
    if (testSegment == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na alocação de memória para o segmento de teste!\n");
        free(testPoint);
        return 1;
    }
    *testSegment = createSegment(createPoint(100.0f, 100.0f), createPoint(300.0f, 300.0f));
    addObject(OBJECT_TYPE_SEGMENT, testSegment);
    printf("[main.c] Segmento de teste adicionado.\n");

    // Inicia o programa com o último objeto já selecionado para facilitar testes.
    g_selectedObjectIndex = g_numObjects - 1;

    // 4. Registro de Callbacks
    // Conecta os eventos do GLUT (desenho, teclado, mouse) às nossas funções.
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback); // Para eventos de arrastar do mouse.

    // 5. Início do Loop Principal
    printf("\n--- Rabisquim - Instruções ---\n");
    printf("Pressione 'P': Modo Criar Pontos\n");
    printf("Pressione 'L': Modo Criar Segmentos\n");
    printf("Pressione 'O': Modo Criar Polígonos\n");
    printf("Pressione 'S': Modo Seleção\n");
    printf("Pressione 'DEL': Excluir objeto selecionado\n");
    printf("Pressione 'ESC': Sair do programa\n");
    printf("------------------------------\n");

    // Inicia o loop principal do GLUT. O programa fica neste loop, aguardando
    // e processando eventos (mouse, teclado, etc.) até que a janela seja fechada.
    glutMainLoop();

    // 6. Limpeza de Recursos (executado após o fechamento da janela)
    printf("[main.c] Saindo do loop principal. Iniciando limpeza de objetos...\n");
    // Libera toda a memória alocada para os objetos gráficos para evitar memory leaks.
    clearAllObjects();
    printf("[main.c] Programa Rabisquim encerrado e memória liberada.\n");

    return 0; // Indica que o programa terminou com sucesso.
}
