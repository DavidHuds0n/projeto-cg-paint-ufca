/**
 * @file file_io.h
 * @brief Define a interface para as funções de entrada e saída de arquivos.
 *
 * Módulo responsável por salvar o estado da cena em um arquivo e carregá-lo,
 * garantindo a persistência dos dados entre as sessões.
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdlib.h>
#include <stdio.h>

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

/**
 * @brief Salva todos os objetos da cena atual em um arquivo de texto.
 *
 * O arquivo será formatado com seções para cada tipo de objeto (pontos,
 * linhas e polígonos), facilitando a leitura.
 * @param filename O nome do arquivo onde a cena será salva (ex: "meu_desenho.txt").
 */
void saveSceneToFile(const char* filename);

/**
 * @brief Limpa a cena atual e carrega os objetos de um arquivo de texto.
 *
 * A função utiliza uma lógica de máquina de estados para interpretar o arquivo
 * e recriar os objetos na memória.
 * @param filename O nome do arquivo de onde a cena será carregada.
 */
void loadSceneFromFile(const char* filename);

#endif // FILE_IO_H
