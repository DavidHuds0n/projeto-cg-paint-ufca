// file_io.h
// Define a interface para as funções de entrada e saída de arquivos.
// Responsável por salvar o estado da cena em um arquivo e carregá-lo de volta.

#ifndef FILE_IO_H
#define FILE_IO_H

/**
 * @brief Salva todos os objetos da cena atual em um arquivo de texto.
 * @param filename O nome do arquivo onde a cena será salva (ex: "meu_desenho.txt").
 */
void saveSceneToFile(const char* filename);

/**
 * @brief Limpa a cena atual e carrega os objetos de um arquivo de texto.
 * @param filename O nome do arquivo de onde a cena será carregada.
 */
void loadSceneFromFile(const char* filename);

#endif // FILE_IO_H
