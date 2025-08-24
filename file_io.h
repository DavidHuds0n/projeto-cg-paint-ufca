/**
 * @file file_io.h
 * @brief Define a interface para as fun��es de entrada e sa�da de arquivos.
 *
 * M�dulo respons�vel por salvar o estado da cena em um arquivo e carreg�-lo,
 * garantindo a persist�ncia dos dados entre as sess�es.
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdlib.h>
#include <stdio.h>

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

/**
 * @brief Salva todos os objetos da cena atual em um arquivo de texto.
 *
 * O arquivo ser� formatado com se��es para cada tipo de objeto (pontos,
 * linhas e pol�gonos), facilitando a leitura.
 * @param filename O nome do arquivo onde a cena ser� salva (ex: "meu_desenho.txt").
 */
void saveSceneToFile(const char* filename);

/**
 * @brief Limpa a cena atual e carrega os objetos de um arquivo de texto.
 *
 * A fun��o utiliza uma l�gica de m�quina de estados para interpretar o arquivo
 * e recriar os objetos na mem�ria.
 * @param filename O nome do arquivo de onde a cena ser� carregada.
 */
void loadSceneFromFile(const char* filename);

#endif // FILE_IO_H
