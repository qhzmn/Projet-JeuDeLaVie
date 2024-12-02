#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

class Cellule {
public:
    bool estVivante;
    bool nouvelEtat; // Stocke le nouvel état avant de l'appliquer
    std::vector<Cellule*> voisins;

    Cellule(bool vivante = false) : estVivante(vivante) {}

    void calculerEtatSuivant() {
        int nbVoisinsVivants = 0;
        for (Cellule* voisin : voisins) {
            if (voisin->estVivante) {
                nbVoisinsVivants++;
            }
        }

        // Règles du jeu de la vie
        if (estVivante) {
            // Meurt si < 2 ou > 3 voisins vivants
            nouvelEtat = (nbVoisinsVivants == 2 || nbVoisinsVivants == 3);
        } else {
            // Devient vivante si exactement 3 voisins vivants
            nouvelEtat = (nbVoisinsVivants == 3);
        }
    }

    void appliquerNouvelEtat() {
        estVivante = nouvelEtat;
    }
};

class JeuDeLaVie {
public:
    JeuDeLaVie(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
        grille.resize(hauteur, std::vector<Cellule*>(largeur, nullptr));
        initialiser();
    }

    ~JeuDeLaVie() {
        for (auto& ligne : grille) {
            for (auto& cellule : ligne) {
                delete cellule;
            }
        }
    }

    void initialiser() {
        srand(time(0));
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                grille[i][j] = new Cellule(rand() % 2 == 0);
            }
        }

        // Définir les voisins
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
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                grille[i][j]->calculerEtatSuivant();
            }
        }
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                grille[i][j]->appliquerNouvelEtat();
            }
        }
    }

    void afficherGrille() {
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                std::cout << (grille[i][j]->estVivante ? "1" : "0");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void afficher(sf::RenderWindow& window) {
        float cellWidth = window.getSize().x / largeur;
        float cellHeight = window.getSize().y / hauteur;
 
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                sf::RectangleShape rect(sf::Vector2f(cellWidth, cellHeight));
                rect.setPosition(j * cellWidth, i * cellHeight);
 
                if (grille[i][j]->estVivante) {
                    rect.setFillColor(sf::Color::Green);
                } else {
                    rect.setFillColor(sf::Color::Black);
                }
 
                window.draw(rect);
            }
        }
    }
    

private:
    std::vector<std::vector<Cellule*>> grille;
    int largeur, hauteur;
};

int main() {
    
    const int largeurFenetre = 800;
    const int hauteurFenetre = 600;
    //sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Jeu de la Vie - SFML", sf::Style::Fullscreen);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Jeu de la Vie");
 
    // Créer une grille de 50x50
    JeuDeLaVie jeu(100, 100);
 
    // Initialiser la simulation
    jeu.initialiser();
 
    // Boucle principale de SFML
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
 
        window.clear();
        jeu.afficher(window); // Afficher l'état actuel de la grille
        window.display();
 
        jeu.etapeSuivante(); // Calculer l'étape suivante
        sf::sleep(sf::milliseconds(50)); // Pause entre chaque étape
    }
 
    return 0;
}
