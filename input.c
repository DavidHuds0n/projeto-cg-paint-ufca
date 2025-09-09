/**
 * @file input.c
 * @brief Gerencia a lógica de tratamento de eventos do usuário (teclado e mouse).
 *
 * Este arquivo atua como uma máquina de estados, onde o comportamento das
 * funções de callback muda dinamicamente com base no modo de operação atual.
 */

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
#include "convexhull.h"

// --- SEÇÃO DE VARIÁVEIS GLOBAIS DE ESTADO ---

ProgramMode g_currentMode = MODE_SELECT;
int g_isDragging = 0;
int g_segmentClickCount = 0;
Point g_segmentP1;
Point g_polygonVertices[MAX_POLYGON_VERTICES];
int g_polygonVertexCount = 0;
Point g_lastMousePos = {0.0f, 0.0f};
Point g_currentMousePos = {0.0f, 0.0f};

// --- SEÇÃO DE VARIÁVEIS GLOBAIS EXTERNAS ---
extern int g_windowWidth;
extern int g_windowHeight;

// --- SEÇÃO DE FUNÇÕES AUXILIARES ---

/**
 * @brief Retorna uma string descritiva para um dado modo de operação.
 * @param mode O modo de operação.
 * @return String constante com o nome do modo.
 */
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

// --- Funções Auxiliares para a Lógica de Seleção ---

/**
 * @brief Verifica se um ponto está dentro de um polígono usando o algoritmo de ray casting (tiro).
 * @param poly O polígono a ser testado.
 * @param p O ponto de teste.
 * @return 1 se o ponto está dentro, 0 caso contrário.
 */
static int selectPolygon(GfxPolygon* poly, Point p) {
    int crossings = 0;
    for (int i = 0; i < poly->numVertices; i++) {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->numVertices];

        if(((p1.y > p.y) && (p2.y > p.y)) || ((p1.y < p.y) && (p2.y < p.y)) || ((p1.x < p.x) && (p2.x < p.x))){
            continue;
        }
        if((p1.y > p.y && p2.y < p.y) || (p1.y < p.y && p2.y > p.y)){
            float xi = p1.x + (p.y - p1.y)*(p2.x - p1.x) / (p2.y - p1.y);
            if(xi> p.x){
                crossings++;
            }
        }
    }
    return crossings % 2; // Ímpar = dentro, Par = fora.
}

/**
 * @brief Calcula a menor distância de um ponto a qualquer uma das arestas de um polígono.
 * @param poly O polígono a ser testado.
 * @param click_point O ponto de clique.
 * @return A menor distância encontrada.
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

// Constantes para o algoritmo de Cohen-Sutherland
#define INSIDE 0 // 0000
#define LEFT   1 // 0001
#define RIGHT  2 // 0010
#define BOTTOM 4 // 0100
#define TOP    8 // 1000

/**
 * @brief Calcula o "outcode" de um ponto em relação a uma janela retangular.
 * @param p O ponto.
 * @param xmin, xmax, ymin, ymax As coordenadas da janela.
 * @return O outcode de 4 bits.
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
 * @brief Verifica se um segmento de reta intercepta um retângulo usando o algoritmo completo de Cohen-Sutherland.
 * @param s O segmento de reta a ser testado.
 * @param xmin, xmax, ymin, ymax As coordenadas do retângulo de tolerância.
 * @return 1 se o segmento intercepta o retângulo, 0 caso contrário.
 */
