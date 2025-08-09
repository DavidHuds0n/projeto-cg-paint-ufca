// main.c
// Gerencia a inicializa��o do programa, configura��o do OpenGL e o loop principal do GLUT.

// Bibliotecas padr�o do C e da GLUT
#include <GL/glut.h>   // Biblioteca GLUT para gerenciamento de janelas e eventos.
#include <stdio.h>     // Para fun��es de entrada/sa�da (printf, fprintf).
#include <stdlib.h>    // Para fun��es de aloca��o de mem�ria (malloc, free).

// Inclui as configura��es globais do projeto (altura, largura da janela, etc.)
#include "config.h"    // Inclui as defini��es centralizadas

// Cabe�alhos dos m�dulos do projeto
#include "render.h"    // Fun��es de desenho.
#include "input.h"     // Gerenciamento de entrada do usu�rio (teclado e mouse).
#include "objects.h"   // Gerenciamento gen�rico de objetos gr�ficos.
#include "point.h"     // Defini��es e fun��es para objetos Point.
#include "segment.h"   // Defini��es e fun��es para objetos Segment.
#include "polygon.h"   // Defini��es e fun��es para objetos GfxPolygon.
#include "transformations.h" // NOVO: Incluir o cabe�alho de transforma��es

// --- Fun��es de Callback do GLUT ---

// displayCallback: Fun��o chamada pelo GLUT para redesenhar o conte�do da janela.
void displayCallback() {
    // Limpa o buffer de cores com a cor de fundo definida.
    glClear(GL_COLOR_BUFFER_BIT);

    // Chama a fun��o que desenha todos os objetos gr�ficos gerenciados pelo programa.
    renderAllObjects();

    // IMPORTANTE: glutSwapBuffers() � essencial para exibir o que foi desenhado
    // quando o modo de exibi��o � GLUT_DOUBLE (buffer duplo).
    glutSwapBuffers();
}

// initOpenGL: Configura o ambiente inicial do OpenGL para renderiza��o 2D.
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Define a cor de fundo da janela como branco (RGBA).

    glMatrixMode(GL_PROJECTION);      // Define o modo da matriz atual para proje��o.
    glLoadIdentity();                 // Carrega a matriz identidade, redefinindo a proje��o.
    // Configura um sistema de coordenadas 2D ortogonal usando as constantes de config.h
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

// --- Fun��o Principal do Programa ---
int main(int argc, char **argv) {
    // 1. Inicializa��o do GLUT: Configura a biblioteca e a janela da aplica��o.
    glutInit(&argc, argv);                             // Inicializa a biblioteca GLUT.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);       // Define o modo de exibi��o: double buffer para evitar cintila��o, e cores RGB.
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);   // Define as dimens�es iniciais da janela usando config.h.
    glutCreateWindow(WINDOW_TITLE);                    // Cria a janela principal com o t�tulo especificado usando config.h.
    printf("[main.c] Janela '%s' criada com sucesso!\n", WINDOW_TITLE);

    // 2. Configura��es Iniciais do Projeto: Prepara o ambiente OpenGL e o gerenciador de objetos.
    initOpenGL();      // Aplica as configura��es iniciais do OpenGL.
    initObjectList();  // Inicializa a lista global onde os objetos gr�ficos ser�o armazenados e gerenciados.
    printf("[main.c] OpenGL e lista de objetos inicializados.\n");

    // 3. Adi��o de Objetos de Teste: Cria e adiciona um ponto e um segmento para demonstra��o inicial.

    // Ponto de Teste: Posicionado no centro da janela.
    Point* testPoint = (Point*)malloc(sizeof(Point));
    if (testPoint == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na aloca��o de mem�ria para o ponto de teste!\n");
        return 1; // Retorna com erro se a aloca��o falhar.
    }
    *testPoint = createPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // Usa WINDOW_WIDTH/HEIGHT de config.h.
    addObject(OBJECT_TYPE_POINT, testPoint); // Adiciona o ponto � lista de objetos.
    printf("[main.c] Ponto de teste adicionado.\n");

    // Segmento de Teste: Desenhado de (100,100) a (300,300).
    Segment* testSegment = (Segment*)malloc(sizeof(Segment));
    if (testSegment == NULL) {
        fprintf(stderr, "[main.c ERROR] Falha na aloca��o de mem�ria para o segmento de teste!\n");
        free(testPoint); // Libera o ponto de teste se a aloca��o do segmento falhar.
        return 1; // Retorna com erro.
    }
    // Define os dois pontos que comp�em o segmento.
    Point segP1 = createPoint(100.0f, 100.0f);
    Point segP2 = createPoint(300.0f, 300.0f);
    *testSegment = createSegment(segP1, segP2); // Cria o segmento.
    addObject(OBJECT_TYPE_SEGMENT, testSegment); // Adiciona o segmento � lista de objetos.
    printf("[main.c] Segmento de teste adicionado.\n");

    // Seleciona o �ltimo objeto adicionado (o segmento de teste) para que ele seja exibido como selecionado ao iniciar.
    g_selectedObjectIndex = g_numObjects - 1;
    printf("[main.c] Total de objetos: %d. Objeto selecionado inicial: %d.\n", g_numObjects, g_selectedObjectIndex);

    // 4. Registro de Callbacks do GLUT: Conecta as fun��es de manipula��o de eventos do programa ao GLUT.
    glutDisplayFunc(displayCallback);     // Registra a fun��o de desenho principal.
    glutKeyboardFunc(keyboardCallback);   // Registra a fun��o para lidar com eventos de teclado.
    glutMouseFunc(mouseCallback);         // Registra a fun��o para lidar com eventos do mouse.
    glutMotionFunc(motionCallback);       // NOVO: Registra a fun��o para lidar com o movimento do mouse enquanto um bot�o est� pressionado.

    // 5. Loop Principal do GLUT: Inicia o processamento de eventos do sistema operacional.
    printf("\n--- Rabisquim - Instru��es ---\n");
    printf("Pressione 'P': Modo Criar Pontos (clique para adicionar um ponto)\n");
    printf("Pressione 'L': Modo Criar Segmentos (dois cliques para definir um segmento)\n");
    printf("Pressione 'O': Modo Criar Pol�gonos (clique ESQUERDO para adicionar v�rtices, clique DIREITO para finalizar)\n");
    printf("Pressione 'S': Modo Sele��o (clique em pontos, segmentos ou pol�gonos para selecion�-los e arrastar)\n");
    printf("Pressione 'DEL': Excluir objeto selecionado\n");
    printf("Pressione 'ESC': Sair do programa\n");
    printf("------------------------------\n");
    printf("[main.c] Entrando no loop principal do GLUT. Feche a janela para sair.\n");
    glutMainLoop();

    // 6. Limpeza de Recursos: Esta se��o � executada quando o loop principal do GLUT termina (ex: janela fechada).
    printf("[main.c] Saindo do loop principal. Iniciando limpeza de objetos...\n");
    clearAllObjects(); // Libera toda a mem�ria alocada para os objetos gr�ficos.
    printf("[main.c] Programa Rabisquim encerrado e mem�ria liberada.\n");

    return 0; // Indica que o programa terminou com sucesso.
}
