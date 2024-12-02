#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>  // <-- Ajouté pour inclure la gestion des fichiers
#include <stdexcept>

class Cellule {
public:
    bool estVivante;
    bool nouvelEtat; // Stocke le nouvel état avant de l'appliquer
    std::vector<Cellule*> voisins;

    Cellule(bool vivante = false) : estVivante(vivante), nouvelEtat(false) {}

    void calculerEtatSuivant() {
        int nbVoisinsVivants = 0;
        for (const Cellule* voisin : voisins) {
            if (voisin->estVivante) {
                nbVoisinsVivants++;
            }
        }

        // Règles du jeu de la vie
        if (estVivante) {
            nouvelEtat = (nbVoisinsVivants == 2 || nbVoisinsVivants == 3);
        } else {
            nouvelEtat = (nbVoisinsVivants == 3);
        }
    }

    void appliquerNouvelEtat() {
        estVivante = nouvelEtat;
    }
};

class JeuDeLaVie {
private:
    std::vector<std::vector<Cellule*>> grille;
    int largeur, hauteur;

public:
    JeuDeLaVie(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
        grille.resize(hauteur, std::vector<Cellule*>(largeur, nullptr));
        initialiser();
        definirVoisins();
    }
    JeuDeLaVie(const std::string& fichier) : largeur(0), hauteur(0) {
        chargerGrilleDepuisFichier(fichier);
        definirVoisins();
    } 

    ~JeuDeLaVie() {
        for (auto& ligne : grille) {
            for (auto& cellule : ligne) {
                delete cellule;
            }
        }
    }

    void initialiser() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                grille[i][j] = new Cellule(dis(gen));
            }
        }
    }

    void chargerGrilleDepuisFichier(const std::string& nomFichier) {
        std::ifstream fichier(nomFichier);
        if (!fichier.is_open()) {
            throw std::runtime_error("Erreur d'ouverture du fichier.");
        }
        fichier >> hauteur >> largeur;
        grille.resize(hauteur, std::vector<Cellule*>(largeur, nullptr));

        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                int val;
                fichier >> val;
                grille[i][j] = new Cellule(val);
            }
        }

        fichier.close();
    }

    void definirVoisins() {
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue; // Ignorer la cellule elle-même
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < hauteur && nj >= 0 && nj < largeur) {
                            grille[i][j]->voisins.push_back(grille[ni][nj]);
                        }
                    }
                }
            }
        }
    }

    void etapeSuivante() {
        for (const auto& ligne : grille) {
            for (const auto& cellule : ligne) {
                cellule->calculerEtatSuivant();
            }
        }
        for (const auto& ligne : grille) {
            for (const auto& cellule : ligne) {
                cellule->appliquerNouvelEtat();
            }
        }
    }

    void afficherGrille() const {
        for (const auto& ligne : grille) {
            for (const auto& cellule : ligne) {
                std::cout << (cellule->estVivante)<<" ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    Cellule* getCellule(int i, int j) {
        return grille[i][j];
    }

    int getHauteur() const { return hauteur; }
    int getLargeur() const { return largeur; }
};

class Interface {
private:
    JeuDeLaVie& jeu;
    sf::RenderWindow window;
    sf::RectangleShape celluleShape;

public:
    Interface(JeuDeLaVie& jeu) : jeu(jeu), window(sf::VideoMode(1000,800), "Jeu de la Vie") {
        celluleShape.setSize(sf::Vector2f(10.f, 10.f));  // Chaque cellule fait 10x10 pixels
        celluleShape.setOutlineColor(sf::Color::Black);
        celluleShape.setOutlineThickness(1.f);
    }
    void run() {
        // Boucle principale de la fenêtre
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                // Si l'utilisateur ferme la fenêtre, on quitte la boucle
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // Calculer l'étape suivante du jeu
            jeu.etapeSuivante();

            // Effacer l'écran avec la couleur blanche
            window.clear(sf::Color::White);

            // Dessiner chaque cellule
            for (int i = 0; i < jeu.getHauteur(); ++i) {
                for (int j = 0; j < jeu.getLargeur(); ++j) {
                    // Si la cellule est vivante, on la colore en vert, sinon elle est blanche
                    if (jeu.getCellule(i, j)->estVivante) {
                        celluleShape.setFillColor(sf::Color::Green);  // Cellule vivante en vert
                    } else {
                        celluleShape.setFillColor(sf::Color::White);  // Cellule morte en blanc
                    }
                    // Positionner le rectangle en fonction de la cellule dans la grille
                    celluleShape.setPosition(j * 10.f, i * 10.f);  // Décalage pour chaque cellule
                    // Dessiner le rectangle
                    window.draw(celluleShape);
                }
            }

            // Afficher tout ce qui a été dessiné dans la fenêtre
            window.display();
        }
    }
};

int main() {
    //JeuDeLaVie jeu(20, 20);
    //JeuDeLaVie jeu("grille2.txt");
    //JeuDeLaVie jeu("grille1.txt");

    /*
    jeu.afficherGrille();
    for (int i = 0; i < 50; i++){
        jeu.etapeSuivante();
        jeu.afficherGrille();
    }
    */
    /*
    Interface interface(jeu);
    interface.run();
    */
    return 0;
}
