// input.c
// Implementa toda a lógica de tratamento de eventos do usuário (teclado e mouse).
// Funciona como uma máquina de estados, onde o comportamento das funções de callback
// muda de acordo com o modo de operação atual (g_currentMode).

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
#include "file_io.h"
#include "animation.h"

// --- Definição das Variáveis Globais de Estado ---
// --- Defini��o das Vari�veis Globais de Estado ---
ProgramMode g_currentMode = MODE_SELECT;
int g_isDragging = 0;
int g_segmentClickCount = 0;
Point g_segmentP1;
Point g_polygonVertices[MAX_POLYGON_VERTICES];
int g_polygonVertexCount = 0;
Point g_lastMousePos = {0.0f, 0.0f};
Point g_currentMousePos = {0.0f, 0.0f};

// --- Função para Debug dos Modos ---
static const char* getModeString(ProgramMode mode) {
    switch (mode) {
        case MODE_SELECT: return "MODE_SELECT";
        case MODE_CREATE_POINT: return "MODE_CREATE_POINT";
        case MODE_CREATE_SEGMENT: return "MODE_CREATE_SEGMENT";
        case MODE_CREATE_POLYGON: return "MODE_CREATE_POLYGON";
        case MODE_TRANSLATE: return "MODE_TRANSLATE";
        case MODE_ROTATE: return "MODE_ROTATE";
        case MODE_SCALE: return "MODE_SCALE";
        case MODE_SHEAR: return "MODE_SHEAR";
        case MODE_REFLECT: return "MODE_REFLECT";
        default: return "MODE_UNKNOWN";
    }
}

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

// Constantes para os "outcodes" do algoritmo de Cohen-Sutherland
#define INSIDE 0 // 0000
#define LEFT   1 // 0001
#define RIGHT  2 // 0010
#define BOTTOM 4 // 0100
#define TOP    8 // 1000

/**
 * @brief Calcula o "outcode" de 4 bits de um ponto em relação a uma janela retangular.
 */
static int computeOutCode(Point p, float xmin, float xmax, float ymin, float ymax) {
    int code = INSIDE;
    if (p.x < xmin) code |= LEFT;
    else if (p.x > xmax) code |= RIGHT;
    if (p.y < ymin) code |= BOTTOM;
    else if (p.y > ymax) code |= TOP;
    return code;
}

/**
 * @brief Verifica se um segmento de reta intercepta um retângulo.
 * Implementa a lógica do algoritmo de Cohen-Sutherland para seleção.
 */
static int segmentIntersectsRect(Segment s, float xmin, float xmax, float ymin, float ymax) {
    int outcode1 = computeOutCode(s.p1, xmin, xmax, ymin, ymax);
    int outcode2 = computeOutCode(s.p2, xmin, xmax, ymin, ymax);

    if (!(outcode1 | outcode2)) {
        // Trivialmente aceito: ambos os pontos estão dentro da caixa.
        return 1;
    }
    if (outcode1 & outcode2) {
        // Trivialmente rejeitado: ambos os pontos estão na mesma região externa.
        return 0;
    }

    // Para os demais casos, a linha PODE cruzar a caixa. Para um teste de
    // seleção, essa possibilidade já é suficiente para considerar uma seleção válida.
    return 1;
}


