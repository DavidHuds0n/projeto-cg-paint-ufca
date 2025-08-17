// input.c
// Implementa toda a l�gica de tratamento de eventos do usu�rio (teclado e mouse).
// Funciona como uma m�quina de estados, onde o comportamento das fun��es de callback
// muda de acordo com o modo de opera��o atual (g_currentMode).

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
#include "transformations.h"


// --- Defini��o das Vari�veis Globais de Estado ---
ProgramMode g_currentMode = MODE_CREATE_POINT;
int g_isDragging = 0;

int g_segmentClickCount = 0;
Point g_segmentP1;

Point g_polygonVertices[MAX_POLYGON_VERTICES];
int g_polygonVertexCount = 0;

Point g_lastMousePos = {0.0f, 0.0f};


// --- Fun��es Auxiliares ---

// ATEN��O: Esta fun��o ser� substitu�da pela implementa��o do Algoritmo do Tiro.
float distPointToPolygonEdges(Point click_point, GfxPolygon* poly) {
    if (poly->numVertices < 2) return -1.0f;
    float min_dist = -1.0f;

    for (int i = 0; i < poly->numVertices; i++) {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->numVertices]; // Garante que a �ltima aresta conecte ao primeiro v�rtice.
        Segment edge = createSegment(p1, p2);
        float dist = distPointSegment(click_point, edge);
        if (min_dist == -1.0f || dist < min_dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}


// --- Fun��es de Callback ---

void keyboardCallback(unsigned char key, int x, int y) {
    // O switch principal que controla a mudan�a de estado do programa.
    switch (key) {
        // Cases para mudan�a de modo. Resetam o estado para evitar comportamentos inesperados.
        case 'p': case 'P':
            g_currentMode = MODE_CREATE_POINT;
            g_selectedObjectIndex = -1;
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Pontos.\n");
            break;

        case 's': case 'S':
            g_currentMode = MODE_SELECT;
            g_selectedObjectIndex = -1;
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Sele��o.\n");
            break;

        case 'l': case 'L':
            g_currentMode = MODE_CREATE_SEGMENT;
            g_selectedObjectIndex = -1;
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Segmentos.\n");
            break;

        case 'o': case 'O':
            g_currentMode = MODE_CREATE_POLYGON;
            g_selectedObjectIndex = -1;
            g_segmentClickCount = 0;
            g_polygonVertexCount = 0;
            printf("[Input] Modo alterado para: Criar Pol�gonos.\n");
            break;

        // Cases para a��es que N�O mudam o modo.
        case 127: // Tecla DELETE
            if (g_selectedObjectIndex != -1) {
                removeObject(g_selectedObjectIndex);
            }
            break;

        case 27: // Tecla ESC
            clearAllObjects();
            exit(0);
            break;
    }
    // For�a o redesenho da tela para refletir qualquer mudan�a (ex: objeto deletado, mudan�a de sele��o).
    glutPostRedisplay();
}

// Callback para eventos de clique do mouse.
void mouseCallback(int button, int state, int x, int y) {
    // Converte a coordenada Y do sistema de janelas (origem no topo esquerdo)
    // para o sistema de coordenadas do OpenGL (origem no canto inferior esquerdo).
    float gl_y = WINDOW_HEIGHT - (float)y;
    Point click_point = { (float)x, gl_y };

    // A��es s�o executadas apenas quando o bot�o � pressionado (GLUT_DOWN).
    if (state == GLUT_DOWN) {
        // --- M�QUINA DE ESTADOS BASEADA NO MODO ATUAL ---

        if (g_currentMode == MODE_CREATE_POINT) {
            if (button == GLUT_LEFT_BUTTON) {
                Point* newPoint = (Point*)malloc(sizeof(Point));
                *newPoint = createPoint(click_point.x, click_point.y);
                addObject(OBJECT_TYPE_POINT, newPoint);
                g_selectedObjectIndex = g_numObjects - 1; // LINHA CORRIGIDA: Seleciona o objeto rec�m-criado.
            }
        }
        else if (g_currentMode == MODE_CREATE_SEGMENT) {
            if (button == GLUT_LEFT_BUTTON) {
                g_segmentClickCount++;
                if (g_segmentClickCount == 1) {
                    // Primeiro clique: armazena o ponto inicial.
                    g_segmentP1 = click_point;
                } else if (g_segmentClickCount == 2) {
                    // Segundo clique: cria o segmento e reseta o contador.
                    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
                    *newSegment = createSegment(g_segmentP1, click_point);
                    addObject(OBJECT_TYPE_SEGMENT, newSegment);
                    g_selectedObjectIndex = g_numObjects - 1; // LINHA CORRIGIDA: Seleciona o objeto rec�m-criado.
                    g_segmentClickCount = 0;
                }
            }
        }
        else if (g_currentMode == MODE_CREATE_POLYGON) {
            if (button == GLUT_LEFT_BUTTON) {
                // Adiciona v�rtices ao buffer tempor�rio at� o limite.
                if (g_polygonVertexCount < MAX_POLYGON_VERTICES) {
                    g_polygonVertices[g_polygonVertexCount] = click_point;
                    g_polygonVertexCount++;
                }
            } else if (button == GLUT_RIGHT_BUTTON) {
                // Clique direito finaliza o pol�gono se ele tiver v�rtices suficientes.
                if (g_polygonVertexCount >= 3) {
                    GfxPolygon* newPolygon = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                    *newPolygon = createPolygon();
                    for (int i = 0; i < g_polygonVertexCount; i++) {
                        addVertexToPolygon(newPolygon, g_polygonVertices[i]);
                    }
                    addObject(OBJECT_TYPE_POLYGON, newPolygon);
                    g_selectedObjectIndex = g_numObjects - 1; // LINHA CORRIGIDA: Seleciona o objeto rec�m-criado.
                }
                // Limpa o buffer de v�rtices para o pr�ximo pol�gono.
                g_polygonVertexCount = 0;
            }
        }
        else if (g_currentMode == MODE_SELECT) {
            if (button == GLUT_LEFT_BUTTON) {
                // // TODO: Esta se��o deve ser refatorada para usar os algoritmos da aula.

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
                    g_isDragging = 1;
                    g_lastMousePos = click_point;
                }
            }
        }
        // Solicita redesenho ao final de qualquer clique que possa alterar a cena.
        glutPostRedisplay();
    }
    // A��es para quando o bot�o � liberado (GLUT_UP).
    else if (state == GLUT_UP) {
        // Para a opera��o de arrastar ao soltar o bot�o esquerdo.
        if (button == GLUT_LEFT_BUTTON) {
            g_isDragging = 0;
        }
    }
}

void motionCallback(int x, int y) {
    // Esta fun��o s� executa a��es se a flag g_isDragging estiver ativa.
    if (g_isDragging && g_currentMode == MODE_SELECT && g_selectedObjectIndex != -1) {
        float gl_y = WINDOW_HEIGHT - (float)y;
        Point currentMousePos = {(float)x, gl_y};

        // Calcula o deslocamento (delta) do mouse desde o �ltimo frame.
        float dx = currentMousePos.x - g_lastMousePos.x;
        float dy = currentMousePos.y - g_lastMousePos.y;

        // Aplica a transla��o com base nesse deslocamento.
        translateObject(g_selectedObjectIndex, dx, dy);

        // Atualiza a �ltima posi��o do mouse para o pr�ximo c�lculo de delta.
        g_lastMousePos = currentMousePos;
        glutPostRedisplay();
    }
}
