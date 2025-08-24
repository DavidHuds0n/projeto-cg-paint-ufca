/**
 * @file file_io.c
 * @brief Implementa as fun��es de salvamento e carregamento de arquivos da cena.
 *
 * O m�dulo converte a lista de objetos em um formato de texto para salvamento
 * e, de forma inversa, l� este formato para reconstruir a cena na mem�ria.
 */

#include "file_io.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h>
#include <string.h>

// --- SE��O DE FUN��ES P�BLICAS ---

void saveSceneToFile(const char* filename) {
    // 1. Abre o arquivo no modo de escrita ("w").
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("[ERRO] Nao foi possivel abrir '%s' para escrita.\n", filename);
        return;
    }

    // 2. Conta os objetos de cada tipo para o cabe�alho do arquivo.
    int pointCount = 0, lineCount = 0, polyCount = 0;
    for (int i = 0; i < g_numObjects; i++) {
        switch (g_objects[i].type) {
            case OBJECT_TYPE_POINT:    pointCount++; break;
            case OBJECT_TYPE_SEGMENT: lineCount++;  break;
            case OBJECT_TYPE_POLYGON: polyCount++;  break;
        }
    }

    // 3. Salva a se��o de PONTOS.
    fprintf(f, "[PONTOS]\n");
    fprintf(f, "Cont: %d\n", pointCount);
    int pi = 0;
    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_POINT) {
            Point* p = (Point*)g_objects[i].data;
            fprintf(f, "P%d: %.2f, %.2f\n", pi++, p->x, p->y);
        }
    }
    fprintf(f, "\n");

    // 4. Salva a se��o de LINHAS.
    fprintf(f, "[LINHAS]\n");
    fprintf(f, "Cont: %d\n", lineCount);
    int li = 0;
    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_SEGMENT) {
            Segment* s = (Segment*)g_objects[i].data;
            fprintf(f, "L%d: (%.2f, %.2f) -> (%.2f, %.2f)\n",
                     li++, s->p1.x, s->p1.y, s->p2.x, s->p2.y);
        }
    }
    fprintf(f, "\n");

    // 5. Salva a se��o de POL�GONOS.
    fprintf(f, "[POLIGONOS]\n");
    fprintf(f, "Cont: %d\n", polyCount);
    int poli = 0;
    for (int i = 0; i < g_numObjects; i++) {
        if (g_objects[i].type == OBJECT_TYPE_POLYGON) {
            GfxPolygon* poly = (GfxPolygon*)g_objects[i].data;
            fprintf(f, "Poligono %d - Pontas: %d\n", poli++, poly->numVertices);
            for (int v = 0; v < poly->numVertices; v++) {
                fprintf(f, "  V%d: %.2f, %.2f\n", v, poly->vertices[v].x, poly->vertices[v].y);
            }
        }
    }

    // 6. Fecha o arquivo.
    fclose(f);
}

void loadSceneFromFile(const char* filename) {
    // 1. Abre o arquivo no modo de leitura ("r").
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("[ERRO] N�o foi poss�vel abrir '%s' para leitura.\n", filename);
        return;
    }

    // 2. Limpa a cena atual para carregar a nova.
    clearAllObjects();

    char line[256];
    int section = 0; // 0=Nenhum, 1=PONTOS, 2=LINHAS, 3=POLIGONOS

    // Variaveis de estado para reconstruir pol�gonos.
    int buildingPoly = 0;
    GfxPolygon tempPoly;

    // 3. L� o arquivo linha por linha.
    while (fgets(line, sizeof(line), f)) {
        // Normaliza a linha.
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue; // Pula linhas vazias.

        // L�gica de m�quina de estados para mudar a se��o de leitura.
        if (strcmp(line, "[PONTOS]") == 0)      { section = 1; continue; }
        if (strcmp(line, "[LINHAS]") == 0)      { section = 2; continue; }
        if (strcmp(line, "[POLIGONOS]") == 0)   { section = 3; continue; }
        if (strstr(line, "Cont:") != NULL) continue;

        // Finaliza o pol�gono anterior antes de ler um novo objeto
        if (section != 3 && buildingPoly && tempPoly.numVertices > 0) {
            GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
            *poly = tempPoly;
            addObject(OBJECT_TYPE_POLYGON, poly);
            buildingPoly = 0;
        }

        // 4. Interpreta a linha com base na se��o atual.
        if (section == 1) { // Se��o de PONTOS
            float x, y;
            int idxDummy;
            if (sscanf(line, "P%d: %f, %f", &idxDummy, &x, &y) == 3) {
                Point* p = (Point*)malloc(sizeof(Point));
                *p = createPoint(x, y);
                addObject(OBJECT_TYPE_POINT, p);
            }
        } else if (section == 2) { // Se��o de LINHAS
            float x1, y1, x2, y2;
            int idxDummy;
            if (sscanf(line, "L%d: (%f, %f) -> (%f, %f)", &idxDummy, &x1, &y1, &x2, &y2) == 5) {
                Segment* s = (Segment*)malloc(sizeof(Segment));
                *s = createSegment((Point){x1, y1}, (Point){x2, y2});
                addObject(OBJECT_TYPE_SEGMENT, s);
            }
        } else if (section == 3) { // Se��o de POL�GONOS
            // L�gica para detectar o in�cio de um pol�gono ou um novo v�rtice.
            if (strncmp(line, "Poligono", 8) == 0) {
                if (buildingPoly && tempPoly.numVertices > 0) {
                    GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                    *poly = tempPoly;
                    addObject(OBJECT_TYPE_POLYGON, poly);
                }
                tempPoly = createPolygon();
                buildingPoly = 1;
            } else {
                float vx, vy;
                int vIdxDummy;
                if (sscanf(line, "  V%d: %f, %f", &vIdxDummy, &vx, &vy) == 3) {
                    if (buildingPoly) {
                        if (tempPoly.numVertices < MAX_POLYGON_VERTICES) {
                            tempPoly.vertices[tempPoly.numVertices++] = (Point){vx, vy};
                        } else {
                            printf("[AVISO] V�rtice ignorado: excedeu MAX_POLYGON_VERTICES.\n");
                        }
                    }
                }
            }
        }
    }

    // 5. Finaliza o �ltimo pol�gono se o arquivo terminou durante sua leitura.
    if (buildingPoly && tempPoly.numVertices > 0) {
        GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
        *poly = tempPoly;
        addObject(OBJECT_TYPE_POLYGON, poly);
    }

    // 6. Fecha o arquivo.
    fclose(f);
}