// --- Função para Seleção de Objetos (reutilizável) ---
static int selectObjectAtPoint(Point click_point) {
    int final_found_index = -1;

    // Prioridade 1: Procura pelo PONTO mais próximo usando Bounding Box para filtrar.
    int closest_point_index = -1;
    float min_sq_dist_point = -1.0f; // Usamos a distância ao quadrado para evitar sqrtf

    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_POINT) {
            Point* p_obj = (Point*)g_objects[i].data;

            // FASE 1: Filtro Rápido com a Caixa de Tolerância (Método da Professora)
            if (fabs(p_obj->x - click_point.x) <= CLICK_TOLERANCE &&
                fabs(p_obj->y - click_point.y) <= CLICK_TOLERANCE)
            {
                // FASE 2: Desempate Preciso com a menor distância (Sua Lógica)
                float dx = p_obj->x - click_point.x;
                float dy = p_obj->y - click_point.y;
                float sq_dist = dx * dx + dy * dy;

                if (closest_point_index == -1 || sq_dist < min_sq_dist_point) {
                    min_sq_dist_point = sq_dist;
                    closest_point_index = i;
                }
            }
        }
    }

    if (closest_point_index != -1) {
        final_found_index = closest_point_index;
    }

    // Prioridade 2: Se não achou um ponto, procura pelo SEGMENTO mais próximo.
    if (final_found_index == -1) {
        int closest_segment_index = -1;
        float min_dist_segment = CLICK_TOLERANCE;

        // Define a "caixa de tolerância" retangular ao redor do clique.
        float xmin = click_point.x - CLICK_TOLERANCE;
        float xmax = click_point.x + CLICK_TOLERANCE;
        float ymin = click_point.y - CLICK_TOLERANCE;
        float ymax = click_point.y + CLICK_TOLERANCE;

        // FASE 1: Filtro Rápido com Cohen-Sutherland (Método da Professora)
        for (int i = 0; i < g_numObjects; i++) {
            if (g_objects[i].type == OBJECT_TYPE_SEGMENT) {
                Segment* s_obj = (Segment*)g_objects[i].data;

                if (segmentIntersectsRect(*s_obj, xmin, xmax, ymin, ymax)) {
                    // FASE 2: Desempate Preciso com a menor distância (Sua Lógica)
                    float dist = distPointSegment(click_point, *s_obj);

                    if (dist < min_dist_segment) {
                        min_dist_segment = dist;
                        closest_segment_index = i;
                    }
                }
            }
        }

        if (closest_segment_index != -1) {
            final_found_index = closest_segment_index;
        }
    }

    // Prioridade 3: Polígonos (sem alterações, sua lógica já é perfeita)
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

    return final_found_index;
}

// --- Funções de Callback ---

void keyboardCallback(unsigned char key, int x, int y) {
    int objectIsSelected = (g_selectedObjectIndex != -1);
    ProgramMode oldMode = g_currentMode;

    switch (key) {
        // --- Cases para MUDANÇA DE MODO ---
        case 'p': case 'P': g_currentMode = MODE_CREATE_POINT; break;
        case 'l': case 'L': g_currentMode = MODE_CREATE_SEGMENT; break;
        case 'o': case 'O': g_currentMode = MODE_CREATE_POLYGON; break;
        case 's': case 'S': g_currentMode = MODE_SELECT; break;
        case 't': case 'T':
            if (objectIsSelected) g_currentMode = MODE_TRANSLATE;
            else printf("[DEBUG] Não é possível entrar no modo TRANSLATE: nenhum objeto selecionado!\n");
            break;
        case 'r': case 'R':
            if (objectIsSelected) g_currentMode = MODE_ROTATE;
            else printf("[DEBUG] Não é possível entrar no modo ROTATE: nenhum objeto selecionado!\n");
            break;
        case 'e': case 'E': // 'E' para Escala
            if (objectIsSelected) g_currentMode = MODE_SCALE;
            else printf("[DEBUG] Não é possível entrar no modo SCALE: nenhum objeto selecionado!\n");
            break;
        case 'h': case 'H': // 'H' para sHear (Cisalhamento)
            if (objectIsSelected) g_currentMode = MODE_SHEAR;
            else printf("[DEBUG] Não é possível entrar no modo SHEAR: nenhum objeto selecionado!\n");
            break;
        case 'm': case 'M': // 'M' de Mirror (Espelho/Reflexão)
            if (objectIsSelected) g_currentMode = MODE_REFLECT;
            else printf("[DEBUG] Não é possível entrar no modo REFLECT: nenhum objeto selecionado!\n");
            break;

        // --- AÇÕES DENTRO DE MODOS ESPECÍFICOS ---

        case 'x': case 'X':
            if (objectIsSelected && g_currentMode == MODE_REFLECT) {
                printf("[DEBUG] Reflexão em X aplicada ao objeto %d\n", g_selectedObjectIndex);
                reflectObject(g_selectedObjectIndex, 0); // Chama a função de transformação
                glutPostRedisplay();
            } else if (objectIsSelected) {
                printf("[AVISO] Para refletir, primeiro entre no modo de reflexão (pressione 'm').\n");
            } else {
                printf("[AVISO] Selecione um objeto para poder refletir.\n");
            }
            break;
        case 'y': case 'Y':
            if (objectIsSelected && g_currentMode == MODE_REFLECT) {
                printf("[DEBUG] Reflexão em Y aplicada ao objeto %d\n", g_selectedObjectIndex);
                reflectObject(g_selectedObjectIndex, 1); // Chama a função de transformação
                glutPostRedisplay();
            } else if (objectIsSelected) {
                printf("[AVISO] Para refletir, primeiro entre no modo de reflexão (pressione 'm').\n");
            } else {
                printf("[AVISO] Selecione um objeto para poder refletir.\n");
            }
            break;

        case '=':
            if (objectIsSelected && g_currentMode == MODE_SCALE) {
                // Aumenta o tamanho do objeto em 10%
                scaleObject(g_selectedObjectIndex, 1.1f, 1.1f);
                glutPostRedisplay(); // Essencial: Pede para a tela ser redesenhada!
            }
            break;
        case '-':
            if (objectIsSelected && g_currentMode == MODE_SCALE) {
                // Diminui o tamanho do objeto em 10%
                scaleObject(g_selectedObjectIndex, 0.9f, 0.9f);
                glutPostRedisplay(); // Essencial: Pede para a tela ser redesenhada!
            }
            break;

        case 127: // Tecla DELETE
            if (objectIsSelected) removeObject(g_selectedObjectIndex);
            break;
        case 27: // Tecla ESC
            clearAllObjects(); exit(0);
            break;
        case 'b': case 'B':
            anim_toggle_selected(); // liga/desliga bounce no selecionado
            break;
        case 'v': case 'V': // jogar na direção do mouse
            anim_kick_towards_mouse(600.0f); // velocidade inicial
            break;

        case 'g': case 'G': // alterna gravidade no selecionado
            if(g_selectedObjectIndex != -1){
                int i = g_selectedObjectIndex;
                g_anim[i].gravity = !g_anim[i].gravity;
                printf("[DEBUG] Gravity %s no objeto %d\n", g_anim[i].gravity?"ON":"OFF", i);
            }
            break;

        case '[':
            if(g_selectedObjectIndex != -1) { g_anim[g_selectedObjectIndex].vx *= 0.9f;
                                              g_anim[g_selectedObjectIndex].vy *= 0.9f; }
            break;

        case ']':
            if(g_selectedObjectIndex != -1) { g_anim[g_selectedObjectIndex].vx *= 1.1f;
                                              g_anim[g_selectedObjectIndex].vy *= 1.1f; }
            break;

    }

    if (g_currentMode != oldMode) {
        printf("[DEBUG] Modo alterado de %s para %s\n", getModeString(oldMode), getModeString(g_currentMode));
        g_segmentClickCount = 0;
        g_polygonVertexCount = 0;
        if (g_currentMode == MODE_SELECT || g_currentMode == MODE_CREATE_POINT ||
            g_currentMode == MODE_CREATE_SEGMENT || g_currentMode == MODE_CREATE_POLYGON) {
             g_selectedObjectIndex = -1;
             g_isDragging = 0;
        }
    }
    glutPostRedisplay();
}

