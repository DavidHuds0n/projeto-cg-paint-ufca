// input.c
// Implementa toda a lógica de tratamento de eventos do usuário (teclado e mouse).
// Funciona como uma máquina de estados, onde o comportamento das funções de callback
// muda de acordo com o modo de operaçãdadao atual (g_currentMode).

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

// --- Definição das Variáveis Globais de Estado ---
ProgramMode g_currentMode = MODE_CREATE_POINT;
int g_isDragging = 0;
int g_segmentClickCount = 0;
Point g_segmentP1;
Point g_polygonVertices[MAX_POLYGON_VERTICES];
int g_polygonVertexCount = 0;
Point g_lastMousePos = {0.0f, 0.0f};
Point g_currentMousePos = {0.0f, 0.0f};

// --- Funções Auxiliares para Seleção ---

/**
 * @brief Verifica se um ponto (p) está dentro de um polígono (poly) usando o Algoritmo do Tiro.
 */
static int selectPolygon(GfxPolygon* poly, Point p) {
    int crossings = 0;
    for (int i = 0; i < poly->numVertices; i++) {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->numVertices];

        if(((p1.y > p.y) && (p2.y > p.y))){
            continue;
        }
        if(((p1.y < p.y) && (p2.y < p.y))){
            continue;
        }
        if(((p1.x < p.x) && (p2.x < p.x))){
            continue;
        }
        if(((p1.x > p.x && p2.x > p.x) && ((p1.y > p.y && p2.y < p.y) || (p1.y < p.y && p2.y > p.y)))){
            crossings++;
            continue;
        }
        if(((p1.y == p2.y) && (p1.y == p.y))){
            continue;
        }

        float xi = p1.x + (p.y - p1.y)*(p2.x - p1.x) / (p2.y - p1.y);

        if(xi> p.x){
            crossings++;
        }

        /*if (((p1.y <= p.y && p.y < p2.y) || (p2.y <= p.y && p.y < p1.y)) &&
            (p.x < (p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y) + p1.x)) {
            crossings++;
        }*/
    }
    return crossings % 2; // Ímpar = dentro, Par = fora.
}

/**
 * @brief Calcula a menor distância de um ponto às arestas de um polígono.
 */