static int segmentIntersectsRect(Segment s, float xmin, float xmax, float ymin, float ymax) {
    // Copia os pontos do segmento para poder modificá-los.
    Point p1 = s.p1;
    Point p2 = s.p2;

    // Calcula os outcodes dos pontos iniciais.
    int outcode1 = computeOutCode(p1, xmin, xmax, ymin, ymax);
    int outcode2 = computeOutCode(p2, xmin, xmax, ymin, ymax);

    while (1) {
        // Caso 1: Aceitação trivial. Ambos os pontos estão dentro do retângulo.
        if (!(outcode1 | outcode2)) {
            return 1;
        }
        // Caso 2: Rejeição trivial. Ambos os pontos estão na mesma região externa.
        else if (outcode1 & outcode2) {
            return 0;
        }
        // Caso 3: Caso não trivial. O segmento pode cruzar o retângulo.
        else {
            float x, y; // Novas coordenadas do ponto de interseção.

            // Seleciona um ponto que está fora do retângulo.
            int outcodeOut = outcode1 ? outcode1 : outcode2;

            // Calcula o ponto de interseção com a fronteira correspondente.
            // A lógica é baseada na equação da reta.
            if (outcodeOut & TOP) {           // Ponto está acima do retângulo.
                x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y); //
                y = ymax; //
            } else if (outcodeOut & BOTTOM) { // Ponto está abaixo do retângulo.
                x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y); //
                y = ymin; //
            } else if (outcodeOut & RIGHT) {  // Ponto está à direita do retângulo.
                y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x); //
                x = xmax; //
            } else { // outcodeOut & LEFT -> Ponto está à esquerda do retângulo
                y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x); //
                x = xmin; //
            }

            // Atualiza o ponto que estava fora com o novo ponto de interseção
            // e recalcula o seu outcode. O loop continuará até que se chegue
            // a um caso de aceitação ou rejeição trivial.
            if (outcodeOut == outcode1) {
                p1.x = x;
                p1.y = y;
                outcode1 = computeOutCode(p1, xmin, xmax, ymin, ymax);
            } else {
                p2.x = x;
                p2.y = y;
                outcode2 = computeOutCode(p2, xmin, xmax, ymin, ymax);
            }
        }
    }
}

/**
 * @brief Seleciona o objeto mais próximo em um ponto de clique, priorizando Pontos > Segmentos > Polígonos.
 * @param click_point O ponto de clique na tela.
 * @return O índice do objeto selecionado ou -1 se nenhum objeto for encontrado.
 */
static int selectObjectAtPoint(Point click_point) {
    int found_index = -1;

    // Prioridade 1: Pontos
    int closest_point_index = -1;
    float min_sq_dist_point = -1.0f;
    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_POINT) {
            Point* p_obj = (Point*)g_objects[i].data;
            if (fabs(p_obj->x - click_point.x) <= CLICK_TOLERANCE &&
                fabs(p_obj->y - click_point.y) <= CLICK_TOLERANCE)
            {
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
        return closest_point_index;
    }

    // Prioridade 2: Segmentos
    int closest_segment_index = -1;
    float min_dist_segment = CLICK_TOLERANCE;
    float xmin = click_point.x - CLICK_TOLERANCE;
    float xmax = click_point.x + CLICK_TOLERANCE;
    float ymin = click_point.y - CLICK_TOLERANCE;
    float ymax = click_point.y + CLICK_TOLERANCE;
    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_SEGMENT) {
            Segment* s_obj = (Segment*)g_objects[i].data;
            if (segmentIntersectsRect(*s_obj, xmin, xmax, ymin, ymax)) {
                float dist = distPointSegment(click_point, *s_obj);
                if (dist < min_dist_segment) {
                    min_dist_segment = dist;
                    closest_segment_index = i;
                }
            }
        }
    }
    if (closest_segment_index != -1) {
        return closest_segment_index;
    }

    // Prioridade 3: Polígonos
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
    return closest_poly_index;
}

// --- SEÇÃO DE FUNÇÕES DE CALLBACK GLUT ---

/**
 * @brief Callback para eventos de teclado (teclas normais).
 * @param key O código ASCII da tecla pressionada.
 * @param x Coordenada X do mouse no momento do evento.
 * @param y Coordenada Y do mouse no momento do evento.
 */
