// file_io.h
// Define a interface para as fun��es de entrada e sa�da de arquivos.
// Respons�vel por salvar o estado da cena em um arquivo e carreg�-lo de volta.

#ifndef FILE_IO_H
#define FILE_IO_H

/**
 * @brief Salva todos os objetos da cena atual em um arquivo de texto.
 * @param filename O nome do arquivo onde a cena ser� salva (ex: "meu_desenho.txt").
 */
void saveSceneToFile(const char* filename);

/**
 * @brief Limpa a cena atual e carrega os objetos de um arquivo de texto.
 * @param filename O nome do arquivo de onde a cena ser� carregada.
 */
void loadSceneFromFile(const char* filename);

#endif // FILE_IO_H
