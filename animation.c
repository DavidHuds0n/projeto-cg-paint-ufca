// animation.c
// Implementação da lógica de animação.

#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include "animation.h"
#include "objects.h"
#include "transformations.h"
#include "utils.h"
#include "input.h"
#include "config.h"

AnimSlot g_anim[MAX_OBJECTS];

static void object_aabb(int i, float* minx,float* maxx,float* miny,float* maxy){
    *minx=*miny= 1e9f; *maxx=*maxy=-1e9f;
    Object* o = &g_objects[i];
    if(o->type == OBJECT_TYPE_POINT){
        Point* p = (Point*)o->data;
        *minx = *maxx = p->x;
        *miny = *maxy = p->y;
    } else if(o->type == OBJECT_TYPE_SEGMENT){
        Segment* s = (Segment*)o->data;
        //verifica o x e y inicial e final
        float xs[2] = {s->p1.x, s->p2.x};
        float ys[2] = {s->p1.y, s->p2.y};
        for(int k=0;k<2;k++){ if(xs[k]<*minx)*minx=xs[k]; if(xs[k]>*maxx)*maxx=xs[k];
                              if(ys[k]<*miny)*miny=ys[k]; if(ys[k]>*maxy)*maxy=ys[k]; }
    } else if(o->type == OBJECT_TYPE_POLYGON){
        GfxPolygon* poly = (GfxPolygon*)o->data;
        for(int v=0; v<poly->numVertices; v++){
            float x = poly->vertices[v].x, y = poly->vertices[v].y;
            if(x<*minx)*minx=x; if(x>*maxx)*maxx=x;
            if(y<*miny)*miny=y; if(y>*maxy)*maxy=y;
        }
    }
}

void anim_init_all(void){
    //inicia todas as animações
    memset(g_anim, 0, sizeof(g_anim));
}

void anim_enable(int idx, float vx, float vy, float e, int gravity){
    if(idx<0 || idx>=g_numObjects) return;
    g_anim[idx].active  = 1;
    g_anim[idx].vx      = vx;
    g_anim[idx].vy      = vy;
    //define se mantem a velocidade ou se altera ela
    g_anim[idx].e       = e;
    g_anim[idx].gravity = gravity;
    g_anim[idx].vmax    = 1200.0f; // velocidade maxima da animação
}

void anim_disable(int idx){
    if(idx<0 || idx>=g_numObjects) return;
    g_anim[idx].active = 0;
}

void anim_toggle_selected(void){
    //função q attiv e desativa a animação
    int i = g_selectedObjectIndex;
    if(i<0) return;
    if(g_anim[i].active) anim_disable(i);
    // está diminuindo a velocidade
    else anim_enable(i, 180.0f, 120.0f, 0.90f, 1);
}

void anim_kick_towards_mouse(float speed){
    int i = g_selectedObjectIndex;
    if(i<0) return;
     // direção do selecionado -> mouse (em coordenadas já no teu mundo 0..W,0..H)
    Point c = getObjectCenter(&g_objects[i]);
    Point m = g_currentMousePos;
    float dx = m.x - c.x, dy = m.y - c.y;
    // se o centro for zero ele n faz a divisão
    // ele verifica se na animação do objeto seguindo o mouse o objeto tem q ir ou se o mouse esta
    //em cima do objeto
    float L = sqrtf(dx*dx + dy*dy); if(L<1e-5f) return;
    dx/=L; dy/=L;
    //velocidade q o objeto vai ate o mouse
    g_anim[i].vx = dx*speed;
    g_anim[i].vy = dy*speed;
    //garante q a animação esteja ativa
    g_anim[i].active = 1;
}

//Auxilia a delimitar as bordas
static float clamp(float v, float low, float hig){ return v<low?low:(v>hig?hig:v); }


void anim_step(float dtim){
    const float G = 700.0f;
    for(int i=0;i<g_numObjects;i++){
        if(!g_anim[i].active) continue;

        // 1) Integra velocidades
        if(g_anim[i].gravity) g_anim[i].vy -= G * dtim;

        // verifica se a velocidade ultrapassou a velocidade maxima
        float sp = sqrtf(g_anim[i].vx*g_anim[i].vx + g_anim[i].vy*g_anim[i].vy);
        if(sp > g_anim[i].vmax){
            float k = g_anim[i].vmax / (sp + 1e-6f);
            g_anim[i].vx *= k; g_anim[i].vy *= k;
        }
         // 2) Move via MATRIZ DE TRANSLAÇÃO (usa tua API)
        translateObject(i, g_anim[i].vx*dtim, g_anim[i].vy*dtim);

        // 3) Colisão com as "paredes" (viewport definida em gluOrtho2D(0..W,0..H))
        float minx,maxx,miny,maxy;
        object_aabb(i, &minx,&maxx,&miny,&maxy);

        // esquerda
        if(minx < 0.0f && g_anim[i].vx < 0.0f){
            translateObject(i, -minx, 0.0f);
            g_anim[i].vx = -g_anim[i].vx * g_anim[i].e;
        }
        // direita
        if(maxx > WINDOW_WIDTH && g_anim[i].vx > 0.0f){
            translateObject(i, WINDOW_WIDTH - maxx, 0.0f);
            g_anim[i].vx = -g_anim[i].vx * g_anim[i].e;
        }
        // fundo
        if(miny < 0.0f && g_anim[i].vy < 0.0f){
            translateObject(i, 0.0f, -miny);
            g_anim[i].vy = -g_anim[i].vy * g_anim[i].e;
        }
        // topo
        if(maxy > WINDOW_HEIGHT && g_anim[i].vy > 0.0f){
            translateObject(i, 0.0f, WINDOW_HEIGHT - maxy);
            g_anim[i].vy = -g_anim[i].vy * g_anim[i].e;
        }

        // 4) Atrito leve opcional
        g_anim[i].vx *= 0.999f;
        g_anim[i].vy *= 0.999f;
    }
}

// manter o vetor g_anim alinhado com g_objects após remoção
void anim_on_remove_compact(int removed_idx){
    for(int i=removed_idx; i<g_numObjects-1; i++){
        g_anim[i] = g_anim[i+1];
    }
    // zera o último slot sobrando
    g_anim[g_numObjects-1] = (AnimSlot){0};
}
