# Projet Jeu de la vie

Le jeu de la vie est un automate cellulaire inventé par le mathématicien John Conway. Ce projet propose une implémentation en C++ du jeu de la vie en utilisant les concepts de la programmation orientée objet (POO). 
Dans ce jeu, une population de cellules, vivantes ou mortes, évolue sur une grille bidimensionnelle selon des règles simples :
-  Une cellule morte possédant exactement trois voisines vivantes devient vivante.
-  Une cellule vivante possédant deux ou trois voisines vivantes reste vivante, sinon elle meurt.
-  
Le projet inclut également un mode graphique basé sur la bibliothèque SFML pour visualiser l'évolution de la grille.

Fonctionnalités
Modes de fonctionnement
Mode console :
-  Affiche les états des cellules pour un nombre d'itérations donné.
-  Les résultats sont écrits dans des fichiers dans un dossier nommé <nom_du_fichier_dentree>_out.
Mode graphique :
-  Affiche la grille de cellules dans une interface graphique.
-  La simulation démarre automatiquement avec un contrôle sur la durée entre deux itérations.

Extensions :
-  Grille torique : Les cellules des bords opposés de la grille sont considérées comme adjacentes.
-  Cellules obstacle : Les cellules obstacles conservent leur état initial (vivant ou mort) tout au long de l'exécution.
