# RSA - Manipulation de grands nombres (EMSE-ISMIN)

Ce projet implémente une structure de données pour manipuler de grands nombres (représentés sous forme de tableaux de `uint32_t`) et fournit des fonctions permettant d'effectuer des opérations telles que l'addition et la multiplication de ces nombres.

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Installation](#installation)
- [Utilisation](#utilisation)
- [Fonctions principales](#fonctions-principales)
- [Test et Génération de Vector](#test-et-génération-de-vector)
- [Contributions](#contributions)

## Fonctionnalités

- **Initialisation de nombres** : Crée un nombre initialisé à zéro.
- **Conversion de chaînes en nombres** : Convertit une chaîne de caractères hexadécimaux en un nombre.
- **Addition** : Effectue l'addition de deux grands nombres.
- **Multiplication** : Effectue la multiplication de deux grands nombres.
- **Génération de tests** : Génère des nombres aléatoires et effectue des tests d'addition et de multiplication, puis écrit les résultats dans un fichier.
- **Affichage et sauvegarde** : Permet d'afficher un nombre à l'écran ou de l'écrire dans un fichier.

## Installation

1. Clonez le repository sur votre machine locale :
   ```bash
   git clone https://github.com/AntoineDevFr/RSA
   ```

2. Assurez-vous d'avoir un compilateur C++ (par exemple GCC) installé sur votre machine.

3. Compilez le projet avec la commande suivante :
   ```bash
   make main
   ```

4. Exécutez le programme compilé :
   ```bash
   ./main
   ```



## Fonctions principales

### `gn_init(struct gn* n)`

Initialise tous les éléments du tableau à zéro dans la structure `gn`.

### `gn_from_string(struct gn* n, const char* str, int nbrCarac)`

Convertit une chaîne hexadécimale en un nombre représenté par un tableau de 32 bits.

### `gn_add(struct gn* a, struct gn* b, struct gn* c)`

Additionne les grands nombres `a` et `b` et stocke le résultat dans `c`.

### `gn_mul(struct gn* a, struct gn* b, struct gn* c)`

Multiplie les grands nombres `a` et `b` et stocke le résultat dans `c`.

### `gn_print(struct gn* n)`

Affiche le nombre dans la structure `gn` à l'écran.

### `gn_writeFile(struct gn* n, FILE* file)`

Écrit le nombre dans la structure `gn` dans un fichier spécifié.

### `gn_generate_1024(struct gn* n)`

Génère un grand nombre aléatoire de 1024 bits.

### `generate_testVector(FILE* file, int nbrTest)`

Génère des vecteurs de test en créant des nombres aléatoires et en effectuant des opérations d'addition et de multiplication sur eux, puis écrit les résultats dans un fichier.

## Test et Génération de Vector

La fonction `generate_testVector` crée des tests en générant 1024 bits de nombres aléatoires, en les additionnant et en les multipliant, puis en sauvegardant les résultats dans un fichier `test.txt`. Chaque ligne du fichier contiendra les résultats sous forme hexadécimale.

### Exemple de contenu du fichier généré

```txt
00000001000000000000000000000000...00000001000000000000000000000000...00000000000000000000000000000000...
00000000000000000000000000000000...00000000000000000000000000000000...00000000000000000000000000000000...
```