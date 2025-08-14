// input.c
// Implementa��o das fun��es para o gerenciamento de entrada do usu�rio.

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "input.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "transformations.h" // NOVO: Vamos precisar de fun��es de transforma��o aqui!

// Defini��o das vari�veis globais (declaradas em input.h)
ProgramMode g_currentMode = MODE_CREATE_POINT;
int g_segmentClickCount = 0;
Point g_segmentP1;

Point g_polygonVertices[MAX_POLYGON_VERTICES];
int g_polygonVertexCount = 0;

// NOVO: Inicializa��o das vari�veis para o Arrastar e Soltar
int g_isDragging = 0; // 0 = false, 1 = true
Point g_lastMousePos = {0.0f, 0.0f};


// Fun��o auxiliar para verificar se um ponto est� perto de alguma aresta de um pol�gono
float distPointToPolygonEdges(Point click_point, GfxPolygon* poly) {
    if (poly->numVertices < 2) return -1.0f;

    float min_dist = -1.0f;

    for (int i = 0; i < poly->numVertices; i++) {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->numVertices];

        Segment edge = createSegment(p1, p2);
        float dist = distPointSegment(click_point, edge);

        if (min_dist == -1.0f || dist < min_dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}

// CORRE��O PARA A FUN��O keyboardCallback em input.c

void keyboardCallback(unsigned char key, int x, int y) {
    printf("[Input] Tecla pressionada: %c (ASCII: %d) em (%d, %d)\n", key, key, x, y);

    // A l�gica de resetar o estado foi movida para dentro do switch

    switch (key) {
        case 'p':
        case 'P':
            g_currentMode = MODE_CREATE_POINT;
            g_selectedObjectIndex = -1; // Reseta a sele��o ao mudar de modo
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Pontos.\n");
            break;

        case 's':
        case 'S':
            g_currentMode = MODE_SELECT;
            g_selectedObjectIndex = -1; // Reseta a sele��o ao entrar no modo de sele��o
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Sele��o.\n");
            break;

        case 'l':
        case 'L':
            g_currentMode = MODE_CREATE_SEGMENT;
            g_selectedObjectIndex = -1; // Reseta a sele��o ao mudar de modo
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Segmentos (Linhas).\n");
            break;

        case 'o':
        case 'O':
            g_currentMode = MODE_CREATE_POLYGON;
            g_selectedObjectIndex = -1; // Reseta a sele��o ao mudar de modo
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Pol�gonos.\n");
            break;

        case 127: // Tecla DELETE (ASCII 127)
            // Agora a verifica��o funciona, pois a sele��o n�o foi resetada.
            if (g_selectedObjectIndex != -1) {
                printf("[Input] Deletando objeto selecionado no �ndice: %d.\n", g_selectedObjectIndex);
                removeObject(g_selectedObjectIndex);
            } else {
                printf("[Input] NENHUM objeto selecionado para deletar.\n");
            }
            break;

        case 27: // Tecla ESC
            printf("[Input] Saindo do programa...\n");
            clearAllObjects(); // Boa pr�tica: limpar a mem�ria antes de sair
            exit(0);
            break;
    }

    // Solicita o redesenho da tela para refletir qualquer mudan�a (ex: exclus�o)
    glutPostRedisplay();
}

// Callback para eventos de mouse
void mouseCallback(int button, int state, int x, int y) {
    float gl_y = WINDOW_HEIGHT - (float)y; // Converte Y do mouse para coordenadas OpenGL
    Point click_point = { (float)x, gl_y }; // Cria um Point para o clique

    if (state == GLUT_DOWN) { // Bot�o pressionado
        // --- MODO DE CRIA��O DE PONTOS ---
        if (g_currentMode == MODE_CREATE_POINT) {
            if (button == GLUT_LEFT_BUTTON) {
                printf("[Input] Clique para criar ponto em (%.1f, %.1f).\n", click_point.x, click_point.y);
                Point* newPoint = (Point*)malloc(sizeof(Point));
                if (newPoint == NULL) { fprintf(stderr, "[Input ERROR] Falha ao alocar mem�ria para novo ponto.\n"); return; }
                *newPoint = createPoint(click_point.x, click_point.y);
                addObject(OBJECT_TYPE_POINT, newPoint);
                g_selectedObjectIndex = g_numObjects - 1;
                glutPostRedisplay();
            }

        // --- MODO DE CRIA��O DE SEGMENTOS ---
        } else if (g_currentMode == MODE_CREATE_SEGMENT) {
            if (button == GLUT_LEFT_BUTTON) {
                g_segmentClickCount++;
                printf("[Input] Clique para criar segmento (clique %d) em (%.1f, %.1f).\n", g_segmentClickCount, click_point.x, click_point.y);
                if (g_segmentClickCount == 1) {
                    g_segmentP1 = createPoint(click_point.x, click_point.y);
                } else if (g_segmentClickCount == 2) {
                    Point segP2 = createPoint(click_point.x, click_point.y);
                    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
                    if (newSegment == NULL) { fprintf(stderr, "[Input ERROR] Falha ao alocar mem�ria para novo segmento.\n"); return; }
                    *newSegment = createSegment(g_segmentP1, segP2);
                    addObject(OBJECT_TYPE_SEGMENT, newSegment);
                    g_selectedObjectIndex = g_numObjects - 1;
                    g_segmentClickCount = 0;
                    glutPostRedisplay();
                }
            } else if (button == GLUT_RIGHT_BUTTON) {
                g_segmentClickCount = 0;
                printf("[Input] Cria��o de segmento resetada (clique direito).\n");
            }

        // --- MODO DE CRIA��O DE POL�GONOS ---
        } else if (g_currentMode == MODE_CREATE_POLYGON) {
            if (button == GLUT_LEFT_BUTTON) {
                if (g_polygonVertexCount < MAX_POLYGON_VERTICES) {
                    g_polygonVertices[g_polygonVertexCount] = click_point;
                    g_polygonVertexCount++;
                    printf("[Input] V�rtice adicionado ao pol�gono em (%.1f, %.1f). Total de v�rtices para o pol�gono atual: %d.\n", click_point.x, click_point.y, g_polygonVertexCount);
                    glutPostRedisplay();
                } else {
                    fprintf(stderr, "[Input ERROR] Limite m�ximo de v�rtices (%d) para o pol�gono atual atingido.\n", MAX_POLYGON_VERTICES);
                }
            } else if (button == GLUT_RIGHT_BUTTON) {
                if (g_polygonVertexCount >= 3) {
                    GfxPolygon* newPolygon = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                    if (newPolygon == NULL) { fprintf(stderr, "[Input ERROR] Falha ao alocar mem�ria para novo pol�gono.\n"); return; }
                    *newPolygon = createPolygon();
                    for (int i = 0; i < g_polygonVertexCount; i++) {
                        addVertexToPolygon(newPolygon, g_polygonVertices[i]);
                    }
                    addObject(OBJECT_TYPE_POLYGON, newPolygon);
                    g_selectedObjectIndex = g_numObjects - 1;
                    g_polygonVertexCount = 0;
                    glutPostRedisplay();
                } else {
                    printf("[Input] Para finalizar um pol�gono, s�o necess�rios pelo menos 3 v�rtices. V�rtices atuais: %d. Resetando a cria��o do pol�gono.\n", g_polygonVertexCount);
                    g_polygonVertexCount = 0;
                }
            }

        // --- MODO DE SELE��O ---
        } else if (g_currentMode == MODE_SELECT) {
            if (button == GLUT_LEFT_BUTTON) {
                printf("[Input] Clique do mouse para sele��o em (%.1f, %.1f).\n", click_point.x, click_point.y);

                int found_index = -1;
                float min_distance_found = CLICK_TOLERANCE + 1.0f;

                for (int i = 0; i < g_numObjects; i++) {
                    float current_distance = -1.0f;

                    if (g_objects[i].type == OBJECT_TYPE_POINT) {
                        Point* p_obj = (Point*)g_objects[i].data;
                        float dx = click_point.x - p_obj->x;
                        float dy = click_point.y - p_obj->y;
                        current_distance = sqrtf((dx * dx) + (dy * dy));

                    } else if (g_objects[i].type == OBJECT_TYPE_SEGMENT) {
                        Segment* s_obj = (Segment*)g_objects[i].data;
                        current_distance = distPointSegment(click_point, *s_obj);
                    } else if (g_objects[i].type == OBJECT_TYPE_POLYGON) {
                        GfxPolygon* poly_obj = (GfxPolygon*)g_objects[i].data;
                        current_distance = distPointToPolygonEdges(click_point, poly_obj);
                    }

                    if (current_distance != -1.0f && current_distance <= CLICK_TOLERANCE) {
                        if (found_index == -1 || current_distance < min_distance_found) {
                            found_index = i;
                            min_distance_found = current_distance;
                        }
                    }
                }

                g_selectedObjectIndex = found_index;

                if (g_selectedObjectIndex != -1) {
                    printf("[Input] Objeto selecionado no �ndice: %d (Tipo: %d).\n", g_selectedObjectIndex, g_objects[g_selectedObjectIndex].type);
                    // NOVO: Inicia o arraste se um objeto foi selecionado
                    g_isDragging = 1; // Ativa a flag de arraste
                    g_lastMousePos = click_point; // Registra a posi��o inicial do mouse
                } else {
                    printf("[Input] Nenhum objeto selecionado.\n");
                    g_isDragging = 0; // Desativa o arraste se nada foi selecionado
                }

                glutPostRedisplay();
            } else if (button == GLUT_RIGHT_BUTTON) {
                // Desseleciona ao clicar com o bot�o direito no modo sele��o
                if (g_selectedObjectIndex != -1) {
                    printf("[Input] Objeto no �ndice %d desselecionado.\n", g_selectedObjectIndex);
                    g_selectedObjectIndex = -1;
                    glutPostRedisplay();
                }
                g_isDragging = 0; // Para qualquer arraste ativo
            }
        }
    } else if (state == GLUT_UP) { // Bot�o liberado
        if (button == GLUT_LEFT_BUTTON) {
            if (g_isDragging) {
                printf("[Input] Arraste do objeto finalizado.\n");
            }
            g_isDragging = 0; // Desativa a flag de arraste ao liberar o bot�o esquerdo
        }
    }
}

// NOVO: Callback para eventos de movimento do mouse enquanto um bot�o est� pressionado
void motionCallback(int x, int y) {
    if (g_isDragging && g_currentMode == MODE_SELECT && g_selectedObjectIndex != -1) {
        float gl_y = WINDOW_HEIGHT - (float)y;
        Point currentMousePos = {(float)x, gl_y};

        float dx = currentMousePos.x - g_lastMousePos.x;
        float dy = currentMousePos.y - g_lastMousePos.y;

        // Chama a fun��o de transla��o para o objeto selecionado
        // Precisaremos criar a fun��o 'translateObject' no m�dulo 'transformations'
        translateObject(g_selectedObjectIndex, dx, dy);
        printf("[Input] Arrastando objeto %d por (%.1f, %.1f). Nova pos mouse (%.1f, %.1f).\n", g_selectedObjectIndex, dx, dy, currentMousePos.x, currentMousePos.y);

        g_lastMousePos = currentMousePos; // Atualiza a �ltima posi��o do mouse
        glutPostRedisplay(); // Solicita redesenho para ver o objeto se movendo
    }
}
