/**
 * @file animation.c
 * @brief Implementação da lógica de animação baseada em física.
 *
 * Contém as funções para inicializar, atualizar e gerenciar o estado
 * de animação de cada objeto, incluindo integração de movimento,
 * detecção de colisões e interações do usuário.
 */

#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include "animation.h"
#include "objects.h"
#include "transformations.h"
#include "utils.h"
#include "input.h"
#include "config.h"

// --- SEÇÃO DE VARIÁVEIS GLOBAIS (DEFINIÇÕES) ---

AnimSlot g_anim[MAX_OBJECTS];

// --- SEÇÃO DE FUNÇÕES AUXILIARES (IMPLEMENTAÇÃO PRIVADA) ---

/**
 * @brief Calcula a Bounding Box (AABB) de um objeto.
 *
 * Uma AABB (Axis-Aligned Bounding Box) é o menor retângulo, alinhado aos eixos,
 * que contém completamente o objeto. É usada para detecção de colisão eficiente.
 * @param i O índice do objeto.
 * @param minx, maxx, miny, maxy Ponteiros para armazenar as coordenadas da caixa.
 */
static void object_aabb(int i, float* minx, float* maxx, float* miny, float* maxy) {
    *minx = *miny = 1e9f;
    *maxx = *maxy = -1e9f;
    Object* o = &g_objects[i];

    if (o->type == OBJECT_TYPE_POINT) {
        Point* p = (Point*)o->data;
        *minx = *maxx = p->x;
        *miny = *maxy = p->y;
    } else if (o->type == OBJECT_TYPE_SEGMENT) {
        Segment* s = (Segment*)o->data;
        *minx = (s->p1.x < s->p2.x) ? s->p1.x : s->p2.x;
        *maxx = (s->p1.x > s->p2.x) ? s->p1.x : s->p2.x;
        *miny = (s->p1.y < s->p2.y) ? s->p1.y : s->p2.y;
        *maxy = (s->p1.y > s->p2.y) ? s->p1.y : s->p2.y;
    } else if (o->type == OBJECT_TYPE_POLYGON) {
        GfxPolygon* poly = (GfxPolygon*)o->data;
        for (int v = 0; v < poly->numVertices; v++) {
            float x = poly->vertices[v].x;
            float y = poly->vertices[v].y;
            if (x < *minx) *minx = x;
            if (x > *maxx) *maxx = x;
            if (y < *miny) *miny = y;
            if (y > *maxy) *maxy = y;
        }
    }
}

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

void anim_init_all(void) {
    memset(g_anim, 0, sizeof(g_anim));
}

void anim_enable(int idx, float vx, float vy, float e, int gravity) {
    if (idx < 0 || idx >= g_numObjects) return;
    g_anim[idx].active = 1;
    g_anim[idx].vx = vx;
    g_anim[idx].vy = vy;
    g_anim[idx].e = e;
    g_anim[idx].gravity = gravity;
    g_anim[idx].vmax = 1200.0f;
}

void anim_disable(int idx) {
    if (idx < 0 || idx >= g_numObjects) return;
    g_anim[idx].active = 0;
}

void anim_toggle_selected(void) {
    int i = g_selectedObjectIndex;
    if (i < 0) return;
    if (g_anim[i].active) {
        anim_disable(i);
    } else {
        anim_enable(i, 180.0f, 120.0f, 0.90f, 1);
    }
}

void anim_kick_towards_mouse(float speed) {
    int i = g_selectedObjectIndex;
    if (i < 0) return;

    // Calcula o vetor direção do centro do objeto para o mouse.
    Point c = getObjectCenter(&g_objects[i]);
    Point m = g_currentMousePos;
    float dx = m.x - c.x;
    float dy = m.y - c.y;

    // Normaliza o vetor para obter apenas a direção.
    float L = sqrtf(dx * dx + dy * dy);
    if (L < 1e-5f) return; // Evita divisão por zero se o mouse estiver no centro.
    dx /= L;
    dy /= L;

    // Aplica a velocidade na direção calculada.
    g_anim[i].vx = dx * speed;
    g_anim[i].vy = dy * speed;
    g_anim[i].active = 1;
}

void anim_step(float dtim) {
    const float G = 700.0f; // Constante gravitacional.
    for (int i = 0; i < g_numObjects; i++) {
        if (!g_anim[i].active) continue;

        // 1. Integração de Euler: atualiza a velocidade com base nas forças (gravidade).
        if (g_anim[i].gravity) {
            g_anim[i].vy -= G * dtim;
        }

        // Limita a velocidade máxima.
        float sp = sqrtf(g_anim[i].vx * g_anim[i].vx + g_anim[i].vy * g_anim[i].vy);
        if (sp > g_anim[i].vmax) {
            float k = g_anim[i].vmax / (sp + 1e-6f);
            g_anim[i].vx *= k;
            g_anim[i].vy *= k;
        }

        // 2. Atualiza a posição do objeto.
        translateObject(i, g_anim[i].vx * dtim, g_anim[i].vy * dtim);

        // 3. Detecção e resposta de colisão com as bordas da tela.
        float minx, maxx, miny, maxy;
        object_aabb(i, &minx, &maxx, &miny, &maxy);

        if (minx < 0.0f && g_anim[i].vx < 0.0f) {
            translateObject(i, -minx, 0.0f);
            g_anim[i].vx = -g_anim[i].vx * g_anim[i].e;
        }
        if (maxx > WINDOW_WIDTH && g_anim[i].vx > 0.0f) {
            translateObject(i, WINDOW_WIDTH - maxx, 0.0f);
            g_anim[i].vx = -g_anim[i].vx * g_anim[i].e;
        }
        if (miny < 0.0f && g_anim[i].vy < 0.0f) {
            translateObject(i, 0.0f, -miny);
            g_anim[i].vy = -g_anim[i].vy * g_anim[i].e;
        }
        if (maxy > WINDOW_HEIGHT && g_anim[i].vy > 0.0f) {
            translateObject(i, 0.0f, WINDOW_HEIGHT - maxy);
            g_anim[i].vy = -g_anim[i].vy * g_anim[i].e;
        }

        // 4. Atrito (arrasto) para diminuir a velocidade gradualmente.
        g_anim[i].vx *= 0.999f;
        g_anim[i].vy *= 0.999f;
    }
}

void anim_on_remove_compact(int removed_idx) {
    // Desloca todos os slots de animação para preencher o espaço do objeto removido.
    for (int i = removed_idx; i < g_numObjects; i++) {
        g_anim[i] = g_anim[i + 1];
    }
    // Zera o último slot que ficou sobrando para evitar dados fantasmas.
    if (g_numObjects >= 0 && g_numObjects < MAX_OBJECTS) {
        g_anim[g_numObjects] = (AnimSlot){0};
    }
}
