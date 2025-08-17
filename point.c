// point.c
// Implementa��o das fun��es para criar, desenhar e liberar objetos do tipo Ponto.

#include <GL/glut.h>
#include <stdio.h>

#include "point.h"
#include "config.h"

Point createPoint(float x, float y) {
    Point p = {x, y};
    // O printf abaixo � para fins de depura��o e pode ser removido na vers�o final.
    // printf("[Point] Novo ponto criado em (%.1f, %.1f).\n", p.x, p.y);
    return p;
}

void drawPoint(Point* p, int is_selected) {
    // Define a cor do ponto com base no estado de sele��o.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado.
    }

    // Aumenta o tamanho do ponto para torn�-lo mais vis�vel e f�cil de selecionar.
    // Basear o tamanho na toler�ncia de clique garante consist�ncia visual.
    glPointSize(CLICK_TOLERANCE / 2.0f);

    // Desenha o ponto na tela usando as primitivas do OpenGL.
    glBegin(GL_POINTS);
        glVertex2f(p->x, p->y);
    glEnd();

    // Reseta o tamanho do ponto para o padr�o para n�o afetar outros desenhos na cena.
    glPointSize(1.0f);
}

void freePoint(Point* p) {
    // A estrutura 'Point' n�o possui ponteiros ou mem�ria alocada dinamicamente
    // em seu interior, ent�o n�o h� nada a ser liberado aqui.
    // A fun��o existe para manter uma API consistente no m�dulo 'objects.c',
    // que chama uma fun��o 'free<Objeto>' para qualquer tipo de objeto.
    // printf("[Point] Fun��o freePoint chamada.\n");
}
