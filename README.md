# Projet Jeu de la Vie 🎮

Le Jeu de la Vie est un automate cellulaire inventé par le mathématicien John Conway. Ce projet propose une implémentation en C++ du Jeu de la Vie en utilisant les concepts de la programmation orientée objet (POO). Dans ce jeu, une population de cellules, vivantes ou mortes, évolue sur une grille bidimensionnelle selon des règles simples :

- Une cellule morte possédant exactement trois voisines vivantes devient vivante.
- Une cellule vivante possédant deux ou trois voisines vivantes reste vivante, sinon elle meurt.

## Fonctionnalités

### Mode Console

- **Affichage des États des Cellules** : Affiche les états des cellules pour un nombre d'itérations donné.
- **Sortie Fichier** : Les résultats sont écrits dans des fichiers dans un dossier nommé `grid_out.txt`.

### Mode Graphique

- **Interface Graphique** : Affiche la grille de cellules dans une interface graphique basée sur la bibliothèque SFML.
- **Contrôle de la Simulation** : La simulation démarre automatiquement avec un contrôle sur la durée entre deux itérations.

## Extensions

- **Grille Torique** : Les cellules des bords opposés de la grille sont considérées comme adjacentes.
- **Cellules Obstacle** : Les cellules obstacles conservent leur état initial (vivant ou mort) tout au long de l'exécution.

## Exemple d'Utilisation

Pour compiler le programme :

```bash
g++ -o jeu_de_la_vie v02121342.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

Pour lancer le programme :

```bash
./jeu_de_la_vie
```

## Captures d'Écran

![image](https://github.com/user-attachments/assets/5913db1a-7a3b-479b-99ac-bd3234785450)

---

1. [Fonctionnalités](#fonctionnalités)
2. [Extensions](#extensions)
3. [Exemple d'Utilisation](#exemple-d'utilisation)
4. Captures d'Écran
