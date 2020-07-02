#include<stdio.h>
#include<stdlib.h>

struct produit {
    int code;
    char libel[25];
    float prix;
    int stock;
    float TVA;
};

struct ligne {
    produit p;
};

void showOptions();

void init_Prod(produit *p) {
    printf("Donner le code de produit: ");
    while (scanf("%d", &p->code) != 1) {
        printf("Donner le code de produit: ");
        while (getchar() != '\n');
    }

    printf("Donner le stock de produit: ");
    while (scanf("%d", &p->stock) != 1) {
        printf("Donner le stock de produit: ");
        while (getchar() != '\n');
    }

    printf("Donner la valeur TVA de produit: ");
    while (scanf("%f", &p->TVA) != 1) {
        printf("Donner la valeur TVA de produit: ");
        while (getchar() != '\n');
    }

    printf("Donner le prix de produit: ");
    while (scanf("%f", &p->prix) != 1) {
        printf("Donner le prix de produit: ");
        while (getchar() != '\n');
    }

    printf("Donner la libelle de produit: ");
    scanf("%s", p->libel);
}

int getSizeOfProductsTable(produit *T) {
    int index = 0;
    produit *p = T;
    while (p->code != 0) {
        p = p + 1;
        index++;
    }
    return index;
}

void insertIntoTabProducts(produit *T, int n) {
    int size = getSizeOfProductsTable(T);
    for (int i = size; i < size + n; i++) {
        printf("Produit %d: \n", (i + 1));
        init_Prod(T + i);
    }
}


void Save_Prod(produit *T, int n, FILE *F) {
    F = fopen("Fiche.Produit.txt", "w");

    fprintf(F, "code_P \t\t libelle_P \t\t prix \t\t Qt� \t\t TVA \n");

    for (int i = 0; i < n; i++) {
        //printf("%d  %s %.2f  %d  %.2f \n",(T+i)->code,(T+i)->libel,(T+i)->prix,(T+i)->stock,(T+i)->TVA);getchar();
        fprintf(F, "%d \t\t %s \t\t %.2f \t\t %d \t\t %.2f \n", (T + i)->code, (T + i)->libel, (T + i)->prix,
                (T + i)->stock,
                (T + i)->TVA);
    }
    fclose(F);
}

void init_ligne(produit *T, int n, ligne *l) {
    bool found = false;
    while (true) {
        int code;
        printf("Donner la code de produit: ");
        scanf("%d", &code);

        for (int i = 0; i < n; i++) {
            if ((T + i)->code == code) {
                l->p = *(T + i);
                found = true;
                break;
            }
        }
        if (found) {
            break;
        } else {
            printf("No products found with this code please try again.\n");
        }
    }
}

void init_tab_ligne(produit *T, int n, ligne *facture, int ln) {
    for (int i = 0; i < ln; i++) {
        printf("Produit n°%d:\n", i+1);
        ligne *l = (ligne *) malloc(sizeof(ligne));
        init_ligne(T, n, l);
        (facture + i)->p = l->p;
    }
}

void save_facture(ligne *T, int n, FILE *factF) {
    factF = fopen("Fiche.facture.txt", "w");

    fprintf(factF, "NumL \t\t Code \t\t Label \t\t Stock \t\t PHT \t\t TTC \n");

    for (int i = 0; i < n; i++) {
        float ttc = ((T + i)->p.prix * ((T + i)->p.TVA / 100) + (T + i)->p.prix);
        //printf("%d  %s %.2f  %d  %.2f \n",(T+i)->code,(T+i)->libel,(T+i)->prix,(T+i)->stock,(T+i)->TVA);getchar();
        fprintf(factF, "%d \t\t %d \t\t %s \t\t %d \t\t %.2f \t\t %.2f \n", i + 1, (T + i)->p.code, (T + i)->p.libel,
                (T + i)->p.stock, (T + i)->p.prix, ttc);
    }
    fclose(factF);
}

void clearScreen() {
#ifdef __linux__
    system("clear");
#elif __APPLE__
    system("clear");
#else
    system("cls");
#endif
}

void showOptions() {
    printf("1. Ajouter produits\n"
           "2. Cree une facture\n"
           "3. Quitter\n\n"
           "Votre choix: ");
}

int main() {
    int n;
    FILE *F;
    produit *T = nullptr;

    int ln;
    ligne *facture;

    int choix;
    while (choix != 3) {
        clearScreen();
        showOptions();
        while (scanf("%d", &choix) != 1) {
            clearScreen();
            showOptions();
            while (getchar() != '\n');
        }
        clearScreen();

        switch (choix) {
            case 1:
                printf("Combien de produits tu veux ajouter? \n");
                while (scanf("%d", &n) != 1) {
                    printf("Combien de produits tu veux ajouter? \n");
                    while (getchar() != '\n');
                }

                if (T == nullptr) {
                    T = (produit *) malloc(n * sizeof(produit));
                } else {
                    int size = getSizeOfProductsTable(T);
                    T = (produit *) realloc(T, (size+n) * sizeof(produit));
                }

                printf("\n");

                insertIntoTabProducts(T, n);
                Save_Prod(T, getSizeOfProductsTable(T), F);
                break;
            case 2: {
                printf("Donner le nombre de lignes de facture:");
                scanf("%d", &ln);

                facture = (ligne *) malloc(ln * sizeof(ligne));
                init_tab_ligne(T, getSizeOfProductsTable(T), facture, ln);
                FILE *factF;

                save_facture(facture, ln, factF);
            }
        }
    }

    return 0;
}
