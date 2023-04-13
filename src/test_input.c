#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *input_string = malloc(1024 * sizeof(char));
    strcpy(input_string, "print 10;\naddline;\nprint 20;");
    char *line;

    line = strtok(input_string, "\n");

    while (line != NULL) {
        printf("%s\n", line);
        line = strtok(NULL, "\n");
    }

    return 0;
}
