#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Fonction pour convertir une chaîne en base 26 (ex. "AB") vers un entier (base 10)
int base26_to_base10(const char* num) {
    int result = 0;
    int is_negative = 0;

    // Vérifie si le nombre est négatif
    if (num[0] == '-') {
        is_negative = 1;
        num++;
    }

    for (int i = 0; num[i] != '\0'; i++) {
        if (!isalpha(num[i])) {
            printf("Erreur : caractère invalide détecté (%c).\n", num[i]);
            exit(1);
        }
        result = result * 26 + (toupper(num[i]) - 'A');
    }

    return is_negative ? -result : result;
}

// Fonction pour convertir un entier (base 10) en base 26 (chaîne)
void base10_to_base26(int num, char* result) {
    int is_negative = 0;
    int index = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        result[index++] = 'A' + (num % 26);
        num /= 26;
    } while (num > 0);

    if (is_negative) {
        result[index++] = '-';
    }

    result[index] = '\0';

    // Inverse la chaîne pour un affichage correct
    for (int i = 0; i < index / 2; i++) {
        char temp = result[i];
        result[i] = result[index - 1 - i];
        result[index - 1 - i] = temp;
    }
}

// Fonction pour appliquer une opération mathématique
int apply_operation(int num1, int num2, char op) {
    switch (op) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/':
        if (num2 == 0) {
            printf("Erreur : division par zéro.\n");
            exit(1);
        }
        return num1 / num2;
    case '%':
        if (num2 == 0) {
            printf("Erreur : division par zéro.\n");
            exit(1);
        }
        return num1 % num2;
    case '^': return pow(num1, num2);
    default:
        printf("Erreur : opération non supportée (%c).\n", op);
        exit(1);
    }
}

// Analyseur d'expressions : gestion des priorités et des parenthèses
int evaluate_expression(char** expr);

// Fonction pour analyser un facteur (un nombre ou une sous-expression entre parenthèses)
int parse_factor(char** expr) {
    char buffer[100];
    int index = 0;

    // Ignorer les espaces
    while (**expr == ' ') (*expr)++;

    if (**expr == '(') {
        (*expr)++; // Ouvre la parenthèse
        int result = evaluate_expression(expr);
        if (**expr == ')') {
            (*expr)++; // Ferme la parenthèse
        }
        else {
            printf("Erreur : parenthèse fermante manquante.\n");
            exit(1);
        }
        return result;
    }

    // Lire un nombre ou une variable
    while (isalpha(**expr) || **expr == '-') {
        buffer[index++] = **expr;
        (*expr)++;
    }
    buffer[index] = '\0';

    // Convertir en base 10
    return base26_to_base10(buffer);
}

// Fonction pour analyser un terme (gère *, /, %)
int parse_term(char** expr) {
    int result = parse_factor(expr);

    while (**expr == '*' || **expr == '/' || **expr == '%') {
        char op = **expr;
        (*expr)++; // Passe à l'opérateur suivant
        int next = parse_factor(expr);
        result = apply_operation(result, next, op);
    }

    return result;
}

// Fonction principale pour analyser une expression complète (gère +, -)
int evaluate_expression(char** expr) {
    int result = parse_term(expr);

    while (**expr == '+' || **expr == '-') {
        char op = **expr;
        (*expr)++; // Passe à l'opérateur suivant
        int next = parse_term(expr);
        result = apply_operation(result, next, op);
    }

    return result;
}

// Fonction principale
int main() {
    char expression[256];
    char result_base26[100];

    printf("=== Calculatrice Universelle en Base 26 ===\n");
    printf("Entrez une expression (ex : AB + C * (D - E)) : ");
    fgets(expression, sizeof(expression), stdin);

    // Supprimer le saut de ligne final
    expression[strcspn(expression, "\n")] = '\0';

    char* expr_ptr = expression;

    // Évaluer l'expression
    int result_base10 = evaluate_expression(&expr_ptr);

    // Convertir le résultat en base 26
    base10_to_base26(result_base10, result_base26);

    // Afficher le résultat
    printf("Résultat : %s (en base 26)\n", result_base26);

    return 0;
}