static float distPointToPolygonEdges(GfxPolygon* poly, Point click_point) {
    if (poly->numVertices < 2) return -1.0f;
    float min_dist = -1.0f;
    for (int i = 0; i < poly->numVertices; i++) {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->numVertices];
        Segment edge = {p1, p2};
        float dist = distPointSegment(click_point, edge);
        if (min_dist == -1.0f || dist < min_dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}


// --- Funções de Callback ---

void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 'p': case 'P':
            g_currentMode = MODE_CREATE_POINT;
            g_selectedObjectIndex = -1; g_segmentClickCount = 0; g_polygonVertexCount = 0;
            break;
        case 's': case 'S':
            g_currentMode = MODE_SELECT;
            g_selectedObjectIndex = -1; g_segmentClickCount = 0; g_polygonVertexCount = 0;
            break;
        case 'l': case 'L':
            g_currentMode = MODE_CREATE_SEGMENT;
            g_selectedObjectIndex = -1; g_segmentClickCount = 0; g_polygonVertexCount = 0;
            break;
        case 'o': case 'O':
            g_currentMode = MODE_CREATE_POLYGON;
            g_selectedObjectIndex = -1; g_segmentClickCount = 0; g_polygonVertexCount = 0;
            break;
        case 127: // Tecla DELETE
            if (g_selectedObjectIndex != -1) removeObject(g_selectedObjectIndex);
            break;
        case 27: // Tecla ESC
            clearAllObjects();
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
    float gl_y = WINDOW_HEIGHT - (float)y;
    Point click_point = { (float)x, gl_y };

    if (state == GLUT_DOWN) {
        if (g_currentMode == MODE_CREATE_POINT) {
            if (button == GLUT_LEFT_BUTTON) {
                Point* newPoint = (Point*)malloc(sizeof(Point));
                *newPoint = createPoint(click_point.x, click_point.y);
                addObject(OBJECT_TYPE_POINT, newPoint);
                g_selectedObjectIndex = g_numObjects - 1;
            }
        }
        else if (g_currentMode == MODE_CREATE_SEGMENT) {
            if (button == GLUT_LEFT_BUTTON) {
                g_segmentClickCount++;
                if (g_segmentClickCount == 1) {
                    g_segmentP1 = click_point;
                } else if (g_segmentClickCount == 2) {
                    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
                    *newSegment = createSegment(g_segmentP1, click_point);
                    addObject(OBJECT_TYPE_SEGMENT, newSegment);
                    g_selectedObjectIndex = g_numObjects - 1;
                    g_segmentClickCount = 0;
                }
            }
        }
        else if (g_currentMode == MODE_CREATE_POLYGON) {
            if (button == GLUT_LEFT_BUTTON) {
                if (g_polygonVertexCount < MAX_POLYGON_VERTICES) {
                    g_polygonVertices[g_polygonVertexCount++] = click_point;
                }
            } else if (button == GLUT_RIGHT_BUTTON) {
                if (g_polygonVertexCount >= 3) {
                    GfxPolygon* newPolygon = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                    *newPolygon = createPolygon();
                    for (int i = 0; i < g_polygonVertexCount; i++) {
                        addVertexToPolygon(newPolygon, g_polygonVertices[i]);
                    }
                    addObject(OBJECT_TYPE_POLYGON, newPolygon);
                    g_selectedObjectIndex = g_numObjects - 1;
                }
                g_polygonVertexCount = 0;
            }
        }
        else if (g_currentMode == MODE_SELECT) {
            if (button == GLUT_LEFT_BUTTON) {
                int final_found_index = -1;

                // --- LÓGICA DE SELEÇÃO COM PRIORIDADE ---

                // Prioridade 1: Procura pelo PONTO mais próximo.
                int closest_point_index = -1;
                float min_point_dist = CLICK_TOLERANCE;
                for (int i = 0; i < g_numObjects; i++) {
                    if (g_objects[i].type == OBJECT_TYPE_POINT) {
                        Point* p_obj = (Point*)g_objects[i].data;
                        float dx = click_point.x - p_obj->x;
                        float dy = click_point.y - p_obj->y;
                        float dist = sqrtf((dx * dx) + (dy * dy));
                        if (dist < min_point_dist) {
                            min_point_dist = dist;
                            closest_point_index = i;
                        }
                    }
                }
                if (closest_point_index != -1) {
                    final_found_index = closest_point_index;
                }

                // Prioridade 2: Se não achou um ponto, procura pelo SEGMENTO mais próximo.
                if (final_found_index == -1) {
                    int closest_segment_index = -1;
                    float min_segment_dist = CLICK_TOLERANCE;
                    for (int i = 0; i < g_numObjects; i++) {
                        if (g_objects[i].type == OBJECT_TYPE_SEGMENT) {
                            Segment* s_obj = (Segment*)g_objects[i].data;
                            float dist = distPointSegment(click_point, *s_obj);
                            if (dist < min_segment_dist) {
                                min_segment_dist = dist;
                                closest_segment_index = i;
                            }
                        }
                    }
                    if (closest_segment_index != -1) {
                        final_found_index = closest_segment_index;
                    }
                }

                // Prioridade 3: Se não achou nada ainda, procura por POLÍGONOS.
                if (final_found_index == -1) {
                    int closest_poly_index = -1;
                    float min_poly_dist = -1.0f;

                    for (int i = 0; i < g_numObjects; i++) {
                        if (g_objects[i].type == OBJECT_TYPE_POLYGON) {
                            GfxPolygon* poly_obj = (GfxPolygon*)g_objects[i].data;
                            if (selectPolygon(poly_obj, click_point)) {
                                float dist_to_edge = distPointToPolygonEdges(poly_obj, click_point);
                                if (closest_poly_index == -1 || dist_to_edge < min_poly_dist) {
                                    min_poly_dist = dist_to_edge;
                                    closest_poly_index = i;
                                }
                            }
                        }
                    }
                    if (closest_poly_index != -1) {
                        final_found_index = closest_poly_index;
                    }
                }

                g_selectedObjectIndex = final_found_index;
                if (g_selectedObjectIndex != -1) {
                    g_isDragging = 1;
                    g_lastMousePos = click_point;
                }
            }
        }
        glutPostRedisplay();
    }
    else if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            g_isDragging = 0;
        }
    }
}

void motionCallback(int x, int y) {
    if (g_isDragging && g_currentMode == MODE_SELECT && g_selectedObjectIndex != -1) {
        float gl_y = WINDOW_HEIGHT - (float)y;
        Point currentMousePos = {(float)x, gl_y};
        float dx = currentMousePos.x - g_lastMousePos.x;
        float dy = currentMousePos.y - g_lastMousePos.y;
        translateObject(g_selectedObjectIndex, dx, dy);
        g_lastMousePos = currentMousePos;
        glutPostRedisplay();
    }
}

void passiveMotionCallback(int x, int y) {
    g_currentMousePos.x = (float)x;
    g_currentMousePos.y = WINDOW_HEIGHT - (float)y;
    if (g_currentMode == MODE_CREATE_SEGMENT && g_segmentClickCount == 1) {
        glutPostRedisplay();
    }
}
