#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
// Fonction pour vérifier si un caractère est un opérateur
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Fonction pour donner la priorité aux opérateurs
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Fonction pour effectuer une opération
int applyOp(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }
    return 0;
}

// Fonction pour convertir un caractère de base 26 en entier de base 10
int charToBase26(char c) {
    if (isdigit(c)) return c - '0';
    if (isalpha(c)) return toupper(c) - 'A' + 10;
    return 0;
}

// Fonction pour convertir une chaîne en entier de base 10
int stringToBase10(const char* str) {
    int num = 0;
    while (*str) {
        num = num * 10 + charToBase26(*str);
        str++;
    }
    return num;
}

// Fonction pour convertir un nombre de base 10 en base 26
char* toBase26(int num) {
    static char result[100];
    char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int index = 0;

    do {
        result[index++] = chars[num % 26];
        num /= 26;
       } 
    while (num > 0);
    result[index] = '\0';

    // Inverser la chaîne de caractères
    for (int i = 0; i < index / 2; i++) {
        char temp = result[i];
        result[i] = result[index - 1 - i];
        result[index - 1 - i] = temp;
    }
    return result;
}
// Fonction pour évaluer une expression en base 26
int evaluateBase26(const char* tokens) {
    int i;
    int values[100];
    char ops[100];
    int valIndex = -1;
    int opIndex = -1;

    for (i = 0; i < strlen(tokens); i++) {
        if (tokens[i] == ' ') continue;
        if (isalnum(tokens[i])) {
            char temp[100];
            int j = 0;
            while (i < strlen(tokens) && isalnum(tokens[i])) {
                temp[j++] = tokens[i++];
            }
            temp[j] = '\0';
            values[++valIndex] = stringToBase10(temp);
            i--;
        }
        else if (tokens[i] == '(') {
            ops[++opIndex] = tokens[i];
        }
        else if (tokens[i] == ')') {
            while (opIndex != -1 && ops[opIndex] != '(') {
                int val2 = values[valIndex--];
                int val1 = values[valIndex--];
                char op = ops[opIndex--];
                values[++valIndex] = applyOp(val1, val2, op);
            }
            opIndex--;  // remove '('
        }
        else if (isOperator(tokens[i])) {
            while (opIndex != -1 && precedence(ops[opIndex]) >= precedence(tokens[i])) {
                int val2 = values[valIndex--];
                int val1 = values[valIndex--];
                char op = ops[opIndex--];
                values[++valIndex] = applyOp(val1, val2, op);
            }
            ops[++opIndex] = tokens[i];
        }

    while (opIndex != -1) {
        int val2 = values[valIndex--];
        int val1 = values[valIndex--];
        char op = ops[opIndex--];
        values[++valIndex] = applyOp(val1, val2, op);
    }

    return values[valIndex];
}

// Fonction principale
int main() {
    char expression[100];
    while (1) {
        printf("Entrez l'expression en base 26 (ou 'exit' pour quitter): ");
        fgets(expression, 100, stdin);

        // Supprimer le '\n' à la fin de l'entrée
        expression[strcspn(expression, "\n")] = '\0';

        if (strcmp(expression, "exit") == 0) break;

        int resultBase10 = evaluateBase26(expression);
        printf("Résultat en base 10: %d\n", resultBase10);
        printf("Résultat en base 26: %s\n", toBase26(resultBase10));
    }
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
