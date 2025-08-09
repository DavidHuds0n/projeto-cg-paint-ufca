// polygon.c
// Implementação das funções para manipulação de polígonos.

#include <GL/glut.h> // Para funções de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depuração)
#include "polygon.h" // Inclui o próprio cabeçalho (agora com GfxPolygon)
#include "point.h"   // Para usar a estrutura Point
#include "config.h"  // Para usar CLICK_TOLERANCE e MAX_POLYGON_VERTICES

// Implementação das funções para GfxPolygon

// Cria um novo polígono vazio (pronto para adicionar vértices)
GfxPolygon createPolygon() { // ATUALIZADO: Retorna GfxPolygon
    GfxPolygon poly; // ATUALIZADO: Usa GfxPolygon
    poly.numVertices = 0; // Inicializa o número de vértices como zero
    printf("[GfxPolygon] Novo polígono vazio criado.\n");
    return poly;
}

// Adiciona um vértice ao polígono
void addVertexToPolygon(GfxPolygon* poly, Point p) { // ATUALIZADO: Recebe GfxPolygon*
    // Verifica se ainda há espaço para adicionar mais vértices
    if (poly->numVertices < MAX_POLYGON_VERTICES) { // Usa MAX_POLYGON_VERTICES de config.h
        poly->vertices[poly->numVertices] = p; // Adiciona o ponto ao array de vértices
        poly->numVertices++;                   // Incrementa o contador de vértices
        printf("[GfxPolygon] Vértice adicionado em (%.1f, %.1f). Total de vértices: %d.\n", p.x, p.y, poly->numVertices);
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite máximo de vértices (%d) atingido para este polígono.\n", MAX_POLYGON_VERTICES);
    }
}

// Desenha o polígono na tela
void drawPolygon(GfxPolygon* poly, int is_selected) { // ATUALIZADO: Recebe GfxPolygon*
    if (poly->numVertices < 2) {
        // Polígonos precisam de pelo menos 2 vértices para uma linha,
        // e 3 para ser uma forma fechada.
        // Se houver 1 vértice, desenha como um ponto (opcional, para feedback de criação)
        if (poly->numVertices == 1) {
            // Desenha o primeiro vértice como um ponto grande para indicar onde o clique foi feito
            if (is_selected) {
                glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para ponto de seleção (se estiver no modo de criação e for o primeiro vértice)
            } else {
                 glColor3f(0.0f, 0.0f, 1.0f); // Azul para ponto de feedback
            }
            glPointSize(CLICK_TOLERANCE * 1.5f); // Um ponto maior para feedback visual
            glBegin(GL_POINTS);
            glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
            glEnd();
            glPointSize(1.0f); // Volta ao tamanho padrão de ponto
        }
        return;
    }

    // Define a cor do polígono
    if (is_selected) {
        // Se o polígono estiver selecionado, desenha-o em vermelho.
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    } else {
        // Caso contrário, desenha-o em azul (ou outra cor padrão).
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
    }

    // Desenha o contorno do polígono
    glLineWidth(2.0f); // Largura da linha
    glBegin(GL_LINE_LOOP); // GL_LINE_LOOP conecta o último vértice ao primeiro, fechando o polígono
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glLineWidth(1.0f); // Volta à largura de linha padrão

    // Opcional: Desenhar os vértices do polígono para maior clareza visual
    // Isso pode ser útil para depuração ou para mostrar os pontos de controle.
    glColor3f(0.5f, 0.5f, 0.5f); // Cor cinza para os vértices
    glPointSize(CLICK_TOLERANCE / 2.0f); // Pontos menores nos vértices
    glBegin(GL_POINTS);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glPointSize(1.0f); // Volta ao tamanho padrão de ponto
}

// Libera a memória alocada para um polígono (se houver, para casos mais complexos)
// Atualmente, não há alocação dinâmica DENTRO da estrutura GfxPolygon em si,
// então esta função é um placeholder.
void freePolygon(GfxPolygon* poly) { // ATUALIZADO: Recebe GfxPolygon*
    printf("[GfxPolygon] Função freePolygon chamada para polígono com %d vértices.\n", poly->numVertices);
}
