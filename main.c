#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void spaces(char **p) {
    while (isspace((unsigned char)**p)) (*p)++;
}

double expression(char **p, int *error);

double number(char **p, int *error) {
    spaces(p);

    if (**p == '+') {
        (*p)++;
        return number(p, error);
    }

    if (**p == '-') {
        (*p)++;
        return -number(p, error);
    }

    if (**p == '(') {
        (*p)++;
        double result = expression(p, error);
        spaces(p);

        if (**p != ')') {
            *error = 1;
            return 0;
        }

        (*p)++;
        return result;
    }

    char *end;
    double value = strtod(*p, &end);

    if (end == *p) {
        *error = 1;
        return 0;
    }

    *p = end;
    return value;
}

double term(char **p, int *error) {
    double result = number(p, error);

    while (!*error) {
        spaces(p);

        char op = **p;
        if (op != '*' && op != '/') break;

        (*p)++;
        double next = number(p, error);

        if (*error) break;

        if (op == '*') {
            result *= next;
        } else {
            if (next == 0) {
                *error = 2;
                return 0;
            }
            result /= next;
        }
    }

    return result;
}

double expression(char **p, int *error) {
    double result = term(p, error);

    while (!*error) {
        spaces(p);

        char op = **p;
        if (op != '+' && op != '-') break;

        (*p)++;
        double next = term(p, error);

        if (*error) break;

        if (op == '+') result += next;
        else result -= next;
    }

    return result;
}

int main(void) {
    char input[512];

    printf("============================\n");
    printf("       C CALCULATOR\n");
    printf("============================\n");

    while (1) {
        printf("\nMath: ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;

        if (strcmp(input, "q\n") == 0 ||
            strcmp(input, "q") == 0) {
            break;
        }

        char *p = input;
        int error = 0;

        double result = expression(&p, &error);
        spaces(&p);

        if (error == 2) {
            printf("Cannot divide by zero.\n");
        } else if (error || *p != '\0') {
            printf("Invalid equation. Try again.\n");
        } else {
            printf("%.10g\n", result);
        }
    }

    printf("\nCalculator closed.\n");
    return 0;
}
