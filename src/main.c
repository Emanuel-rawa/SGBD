#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "database.h"
#include "userInteraction.h"
#include "table.h"
#include "fileOperations.h"
#include "row.h"
void clearTerminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseProgram() {
    printf("Pressione qualquer tecla para continuar...");
    getchar();
}
void initialMenu() {
    printf("   ____   ____ ____  ____    \n");
    printf("  / ___| / ___| __ )|  _ \\  \n");
    printf("  \\___ \\| |  _|  _ \\| | | |  \n");
    printf("   ___) | |_| | |_) | |_| |     \n");
    printf("  |____/ \\____|____/|____/  \n");
    printf("1 - Carregar arquivo\n");
    printf("2 - Criar novo banco de dados\n");
    printf("0 - Sair\n");
}

int mainMenu() {
    printf("-------------------------------------------\n");
    printf("1 - Criar Tabela\n"
           "2 - Deletar Tabela\n"
           "3 - Criar Linha\n"
           "4 - Listar Tabelas\n"
           "5 - Listar Dados\n"
           "6 - Deletar Linha\n"
           "7 - Procurar Linha\n"
           "8 - Salvar Banco de Dados\n"
           "0 - Sair\n");
    printf("-------------------------------------------\n");
    printf("Digite um número correspondente a função desejada: ");
    int input = readInteger();

    return input;
}

int main() {

    while (1) {
        start:
        clearTerminal();
        printf("Bem-vindo ao nosso SGDB!\n");
        initialMenu();
        int choice = readInteger();

        switch (choice) {
            case 1:
                while (1) {
                    char* filename = readFileName();
                    if (filename == NULL) {
                        goto start;
                    }
                    if (loadFile(filename)) {
                        free(filename);
                        break;
                    } else {
                        free(filename);
                        printf("Deseja tentar novamente? (s/n): ");
                        char retry = readChar();
                        if (retry == 'n' || retry == 'N') {
                            return 0;
                        }
                    }
                }
                break;
            case 2:
                // Inicialize o banco de dados
                initDatabase();
                break;
            case 0:
                // Saia do programa
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        // Se chegamos aqui, o banco de dados foi inicializado ou um arquivo foi carregado
        // Podemos agora entrar no menu principal
        while (1) {
            clearTerminal();
            int input = mainMenu();
            switch (input) {
                case 1:
                    createTable();
                    pauseProgram();
                    break;
                case 2:
                    deleteTableUserInteraction();
                    pauseProgram();
                    break;
                case 3:
                   createRowUserInteraction();
                   pauseProgram();
                    break;
                case 4:
                    listTables();
                    pauseProgram();
                    break;
                case 5:
                    listTableDataUserInteraction();
                    pauseProgram();
                    break;
                case 6:
                    deleteRowUserInteraction();
                    pauseProgram();
                    break;
                case 7:
                    searchValue();
                    pauseProgram();
                    break;
                case 8: {
                    char* filename = readFileName();
                    if (filename == NULL) {
                        // O usuário decidiu sair
                        break;
                    }
                    saveDatabaseToFile(filename);
                    free(filename);
                    pauseProgram();
                    break;
                }
                case 0:
                    // Saia do programa
                    return 0;
                default:
                    printf("Opção inválida. Tente novamente.\n");
                    break;
            }
        }
    }
}
