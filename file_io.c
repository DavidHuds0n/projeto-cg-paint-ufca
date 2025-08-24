/**
 * @file file_io.c
 * @brief Implementa as funções de salvamento e carregamento de arquivos da cena.
 *
 * O módulo converte a lista de objetos em um formato de texto para salvamento
 * e, de forma inversa, lê este formato para reconstruir a cena na memória.
 */

#include "file_io.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h>
#include <string.h>

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

void saveSceneToFile(const char* filename) {
    // 1. Abre o arquivo no modo de escrita ("w").
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("[ERRO] Nao foi possivel abrir '%s' para escrita.\n", filename);
        return;
    }

    // 2. Conta os objetos de cada tipo para o cabeçalho do arquivo.
    int pointCount = 0, lineCount = 0, polyCount = 0;
    for (int i = 0; i < g_numObjects; i++) {
        switch (g_objects[i].type) {
            case OBJECT_TYPE_POINT:    pointCount++; break;
            case OBJECT_TYPE_SEGMENT: lineCount++;  break;
            case OBJECT_TYPE_POLYGON: polyCount++;  break;
        }
    }

    // 3. Salva a seção de PONTOS.
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

    // 4. Salva a seção de LINHAS.
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

    // 5. Salva a seção de POLÍGONOS.
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
        printf("[ERRO] Não foi possível abrir '%s' para leitura.\n", filename);
        return;
    }

    // 2. Limpa a cena atual para carregar a nova.
    clearAllObjects();

    char line[256];
    int section = 0; // 0=Nenhum, 1=PONTOS, 2=LINHAS, 3=POLIGONOS

    // Variaveis de estado para reconstruir polígonos.
    int buildingPoly = 0;
    GfxPolygon tempPoly;

    // 3. Lê o arquivo linha por linha.
    while (fgets(line, sizeof(line), f)) {
        // Normaliza a linha.
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue; // Pula linhas vazias.

        // Lógica de máquina de estados para mudar a seção de leitura.
        if (strcmp(line, "[PONTOS]") == 0)      { section = 1; continue; }
        if (strcmp(line, "[LINHAS]") == 0)      { section = 2; continue; }
        if (strcmp(line, "[POLIGONOS]") == 0)   { section = 3; continue; }
        if (strstr(line, "Cont:") != NULL) continue;

        // Finaliza o polígono anterior antes de ler um novo objeto
        if (section != 3 && buildingPoly && tempPoly.numVertices > 0) {
            GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
            *poly = tempPoly;
            addObject(OBJECT_TYPE_POLYGON, poly);
            buildingPoly = 0;
        }

        // 4. Interpreta a linha com base na seção atual.
        if (section == 1) { // Seção de PONTOS
            float x, y;
            int idxDummy;
            if (sscanf(line, "P%d: %f, %f", &idxDummy, &x, &y) == 3) {
                Point* p = (Point*)malloc(sizeof(Point));
                *p = createPoint(x, y);
                addObject(OBJECT_TYPE_POINT, p);
            }
        } else if (section == 2) { // Seção de LINHAS
            float x1, y1, x2, y2;
            int idxDummy;
            if (sscanf(line, "L%d: (%f, %f) -> (%f, %f)", &idxDummy, &x1, &y1, &x2, &y2) == 5) {
                Segment* s = (Segment*)malloc(sizeof(Segment));
                *s = createSegment((Point){x1, y1}, (Point){x2, y2});
                addObject(OBJECT_TYPE_SEGMENT, s);
            }
        } else if (section == 3) { // Seção de POLÍGONOS
            // Lógica para detectar o início de um polígono ou um novo vértice.
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
                            printf("[AVISO] Vértice ignorado: excedeu MAX_POLYGON_VERTICES.\n");
                        }
                    }
                }
            }
        }
    }

    // 5. Finaliza o último polígono se o arquivo terminou durante sua leitura.
    if (buildingPoly && tempPoly.numVertices > 0) {
        GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
        *poly = tempPoly;
        addObject(OBJECT_TYPE_POLYGON, poly);
    }

    // 6. Fecha o arquivo.
    fclose(f);
}
