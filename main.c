#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_VERTICES 10000
#define DENOMINATOR 2
#define ODD_NUMBER 1

typedef struct {
    int vertex;
    int degree;
} Vertex;


Vertex **countSort(Vertex **oddDegreeVertices, int numOddDegreeVertices) {

    Vertex **output = (Vertex **) calloc(numOddDegreeVertices, sizeof(Vertex *));
    for (int i = 0; i < numOddDegreeVertices; i++) {
        output[i] = (Vertex *) calloc(1, sizeof(Vertex));
    }

    int count[MAX_VERTICES + 1], i;
    memset(count, 0, sizeof(count));

    for (i = 0; i < numOddDegreeVertices; ++i)
        ++count[oddDegreeVertices[i]->degree];

    for (i = 1; i <= MAX_VERTICES; ++i)
        count[i] += count[i - 1];

    for (i = 0; i < numOddDegreeVertices; ++i) {
        output[numOddDegreeVertices - count[oddDegreeVertices[i]->degree]] = oddDegreeVertices[i];
        --count[oddDegreeVertices[i]->degree];
    }

    return output;
}


int main(int argc, char **argv) {

    FILE *fp = fopen("../graph.txt", "rt");

    int line = atoi(argv[1]), column = atoi(argv[1]);

    int **adjacencyMatrix = (int **) calloc(MAX_VERTICES, sizeof(int *));
    int *degrees = (int *) calloc(MAX_VERTICES, sizeof(int));
    for (int i = 0; i < MAX_VERTICES; i++) {
        adjacencyMatrix[i] = (int *) calloc(MAX_VERTICES, sizeof(int));
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *) malloc(file_size * sizeof(char));
    int i = 0, j = 0;
    char *record;
    while ((fgets(buffer, file_size, fp)) != NULL) {
        record = strtok(buffer, ",");
        while (record != NULL) {
            adjacencyMatrix[i][j] = (int) atoi(record);
            record = strtok(NULL, ",");
            j++;
        }
        ++i;
        j = 0;
    }
    free(buffer);
    fclose(fp);

    for (i = 0; i < column; i++) {
        for (j = 0; j < line; j++){
            if (adjacencyMatrix[i][j] == 1) {
                degrees[i]++;
            }
        }
    }

    Vertex **oddDegreeVertices = (Vertex **) calloc(MAX_VERTICES, sizeof(Vertex *));
    for (i = 0; i < MAX_VERTICES; i++) {
        oddDegreeVertices[i] = (Vertex *) calloc(1, sizeof(Vertex));
    }

    int numOddDegreeVertices = 0;
    for (i = 0; i < column; i++) {
        if (degrees[i] % DENOMINATOR == ODD_NUMBER) {
            oddDegreeVertices[numOddDegreeVertices]->vertex = i;
            oddDegreeVertices[numOddDegreeVertices]->degree = degrees[i];
            numOddDegreeVertices++;
        }
    }
    int startTime = clock();
    Vertex **sortedVertices = countSort(oddDegreeVertices, numOddDegreeVertices);
    int endTime = clock();
    printf("startTime -> %d msec\n", startTime);
    printf("endTime -> %d msec\n", endTime);
    printf("Sorting time -> %d msec\n", endTime - startTime);

    fp = fopen("../result", "wt");
    fprintf(fp, "%d -> %d\n", line, endTime - startTime);
    fclose(fp);

    puts("Odd-degree vertices in descending order of degree:");
    for (i = 0; i < numOddDegreeVertices; i++) {
        if (sortedVertices[i]->degree != 0) {
            printf("%d (degree %d)\n", sortedVertices[i]->vertex + 1, sortedVertices[i]->degree);
        }

    }

    for (i = 0; i < MAX_VERTICES; i++) {
        free(adjacencyMatrix[i]);
    }
    free(adjacencyMatrix);
    free(degrees);

    for (i = 0; i < MAX_VERTICES; i++) {
        free(oddDegreeVertices[i]);
    }
    free(oddDegreeVertices);

    for (i = 0; i < numOddDegreeVertices; i++) {
        free(sortedVertices[i]);
    }
    free(sortedVertices);
    return 0;
}