void specialKeysCallback(int key, int x, int y) {
    if (key == GLUT_KEY_F5){
        saveSceneToFile("scene.txt");
        return;
    }
    if (key == GLUT_KEY_F9){
        loadSceneFromFile("scene.txt");
        glutPostRedisplay();
        return;
    }
    if (g_selectedObjectIndex != -1 && g_currentMode == MODE_ROTATE) {
        switch (key) {
            case GLUT_KEY_LEFT:
                printf("Ação: Rotação Esquerda\n");
                rotateObject(g_selectedObjectIndex, 20);
                break;
            case GLUT_KEY_RIGHT:
                printf("Ação: Rotação Direita\n");
                rotateObject(g_selectedObjectIndex, -20);
                break;
        }
        glutPostRedisplay();
    }
    if (g_currentMode == MODE_SHEAR) {
        float shearFactor = 0.1f; // Define o quanto o objeto será inclinado a cada tecla pressionada

        switch (key) {
            case GLUT_KEY_RIGHT: // Seta para a direita
                // Aplica cisalhamento positivo em X
                shearObject(g_selectedObjectIndex, shearFactor, 0.0f);
                printf("[DEBUG] Shear X+ aplicado ao objeto %d\n", g_selectedObjectIndex);
                break;
            case GLUT_KEY_LEFT: // Seta para a esquerda
                // Aplica cisalhamento negativo em X
                shearObject(g_selectedObjectIndex, -shearFactor, 0.0f);
                printf("[DEBUG] Shear X- aplicado ao objeto %d\n", g_selectedObjectIndex);
                break;
            case GLUT_KEY_UP: // Seta para cima
                // Aplica cisalhamento positivo em Y
                shearObject(g_selectedObjectIndex, 0.0f, shearFactor);
                printf("[DEBUG] Shear Y+ aplicado ao objeto %d\n", g_selectedObjectIndex);
                break;
            case GLUT_KEY_DOWN: // Seta para baixo
                // Aplica cisalhamento negativo em Y
                shearObject(g_selectedObjectIndex, 0.0f, -shearFactor);
                printf("[DEBUG] Shear Y- aplicado ao objeto %d\n", g_selectedObjectIndex);
                break;
        }
        glutPostRedisplay(); // Pede para redesenhar a tela para ver a mudança
    }
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
                printf("[DEBUG] Ponto criado e selecionado (índice %d)\n", g_selectedObjectIndex);
            }
        }
        else if (g_currentMode == MODE_CREATE_SEGMENT) {
            if (button == GLUT_LEFT_BUTTON) {
                g_segmentClickCount++;
                if (g_segmentClickCount == 1) {
                    g_segmentP1 = click_point;
                    printf("[DEBUG] Primeiro ponto do segmento definido\n");
                } else if (g_segmentClickCount == 2) {
                    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
                    *newSegment = createSegment(g_segmentP1, click_point);
                    addObject(OBJECT_TYPE_SEGMENT, newSegment);
                    g_selectedObjectIndex = g_numObjects - 1;
                    g_segmentClickCount = 0;
                    printf("[DEBUG] Segmento criado e selecionado (índice %d)\n", g_selectedObjectIndex);
                }
            }
        }
        else if (g_currentMode == MODE_CREATE_POLYGON) {
            if (button == GLUT_LEFT_BUTTON) {
                if (g_polygonVertexCount < MAX_POLYGON_VERTICES) {
                    g_polygonVertices[g_polygonVertexCount++] = click_point;
                    printf("[DEBUG] Vértice %d do polígono adicionado\n", g_polygonVertexCount);
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
                    printf("[DEBUG] Polígono criado e selecionado (índice %d) com %d vértices\n",
                           g_selectedObjectIndex, g_polygonVertexCount);
                }
                g_polygonVertexCount = 0;
            }
        }
        else if (g_currentMode == MODE_SELECT) {
            if (button == GLUT_LEFT_BUTTON) {
                int found_index = selectObjectAtPoint(click_point);
                g_selectedObjectIndex = found_index;

                if (g_selectedObjectIndex != -1) {
                    g_isDragging = 1;
                    g_lastMousePos = click_point;
                    printf("[DEBUG] Objeto selecionado (índice %d) - Drag iniciado\n", g_selectedObjectIndex);
                } else {
                    printf("[DEBUG] Nenhum objeto encontrado no clique\n");
                }
            }
        }
        //Permitir iniciar drag no modo TRANSLATE
        else if (g_currentMode == MODE_TRANSLATE) {
            if (button == GLUT_LEFT_BUTTON) {
                if (g_selectedObjectIndex != -1) {
                    // Se já tem um objeto selecionado, inicia o drag
                    g_isDragging = 1;
                    g_lastMousePos = click_point;
                    printf("[DEBUG] Drag de translação iniciado para objeto %d\n", g_selectedObjectIndex);
                } else {
                    // Se não tem objeto selecionado, tenta selecionar um
                    int found_index = selectObjectAtPoint(click_point);
                    if (found_index != -1) {
                        g_selectedObjectIndex = found_index;
                        g_isDragging = 1;
                        g_lastMousePos = click_point;
                        printf("[DEBUG] Objeto selecionado (índice %d) e drag de translação iniciado\n", g_selectedObjectIndex);
                    } else {
                        printf("[DEBUG] Nenhum objeto encontrado para transladar\n");
                    }
                }
            }
        }


        glutPostRedisplay();
    }
    else if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON && g_isDragging) {
            g_isDragging = 0;
            printf("[DEBUG] Drag finalizado\n");
        }
    }
}

void motionCallback(int x, int y) {
    if (g_isDragging && g_currentMode == MODE_TRANSLATE && g_selectedObjectIndex != -1) {
        float gl_y = WINDOW_HEIGHT - (float)y;
        Point currentMousePos = {(float)x, gl_y};

        float dx = currentMousePos.x - g_lastMousePos.x;
        float dy = currentMousePos.y - g_lastMousePos.y;

        printf("[DEBUG] Transladando objeto %d: dx=%.2f, dy=%.2f\n", g_selectedObjectIndex, dx, dy);

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
