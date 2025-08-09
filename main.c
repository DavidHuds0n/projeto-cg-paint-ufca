// main.c
// Gerencia a inicialização do programa, configuração do OpenGL e o loop principal do GLUT.

// Bibliotecas padrão do C e da GLUT
#include <GL/glut.h>   // Biblioteca GLUT para gerenciamento de janelas e eventos.
#include <stdio.h>     // Para funções de entrada/saída (printf, fprintf).
#include <stdlib.h>    // Para funções de alocação de memória (malloc, free).

// Inclui as configurações globais do projeto (altura, largura da janela, etc.)
#include "config.h"    // Inclui as definições centralizadas

// Cabeçalhos dos módulos do projeto
#include "render.h"    // Funções de desenho.
#include "input.h"     // Gerenciamento de entrada do usuário (teclado e mouse).
#include "objects.h"   // Gerenciamento genérico de objetos gráficos.
#include "point.h"     // Definições e funções para objetos Point.
#include "segment.h"   // Definições e funções para objetos Segment.
#include "polygon.h"   // Definições e funções para objetos GfxPolygon.
#include "transformations.h" // NOVO: Incluir o cabeçalho de transformações

// --- Funções de Callback do GLUT ---

// displayCallback: Função chamada pelo GLUT para redesenhar o conteúdo da janela.
void displayCallback() {
    // Limpa o buffer de cores com a cor de fundo definida.
    glClear(GL_COLOR_BUFFER_BIT);

    // Chama a função que desenha todos os objetos gráficos gerenciados pelo programa.
    renderAllObjects();

    // IMPORTANTE: glutSwapBuffers() é essencial para exibir o que foi desenhado
    // quando o modo de exibição é GLUT_DOUBLE (buffer duplo).
    glutSwapBuffers();
}

// initOpenGL: Configura o ambiente inicial do OpenGL para renderização 2D.
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Define a cor de fundo da janela como branco (RGBA).

    glMatrixMode(GL_PROJECTION);      // Define o modo da matriz atual para projeção.
    glLoadIdentity();                 // Carrega a matriz identidade, redefinindo a projeção.
    // Configura um sistema de coordenadas 2D ortogonal usando as constantes de config.h
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

// --- Função Principal do Programa ---
int main(int argc, char **argv) {
    // 1. Inicialização do GLUT: Configura a biblioteca e a janela da aplicação.
    glutInit(&argc, argv);                             // Inicializa a biblioteca GLUT.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);       // Define o modo de exibição: double buffer para evitar cintilação, e cores RGB.
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);   // Define as dimensões iniciais da janela usando config.h.
    glutCreateWindow(WINDOW_TITLE);                    // Cria a janela principal com o título especificado usando config.h.
    printf("[main.c] Janela '%s' criada com sucesso!\n", WINDOW_TITLE);

    // 2. Configurações Iniciais do Projeto: Prepara o ambiente OpenGL e o gerenciador de objetos.
    initOpenGL();      // Aplica as configurações iniciais do OpenGL.
    initObjectList();  // Inicializa a lista global onde os objetos gráficos serão armazenados e gerenciados.
    printf("[main.c] OpenGL e lista de objetos inicializados.\n");

    // 3. Adição de Objetos de Teste: Cria e adiciona um ponto e um segmento para demonstração inicial.

    // Ponto de Teste: Posicionado no centro da janela.
    Point* testPoint = (Point*)malloc(sizeof(Point));
    if (testPoint == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na alocação de memória para o ponto de teste!\n");
        return 1; // Retorna com erro se a alocação falhar.
    }
    *testPoint = createPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // Usa WINDOW_WIDTH/HEIGHT de config.h.
    addObject(OBJECT_TYPE_POINT, testPoint); // Adiciona o ponto à lista de objetos.
    printf("[main.c] Ponto de teste adicionado.\n");

    // Segmento de Teste: Desenhado de (100,100) a (300,300).
    Segment* testSegment = (Segment*)malloc(sizeof(Segment));
    if (testSegment == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na alocação de memória para o segmento de teste!\n");
        free(testPoint); // Libera o ponto de teste se a alocação do segmento falhar.
        return 1; // Retorna com erro.
    }
    // Define os dois pontos que compõem o segmento.
    Point segP1 = createPoint(100.0f, 100.0f);
    Point segP2 = createPoint(300.0f, 300.0f);
    *testSegment = createSegment(segP1, segP2); // Cria o segmento.
    addObject(OBJECT_TYPE_SEGMENT, testSegment); // Adiciona o segmento à lista de objetos.
    printf("[main.c] Segmento de teste adicionado.\n");

    // Seleciona o último objeto adicionado (o segmento de teste) para que ele seja exibido como selecionado ao iniciar.
    g_selectedObjectIndex = g_numObjects - 1;
    printf("[main.c] Total de objetos: %d. Objeto selecionado inicial: %d.\n", g_numObjects, g_selectedObjectIndex);

    // 4. Registro de Callbacks do GLUT: Conecta as funções de manipulação de eventos do programa ao GLUT.
    glutDisplayFunc(displayCallback);     // Registra a função de desenho principal.
    glutKeyboardFunc(keyboardCallback);   // Registra a função para lidar com eventos de teclado.
    glutMouseFunc(mouseCallback);         // Registra a função para lidar com eventos do mouse.
    glutMotionFunc(motionCallback);       // NOVO: Registra a função para lidar com o movimento do mouse enquanto um botão está pressionado.

    // 5. Loop Principal do GLUT: Inicia o processamento de eventos do sistema operacional.
    printf("\n--- Rabisquim - Instruções ---\n");
    printf("Pressione 'P': Modo Criar Pontos (clique para adicionar um ponto)\n");
    printf("Pressione 'L': Modo Criar Segmentos (dois cliques para definir um segmento)\n");
    printf("Pressione 'O': Modo Criar Polígonos (clique ESQUERDO para adicionar vértices, clique DIREITO para finalizar)\n");
    printf("Pressione 'S': Modo Seleção (clique em pontos, segmentos ou polígonos para selecioná-los e arrastar)\n");
    printf("Pressione 'DEL': Excluir objeto selecionado\n");
    printf("Pressione 'ESC': Sair do programa\n");
    printf("------------------------------\n");
    printf("[main.c] Entrando no loop principal do GLUT. Feche a janela para sair.\n");
    glutMainLoop();

    // 6. Limpeza de Recursos: Esta seção é executada quando o loop principal do GLUT termina (ex: janela fechada).
    printf("[main.c] Saindo do loop principal. Iniciando limpeza de objetos...\n");
    clearAllObjects(); // Libera toda a memória alocada para os objetos gráficos.
    printf("[main.c] Programa Rabisquim encerrado e memória liberada.\n");

    return 0; // Indica que o programa terminou com sucesso.
}
