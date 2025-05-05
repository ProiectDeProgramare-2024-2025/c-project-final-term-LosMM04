#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET       "\033[0m"
#define PORTOCALIU  "\033[33m"
#define MOV         "\033[35m"
#define VERDE       "\033[32m"
#define ALBASTRU    "\033[36m"


typedef struct {
    char nume[50];
    int cantitate;
    float pret;
} Produs;

void afisare_header(int zi, int luna, int an) {
    printf("Data selectata: " PORTOCALIU "%02d-%02d-%04d" RESET "\n\n", zi, luna, an);
}


void inregistrare_vanzare(int zi, int luna, int an) {
    char filename[20];
    sprintf(filename, "%02d-%02d-%04d.txt", zi, luna, an);

    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        system("cls");
        afisare_header(zi, luna, an);
        printf("Nu poti deschide aceasta data\n");
        system("pause");
        return;
    }

    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        fprintf(file, "=== VANZARI ===\n");
    }

    Produs produs;
    int optiune;
    do {
        system("cls");
        afisare_header(zi, luna, an);
        printf("1. Introduceti produsul\n");
        printf("2. Revenire la meniul anterior\n");
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &optiune);
        getchar();

        if (optiune == 1) {
            system("cls");
            afisare_header(zi, luna, an);
            printf("Introduceti numele produsului: ");
            fgets(produs.nume, sizeof(produs.nume), stdin);
            produs.nume[strcspn(produs.nume, "\n")] = '\0';

            printf("Introduceti cantitatea: ");
            scanf("%d", &produs.cantitate);
            printf("Introduceti pretul: ");
            scanf("%f", &produs.pret);

            fprintf(file, "%s\n%d\n%.2f\n", produs.nume, produs.cantitate, produs.pret);

            system("cls");
            afisare_header(zi, luna, an);
            printf("Vanzare inregistrata cu succes!\n\n");
            system("pause");
        }
    } while (optiune != 2);

    fclose(file);
}

void vizualizare_istoric(int zi, int luna, int an) {
    char filename[20];
    sprintf(filename, "%02d-%02d-%04d.txt", zi, luna, an);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        system("cls");
        afisare_header(zi, luna, an);
        printf("Nu exista inregistrari pentru aceasta data.\n");
        system("pause");
        return;
    }

    system("cls");
    afisare_header(zi, luna, an);

    char line[100];
    fgets(line, sizeof(line), file);
    printf("%s", line);

    int counter = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        counter++;
        if (counter == 1) { // produs
            printf(ALBASTRU "%s" RESET "\n", line);
        } else if (counter == 2) { // cantitate
            printf(MOV "%s bucati" RESET "\n", line);
        } else if (counter == 3) { // pret
            printf(VERDE "%s lei" RESET "\n", line);
            counter = 0;
        }
    }
    fclose(file);

    printf("\n");
    system("pause");
}


void calculare_profit(int zi, int luna, int an) {
    char filename[20];
    sprintf(filename, "%02d-%02d-%04d.txt", zi, luna, an);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        system("cls");
        afisare_header(zi, luna, an);
        printf("Nu exista inregistrari pentru aceasta data.\n");
        system("pause");
        return;
    }

    Produs produs;
    char line[100];
    float profit_total = 0, profit_maxim = 0;
    char produs_maxim[50] = "";
    int cantitate_maxima = 0;
    float pret_maxim = 0;

    fgets(line, sizeof(line), file);

    while (fgets(produs.nume, sizeof(produs.nume), file)) {
        produs.nume[strcspn(produs.nume, "\n")] = '\0';
        fgets(line, sizeof(line), file);
        produs.cantitate = atoi(line);
        fgets(line, sizeof(line), file);
        produs.pret = atof(line);

        float venit = produs.pret * produs.cantitate;
        profit_total += venit;

        if (venit > profit_maxim) {
            profit_maxim = venit;
            strcpy(produs_maxim, produs.nume);
            cantitate_maxima = produs.cantitate;
            pret_maxim = produs.pret;
        }
    }

    fclose(file);

    system("cls");
    printf("Profitul total pentru data " PORTOCALIU "%02d-%02d-%04d" RESET " este: " VERDE "%.2f lei" RESET "\n", zi, luna, an, profit_total);
    printf("Cea mai mare vanzare: " ALBASTRU "%s" RESET ", " MOV "%d bucati" RESET " la pretul de " VERDE "%.2f lei" RESET " fiecare, cu un total de " VERDE "%.2f lei" RESET "\n",
       produs_maxim, cantitate_maxima, pret_maxim, profit_maxim);



    printf("1. Revenire la meniul anterior\n");
    int optiune;
    do {
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &optiune);
    } while (optiune != 1);
}

int main() {
    int choice, zi, luna, an;

    while (1) {
        system("cls");
        printf("-= Sistem Inregistrare Vanzari Fast-Food =-\n");
        printf("1. Inregistrare vanzare\n");
        printf("2. Vizualizare istoric vanzari\n");
        printf("3. Calculare profit si produs maxim\n");
        printf("4. Iesire\n");

        do {
            printf("Introduceti optiunea dorita: ");
            scanf("%d", &choice);
        } while (choice < 1 || choice > 4);

        if (choice == 4) {
            system("cls");
            printf("Iesire din aplicatie...\n");
            break;
        }

        system("cls");
        printf("Introduceti data (DD MM YYYY): ");
        scanf("%d %d %d", &zi, &luna, &an);

        if (choice == 1) {
            inregistrare_vanzare(zi, luna, an);
        } else if (choice == 2) {
            vizualizare_istoric(zi, luna, an);
        } else if (choice == 3) {
            calculare_profit(zi, luna, an);
        }
    }

    return 0;
}