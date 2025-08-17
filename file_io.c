// file_io.c
// Implementa��o das fun��es de salvamento e carregamento de arquivos.

#include "file_io.h"
#include "objects.h"
#include <stdio.h>

void saveSceneToFile(const char* filename) {
    // TODO: Implementar a l�gica de salvamento.
    // 1. Abrir o arquivo no modo de escrita ("w").
    // 2. Percorrer o array g_objects.
    // 3. Para cada objeto, usar um switch no tipo e escrever seus dados
    //    formatados no arquivo com fprintf.
    // 4. Fechar o arquivo.
    printf("Fun��o saveSceneToFile chamada com o nome: %s\n", filename);
}

void loadSceneFromFile(const char* filename) {
    // TODO: Implementar a l�gica de carregamento.
    // 1. Limpar a cena atual com clearAllObjects().
    // 2. Abrir o arquivo no modo de leitura ("r").
    // 3. Ler o arquivo linha por linha (ou dado por dado) com fscanf.
    // 4. Para cada linha/objeto, identificar o tipo, alocar mem�ria (malloc),
    //    criar o objeto e adicion�-lo � cena com addObject().
    // 5. Fechar o arquivo.
    printf("Fun��o loadSceneFromFile chamada com o nome: %s\n", filename);
}
