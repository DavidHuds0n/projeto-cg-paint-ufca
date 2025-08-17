// polygon.c
// Implementação das funções para criar, adicionar vértices, desenhar e
// liberar objetos do tipo Polígono.

#include <GL/glut.h>
#include <stdio.h>

#include "polygon.h"
#include "point.h"
#include "config.h"


GfxPolygon createPolygon() {
    GfxPolygon poly;
    poly.numVertices = 0; // Um polígono recém-criado não tem vértices.
    // printf("[GfxPolygon] Novo polígono vazio criado.\n");
    return poly;
}

void addVertexToPolygon(GfxPolygon* poly, Point p) {
    // Verifica se ainda há espaço no array de vértices antes de adicionar.
    if (poly->numVertices < MAX_POLYGON_VERTICES) {
        poly->vertices[poly->numVertices] = p;
        poly->numVertices++;
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite máximo de vértices (%d) atingido.\n", MAX_POLYGON_VERTICES);
    }
}

void drawPolygon(GfxPolygon* poly, int is_selected) {
    // --- Tratamento de Casos Especiais ---
    // Um polígono só pode ser desenhado como uma área fechada com pelo menos 3 vértices.
    if (poly->numVertices < 2) {
        // Se houver apenas 1 vértice, desenha um ponto grande como feedback visual
        // para o usuário durante o processo de criação do polígono.
        if (poly->numVertices == 1) {
            glColor3f(0.0f, 0.0f, 1.0f); // Ponto de feedback em azul.
            glPointSize(CLICK_TOLERANCE / 2.0f);
            glBegin(GL_POINTS);
                glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
            glEnd();
            glPointSize(1.0f); // Reseta o tamanho do ponto.
        }
        return; // Não desenha mais nada se tiver menos de 2 vértices.
    }

    // --- Desenho do Polígono Preenchido ---
    // Define a cor da área do polígono com base no estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // Usa GL_POLYGON, que desenha uma forma sólida e preenchida conectando
    // todos os vértices. O OpenGL cuida de fechar o polígono automaticamente.
    glBegin(GL_POLYGON);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
}

void freePolygon(GfxPolygon* poly) {
    // A struct 'GfxPolygon' usa um array de tamanho fixo para os vértices,
    // então não há memória interna alocada dinamicamente para ser liberada.
    // A função existe para manter a consistência com a API do 'objects.c'.
    // printf("[GfxPolygon] Função freePolygon chamada.\n");
}
