#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double readDouble() {
    char string[50];
    char *endptr;

    fgets(string, 50, stdin);

    double output = strtod(string, &endptr);

    if (endptr != string && *endptr == '\n')
        return output;
    else {
        fprintf(stderr,"Erro: A string precisa ser do tipo double.\n");
        return readDouble();
    }
}

float readFloat() {
    char string[50];
    char *endptr;

    fgets(string, 50, stdin);

    float output = strtof(string, &endptr);

    if (endptr != string && *endptr == '\n')
        return output;
    else {
        fprintf(stderr,"Erro: A string precisa ser do tipo float.\n");
        return readFloat();
    }

}

int readInteger() {
    char input[50];
    char *endptr;

    fgets(input, 50, stdin);

    long output = strtol(input, &endptr, 10);

    if (endptr != input && (*endptr == '\n' || *endptr == '\0')) {
        return (int)output;
    } else {
        fprintf(stderr,"Erro: A entrada precisa ser do tipo inteiro.\n");
        return readInteger();
    }
}

void removeNewLine(char *string) {
    if (string == NULL)
        return;

    size_t len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
}


char readChar() {
    char input[50];

    fgets(input, 50, stdin);

    if (strlen(input) == 2 && input[1] == '\n') {
        return input[0];
    } else {
        fprintf(stderr,"Erro: A entrada precisa ser do tipo char.\n");
        return readChar();
    }
}

void readString(char *string, int maxLength) {
    fgets(string, maxLength, stdin);
    removeNewLine(string);
}

int minimumOfTwo(int x, int y) {
    return (x < y) ? x : y;
}

int calculateStringSimilarity(const char *string1, const char *string2) {
    int string1Len = strlen(string1);
    int string2Len = strlen(string2);

    if (string1Len == 0) return string2Len;
    if (string2Len == 0) return string1Len;

    int *distanceMatrix = malloc((string1Len + 1) * (string2Len + 1) * sizeof(int));
    if (distanceMatrix == NULL) {
        fprintf(stderr,"Erro: Falha ao alocar memória.\n");
        return -1;
    }

    for (int i = 0; i <= string1Len; i++) {
        distanceMatrix[i * (string2Len + 1)] = i;
    }
    for (int j = 0; j <= string2Len; j++) {
        distanceMatrix[j] = j;
    }

    for (int j = 1; j <= string2Len; j++) {
        for (int i = 1; i <= string1Len; i++) {
            int substitutionCost;
            if (string1[i - 1] == string2[j - 1])
                substitutionCost = 0;
            else
                substitutionCost = 1;

            int deletion = distanceMatrix[i * (string2Len + 1) + j - 1] + 1;
            int insertion = distanceMatrix[(i - 1) * (string2Len + 1) + j] + 1;
            int substitution = distanceMatrix[(i - 1) * (string2Len + 1) + j - 1] + substitutionCost;

            distanceMatrix[i * (string2Len + 1) + j] = minimumOfTwo(minimumOfTwo(deletion, insertion), substitution);
        }
    }

    int result = distanceMatrix[string1Len * (string2Len + 1) + string2Len];
    free(distanceMatrix);
    return result;
}

char* suggestName(const char *incorrectName, const char **existingNames, int numNames) {
    int bestDistance = -1;
    const char *bestSuggestion = NULL;
    int threshold = 3;

    for (int i = 0; i < numNames; ++i) {
        int distance = calculateStringSimilarity(incorrectName, existingNames[i]);

        if (bestDistance == -1 || distance < bestDistance) {
            bestDistance = distance;
            bestSuggestion = existingNames[i];
        }

        if (distance < threshold) break;
    }

    if (bestSuggestion != NULL) {
        char *suggestion = strdup(bestSuggestion);
        return suggestion;
    }

    return NULL;
}
