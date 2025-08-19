// file_io.c
// Implementação das funções de salvamento e carregamento de arquivos.

#include "file_io.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h>

void saveSceneToFile(const char* filename) {
    // TODO: Implementar a lógica de salvamento.
    // 1. Abrir o arquivo no modo de escrita ("w").
     FILE* f = fopen(filename, "w");
    if (!f) {
        printf("[ERRO] Nao foi possivel abrir '%s' para escrita.\n", filename);
        return;
    }

    // 2. Percorrer o array g_objects.
    int pointCount = 0, lineCount = 0, polyCount = 0;
    for (int i = 0; i < g_numObjects; i++) {
        switch (g_objects[i].type) {
            case OBJECT_TYPE_POINT:   pointCount++; break;
            case OBJECT_TYPE_SEGMENT: lineCount++;  break;
            case OBJECT_TYPE_POLYGON: polyCount++;  break;
        }
    }
    // 3. Para cada objeto, usar um switch no tipo e escrever seus dados
    //    formatados no arquivo com fprintf.
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

    // 3) [LINHAS]
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

    // 4) [POLIGONOS]
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
    // 4. Fechar o arquivo.
    fclose(f);
    printf("Função saveSceneToFile chamada com o nome: %s\n", filename);
}

void loadSceneFromFile(const char* filename) {
    // TODO: Implementar a lógica de carregamento.

    // 2. Abrir o arquivo no modo de leitura ("r").
    // 3. Ler o arquivo linha por linha (ou dado por dado) com fscanf.
    // 4. Para cada linha/objeto, identificar o tipo, alocar memória (malloc),
    //    criar o objeto e adicioná-lo à cena com addObject().
    // 5. Fechar o arquivo.
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("[ERRO] Não foi possível abrir '%s' para leitura.\n", filename);
        return;
    }

    // 1. Limpar a cena atual com clearAllObjects().
    clearAllObjects();

    char line[256];
    int section = 0; // 1=PONTOS, 2=LINHAS, 3=POLIGONOS

    // Controle de polígono em construção
    int buildingPoly = 0;
    GfxPolygon tempPoly;

    while (fgets(line, sizeof(line), f)) {
        // remove \r\n
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;       // pula linhas vazias

        // troca de seção
        if (strcmp(line, "[PONTOS]") == 0)   { section = 1; continue; }
        if (strcmp(line, "[LINHAS]") == 0)   { section = 2; continue; }
        if (strcmp(line, "[POLIGONOS]") == 0){ section = 3; continue; }

        // ignora linhas "Cont: X"
        if (strstr(line, "Cont:") != NULL) continue;

        if (section == 1) {
            // Pk: x, y
            float x, y;
            int idxDummy;
            if (sscanf(line, "P%d: %f, %f", &idxDummy, &x, &y) == 3) {
                Point* p = (Point*)malloc(sizeof(Point));
                *p = createPoint(x, y);
                addObject(OBJECT_TYPE_POINT, p);
            }
        } else if (section == 2) {
            // Lk: (x1, y1) -> (x2, y2)
            float x1, y1, x2, y2;
            int idxDummy;
            if (sscanf(line, "L%d: (%f, %f) -> (%f, %f)", &idxDummy, &x1, &y1, &x2, &y2) == 5) {
                Segment* s = (Segment*)malloc(sizeof(Segment));
                *s = createSegment((Point){x1, y1}, (Point){x2, y2});
                addObject(OBJECT_TYPE_SEGMENT, s);
            }
        } else if (section == 3) {
            // Polígono ou vértice
            if (strncmp(line, "Poligono", 8) == 0) {
                // Se já estávamos montando um polígono anterior, finalize-o
                if (buildingPoly && tempPoly.numVertices > 0) {
                    GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
                    *poly = tempPoly;
                    addObject(OBJECT_TYPE_POLYGON, poly);
                    buildingPoly = 0;
                }
                // Inicia novo polígono
                int polyIdx, numPts; // numPts é informativo; vamos confiar nas linhas Vx
                if (sscanf(line, "Poligono %d - Pontas: %d", &polyIdx, &numPts) >= 1) {
                    tempPoly = createPolygon();
                    buildingPoly = 1;
                }
            } else {
                // Vértice: "  Vk: x, y"
                float vx, vy;
                int vIdxDummy;
                if (sscanf(line, "  V%d: %f, %f", &vIdxDummy, &vx, &vy) == 3) {
                    if (buildingPoly) {
                        if (tempPoly.numVertices < MAX_POLYGON_VERTICES) {
                            tempPoly.vertices[tempPoly.numVertices++] = (Point){vx, vy};
                        } else {
                            printf("[AVISO] Vértice ignorado; excedeu MAX_POLYGON_VERTICES.\n");
                        }
                    }
                }
            }
        }
    }

    // Fecha e adiciona o último polígono se estava em construção
    if (buildingPoly && tempPoly.numVertices > 0) {
        GfxPolygon* poly = (GfxPolygon*)malloc(sizeof(GfxPolygon));
        *poly = tempPoly;
        addObject(OBJECT_TYPE_POLYGON, poly);
    }

    fclose(f);

    printf("Função loadSceneFromFile chamada com o nome: %s\n", filename);
}