void keyboardCallback(unsigned char key, int x, int y) {
    int objectIsSelected = (g_selectedObjectIndex != -1);
    ProgramMode oldMode = g_currentMode;

    switch (key) {
        // Ações de mudança de modo
        case 'p': case 'P': g_currentMode = MODE_CREATE_POINT; break;
        case 'l': case 'L': g_currentMode = MODE_CREATE_SEGMENT; break;
        case 'o': case 'O': g_currentMode = MODE_CREATE_POLYGON; break;
        case 's': case 'S': g_currentMode = MODE_SELECT; break;
        case 't': case 'T':
            if (objectIsSelected) g_currentMode = MODE_TRANSLATE;
            else printf("[AVISO] Selecione um objeto para o modo de translação.\n");
            break;
        case 'r': case 'R':
            if (objectIsSelected) g_currentMode = MODE_ROTATE;
            else printf("[AVISO] Selecione um objeto para o modo de rotação.\n");
            break;
        case 'e': case 'E':
            if (objectIsSelected) g_currentMode = MODE_SCALE;
            else printf("[AVISO] Selecione um objeto para o modo de escala.\n");
            break;
        case 'h': case 'H':
            if (objectIsSelected) g_currentMode = MODE_SHEAR;
            else printf("[AVISO] Selecione um objeto para o modo de cisalhamento.\n");
            break;
        case 'm': case 'M':
            if (objectIsSelected) g_currentMode = MODE_REFLECT;
            else printf("[AVISO] Selecione um objeto para o modo de reflexão.\n");
            break;

        // --- LÓGICA PARA O FECHO CONVEXO ---
        case 'c': case 'C':
            if (objectIsSelected) {
                Object* selectedObject = &g_objects[g_selectedObjectIndex];
                if (selectedObject->type == OBJECT_TYPE_POLYGON) {
                    // printf("[INFO] Acionando calculo de fecho convexo...\n");
                    GfxPolygon* originalPolygon = (GfxPolygon*)selectedObject->data;

                    // Chama o algoritmo da Marcha de Jarvis.
                    GfxPolygon* newConvexHull = jarvisMarch(originalPolygon);

                    // Se o algoritmo foi bem-sucedido, substitui o polígono.
                    if (newConvexHull != NULL) {
                        free(originalPolygon); // Libera a memória do polígono antigo.
                        selectedObject->data = newConvexHull; // Atualiza o ponteiro para o novo.
                        // printf("[INFO] Poligono transformado em seu fecho convexo.\n");
                    } else {
                        printf("[AVISO] Nao foi possivel calcular o fecho convexo (poligono pode ter menos de 3 vertices).\n");
                    }
                } else {
                    printf("[AVISO] O objeto selecionado nao eh um poligono. Selecione um poligono para usar esta funcao.\n");
                }
            } else {
                printf("[AVISO] Nenhum objeto selecionado. Selecione um poligono para calcular o fecho convexo.\n");
            }
            break;

        // Ações específicas de transformação
        case 'x': case 'X':
            if (objectIsSelected && g_currentMode == MODE_REFLECT) {
                reflectObject(g_selectedObjectIndex, 0); // Eixo X
            }
            break;
        case 'y': case 'Y':
            if (objectIsSelected && g_currentMode == MODE_REFLECT) {
                reflectObject(g_selectedObjectIndex, 1); // Eixo Y
            }
            break;
        case '=':
            if (objectIsSelected && g_currentMode == MODE_SCALE) {
                scaleObject(g_selectedObjectIndex, 1.1f, 1.1f);
            }
            break;
        case '-':
            if (objectIsSelected && g_currentMode == MODE_SCALE) {
                scaleObject(g_selectedObjectIndex, 0.9f, 0.9f);
            }
            break;

        // Ações de gerenciamento de objetos
        case 127: // Tecla DELETE
            if (objectIsSelected) removeObject(g_selectedObjectIndex);
            break;
        case 27: // Tecla ESC
            clearAllObjects(); exit(0);
            break;

        // Ações de animação
        case 'b': case 'B': anim_toggle_selected(); break;
        case 'v': case 'V': anim_kick_towards_mouse(600.0f); break;
        case 'g': case 'G':
            if (objectIsSelected) {
                g_anim[g_selectedObjectIndex].gravity = !g_anim[g_selectedObjectIndex].gravity;
            }
            break;
        case '[':
            if (objectIsSelected) {
                g_anim[g_selectedObjectIndex].vx *= 0.9f;
                g_anim[g_selectedObjectIndex].vy *= 0.9f;
            }
            break;
        case ']':
            if (objectIsSelected) {
                g_anim[g_selectedObjectIndex].vx *= 1.1f;
                g_anim[g_selectedObjectIndex].vy *= 1.1f;
            }
            break;
    }

    if (g_currentMode != oldMode) {
        printf("[DEBUG] Modo alterado para %s\n", getModeString(g_currentMode));
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

/**
 * @brief Callback para eventos de teclado (teclas especiais, como F1, setas, etc).
 * @param key O código da tecla especial.
 * @param x Coordenada X do mouse.
 * @param y Coordenada Y do mouse.
 */
void specialKeysCallback(int key, int x, int y) {
    // Ações de Arquivo
    if (key == GLUT_KEY_F5) {
        saveSceneToFile("scene.txt");
        return;
    }
    if (key == GLUT_KEY_F9) {
        loadSceneFromFile("scene.txt");
        glutPostRedisplay();
        return;
    }

    // Ações condicionadas ao modo de operação
    if (g_selectedObjectIndex != -1 && g_currentMode == MODE_ROTATE) {
        switch (key) {
            case GLUT_KEY_LEFT: rotateObject(g_selectedObjectIndex, 5); break;
            case GLUT_KEY_RIGHT: rotateObject(g_selectedObjectIndex, -5); break;
        }
        glutPostRedisplay();
    }
    if (g_selectedObjectIndex != -1 && g_currentMode == MODE_SHEAR) {
        float shearFactor = 0.1f;
        switch (key) {
            case GLUT_KEY_RIGHT: shearObject(g_selectedObjectIndex, shearFactor, 0.0f); break;
            case GLUT_KEY_LEFT: shearObject(g_selectedObjectIndex, -shearFactor, 0.0f); break;
            case GLUT_KEY_UP: shearObject(g_selectedObjectIndex, 0.0f, shearFactor); break;
            case GLUT_KEY_DOWN: shearObject(g_selectedObjectIndex, 0.0f, -shearFactor); break;
        }
        glutPostRedisplay();
    }
}

/**
 * @brief Callback para eventos de clique do mouse.
 * @param button O botão do mouse pressionado.
 * @param state O estado do botão (GLUT_DOWN ou GLUT_UP).
 * @param x Coordenada X do clique.
 * @param y Coordenada Y do clique.
 */
void mouseCallback(int button, int state, int x, int y) {
    float gl_y = g_windowHeight - (float)y;
    Point click_point = { (float)x, gl_y };

    if (state == GLUT_DOWN) {
        if (g_currentMode == MODE_CREATE_POINT && button == GLUT_LEFT_BUTTON) {
            Point* newPoint = (Point*)malloc(sizeof(Point));
            *newPoint = createPoint(click_point.x, click_point.y);
            addObject(OBJECT_TYPE_POINT, newPoint);
            g_selectedObjectIndex = g_numObjects - 1;
        }
        else if (g_currentMode == MODE_CREATE_SEGMENT && button == GLUT_LEFT_BUTTON) {
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
        else if (g_currentMode == MODE_CREATE_POLYGON) {
            if (button == GLUT_LEFT_BUTTON && g_polygonVertexCount < MAX_POLYGON_VERTICES) {
                g_polygonVertices[g_polygonVertexCount++] = click_point;
            } else if (button == GLUT_RIGHT_BUTTON && g_polygonVertexCount >= 3) {
                GfxPolygon* newPolygon = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                *newPolygon = createPolygon();
                for (int i = 0; i < g_polygonVertexCount; i++) {
                    addVertexToPolygon(newPolygon, g_polygonVertices[i]);
                }
                addObject(OBJECT_TYPE_POLYGON, newPolygon);
                g_selectedObjectIndex = g_numObjects - 1;
                g_polygonVertexCount = 0;
            }
        }
        else if (g_currentMode == MODE_SELECT || g_currentMode == MODE_TRANSLATE) {
            if (button == GLUT_LEFT_BUTTON) {
                int found_index = selectObjectAtPoint(click_point);
                if (found_index != -1) {
                    g_selectedObjectIndex = found_index;
                    g_isDragging = 1;
                    g_lastMousePos = click_point;
                } else {
                    g_selectedObjectIndex = -1;
                    g_isDragging = 0;
                }
            }
        }
    }
    else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        g_isDragging = 0;
    }
    glutPostRedisplay();
}

/**
 * @brief Callback para o evento de arrastar o mouse (botão pressionado).
 * @param x Coordenada X do mouse.
 * @param y Coordenada Y do mouse.
 */
void motionCallback(int x, int y) {
    if (g_isDragging && g_currentMode == MODE_TRANSLATE && g_selectedObjectIndex != -1) {
        float gl_y = g_windowHeight - (float)y;
        Point currentMousePos = {(float)x, gl_y};
        float dx = currentMousePos.x - g_lastMousePos.x;
        float dy = currentMousePos.y - g_lastMousePos.y;
        translateObject(g_selectedObjectIndex, dx, dy);
        g_lastMousePos = currentMousePos;
        glutPostRedisplay();
    }
}

/**
 * @brief Callback para o evento de movimento passivo do mouse (sem botão pressionado).
 * @param x Coordenada X do mouse.
 * @param y Coordenada Y do mouse.
 */
void passiveMotionCallback(int x, int y) {
    g_currentMousePos.x = (float)x;
    g_currentMousePos.y = g_windowHeight - (float)y;
    if (g_currentMode == MODE_CREATE_SEGMENT && g_segmentClickCount == 1) {
        glutPostRedisplay();
    }
}
