#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_MOTS 10000
#define TAILLE_MOT 6

// Structure pour stocker les mots
typedef struct {
    char mot[TAILLE_MOT];
} Mot;

// Charger les mots depuis le fichier bdd_wordle.txt
int charger_mots(Mot mots[]) {
    FILE *fichier = fopen("ressource/bdd_wordle.txt", "r");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier base des donnees.\n");
        exit(EXIT_FAILURE);
    }

    int nb_mots = 0;
    while (fgets(mots[nb_mots].mot, TAILLE_MOT, fichier) != NULL && nb_mots < MAX_MOTS) {
        // Supprimer le caractere de saut de ligne a la fin du mot
        mots[nb_mots].mot[strcspn(mots[nb_mots].mot, "\n")] = '\0';
        
        // Verifier si le mot a exactement 5 caracteres et ne contient pas d'espace
        bool mot_valide = true;
        for (int i = 0; i < strlen(mots[nb_mots].mot); i++) {
            if (mots[nb_mots].mot[i] == ' ') {
                mot_valide = false;
                break;
            }
        }
        if (strlen(mots[nb_mots].mot) == 5 && mot_valide) {
            nb_mots++;
        }
    }

    fclose(fichier);
    return nb_mots;
}

// Verifier si le mot est correct (5 lettres)
bool verifier_mot(const char mot[]) {
    return strlen(mot) == 5;
}

// Comparer le mot proposee avec le mot A trouver
void comparer_mots(const char mot_a_trouver[], const char mot_propose[]) {
    int positions_correctes[5] = {0}; // Tableau pour marquer les positions correctes
    int positions_bien_placees[5] = {0}; // Tableau pour marquer les positions bien placées
    
    // Recherche des lettres bien placées et des lettres correctes
    for (int i = 0; i < 5; i++) {
        if (mot_a_trouver[i] == mot_propose[i]) {
            positions_correctes[i] = 1;
            positions_bien_placees[i] = 1;
        } else {
            for (int j = 0; j < 5; j++) {
                if (i != j && mot_a_trouver[i] == mot_propose[j] && !positions_correctes[j] && !positions_bien_placees[i]) {
                    positions_correctes[j] = 1;
                    break;
                }
            }
        }
    }

    // Affichage des positions des lettres correctes
    printf("Positions des lettres correctes : ");
    for (int i = 0; i < 5; i++) {
        if (positions_correctes[i]) {
            printf("%d ", i + 1);
        }
    }
    printf("\n");

    printf("Positions des lettres bien placees : ");
    for (int i = 0; i < 5; i++) {
        if (positions_bien_placees[i]) {
            printf("%d ", i + 1);
        }
    }
    printf("\n");
}



// Filtrer les mots qui contiennent une lettre spÃ©cifique
void filtrer_par_lettre(Mot mots[], int nb_mots, char lettre) {
    for (int i = 0; i < nb_mots; i++) {
        if (strchr(mots[i].mot, lettre) != NULL) {
            printf("%s\n", mots[i].mot);
        }
    }
}

int main() {
    Mot mots[MAX_MOTS];
    int nb_mots = charger_mots(mots);

    // Choisir un mot alÃ©atoire parmi les mots chargÃ©s
    srand(time(NULL));
    int index_mot_a_trouver = rand() % nb_mots;
    const char *mot_a_trouver = mots[index_mot_a_trouver].mot;

    printf("Bienvenue dans le jeu Wordle!\n");
    printf("Vous avez 6 propositions pour deviner un mot de 5 lettres.\n");

    char lettre_filtre;
    printf("Voulez-vous filtrer les mots par une lettre specifique ? (O/N) : ");
    char choix_filtre;
    scanf(" %c", &choix_filtre);

    if (choix_filtre == 'O' || choix_filtre == 'o') {
        printf("Entrez la lettre pour filtrer les mots : ");
        scanf(" %c", &lettre_filtre);
        printf("Mots filtrees par la lettre '%c' :\n", lettre_filtre);
        filtrer_par_lettre(mots, nb_mots, lettre_filtre);
    } else {
        printf("Tous les mots sont affiches :\n");
        for (int i = 0; i < nb_mots; i++) {
            printf("%s\n", mots[i].mot);
        }
    }

    // Suite du jeu : propositions du joueur
    for (int essai = 1; essai <= 6; essai++) {
        char mot_propose[TAILLE_MOT];
        printf("\nEssai %d : Entrez votre proposition de mot : ", essai);
        scanf("%s", mot_propose);

        if (!verifier_mot(mot_propose)) {
            printf("Erreur : Veuillez entrer un mot de 5 lettres.\n");
            continue;
        }

        comparer_mots(mot_a_trouver, mot_propose);

        if (strcmp(mot_a_trouver, mot_propose) == 0) {
            printf("Felicitations! Vous avez trouvee le mot %s en %d essais.\n", mot_a_trouver, essai);
            return 0;
        }
    }

    printf("Desolee, vous n'avez pas trouvee le mot. Le mot A trouver etait : %s\n", mot_a_trouver);
    return 0;
}
