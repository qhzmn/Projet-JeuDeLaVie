#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>

class Cellule {
public:
    bool estVivante;
    bool nouvelEtat;
    bool obstacle;
    std::vector<Cellule*> voisins;

    Cellule(bool vivante = false) : estVivante(vivante), nouvelEtat(false), obstacle(false) {}

    void calculerEtatSuivant() { //Calcule l'état suivant de la cellule en fonction du nombre de voisins vivants.
        if (obstacle) {
            nouvelEtat = estVivante;
            return;
        }
        int nbVoisinsVivants = 0;
        for (const Cellule* voisin : voisins) {
            if (voisin->estVivante) {
                nbVoisinsVivants++;
            }
        }
        if (estVivante) {
            nouvelEtat = (nbVoisinsVivants == 2 || nbVoisinsVivants == 3);
        } else {
            nouvelEtat = (nbVoisinsVivants == 3);
        }
    }
    void appliquerNouvelEtat() { //Applique l'état calculé dans nouvelEtat à la cellule si ce n'est pas un obstacle.
        if (!obstacle) {
            estVivante = nouvelEtat;
        }
    }
};

class Grille {
private:
    std::vector<std::vector<Cellule*>> cellules;
    std::vector<std::vector<bool>> etatPrecedent;
    
    int largeur, hauteur;

public:
    Grille(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
        cellules.resize(hauteur, std::vector<Cellule*>(largeur, nullptr));
        initialiser();
        definirVoisins();
    }
    Grille(const std::string& fichier) : largeur(0), hauteur(0) {
        chargerGrilleDepuisFichier(fichier);
        definirVoisins();
    }
    ~Grille() {
        for (auto& ligne : cellules) {
            for (auto& cellule : ligne) {
                delete cellule;
            }
        }
    }
    void initialiserEtatPrecedent() { 
        etatPrecedent.resize(hauteur, std::vector<bool>(largeur, false));
    }

    void initialiser() { ////Remplie la grille avec des cellules vivantes ou mortes de manière aléatoire.
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                cellules[i][j] = new Cellule(dis(gen));
            }
        }
        sauvegarderGrilleDansFichier("grid_out.txt");
    }
    void chargerGrilleDepuisFichier(const std::string& nomFichier) { //Charge une grille à partir d'un fichier texte, en lisant les dimensions et les états des cellules.
        std::ifstream fichier(nomFichier);
        if (!fichier.is_open()) {
            throw std::runtime_error("Erreur d'ouverture du fichier.");
        }
        fichier >> hauteur >> largeur;
        cellules.resize(hauteur, std::vector<Cellule*>(largeur, nullptr));
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                int val;
                fichier >> val;
                cellules[i][j] = new Cellule(val == 1);
            }
        }
        sauvegarderGrilleDansFichier("grid_out.txt");
        fichier.close();
    }
    void obstacle(int x, int y) { //Place un obstacle à la position (x, y).
        if (x >= 0 && x < hauteur && y >= 0 && y < largeur) {
            cellules[x][y]->obstacle = true;
             cellules[x][y]->estVivante = false;
        }
    }
    void definirVoisins() { //Associe les cellules voisines pour chaque cellule, en tenant compte des bords de la grille.
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue;
                        int ni = (i + di + hauteur) % hauteur;
                        int nj = (j + dj + largeur) % largeur;
                        cellules[i][j]->voisins.push_back(cellules[ni][nj]);
                    }
                }
            }
        }
    }
    void afficherDansTerminal() const { //Affiche l'état actuel de la grille dans le terminal.
        for (const auto& ligne : cellules) {
            for (const auto& cellule : ligne) {
                if (cellule->obstacle) {
                    std::cout << "X ";
                } else {
                    std::cout << (cellule->estVivante ? "1 " : "0 ");
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    void sauvegarderGrilleDansFichier(const std::string& nomFichier) const { //Sauvegarde l'état actuel de la grille dans un fichier texte.
        std::ofstream fichier(nomFichier, std::ios::app);
        if (!fichier.is_open()) {
            throw std::runtime_error("Erreur lors de l'ouverture du fichier pour l'écriture.");
        }
        for (const auto& ligne : cellules) {
            for (const auto& cellule : ligne) {
                fichier << (cellule->obstacle ? "X" : (cellule->estVivante ? "1" : "0")) << " ";
            }
            fichier << '\n';
        }
        fichier << '\n';
        fichier.close();
    }
    bool estVide() const { //Retourne true si toutes les cellules sont mortes.
        for (const auto& ligne : cellules) {
            for (const auto& cellule : ligne) {
                if (cellule->estVivante) {
                    return false;
                }
            }
        }
        return true;
    }
    void sauvegarderEtatPrecedent() { //Sauvegarde l'état courant de la grille dans etatPrecedent pour pouvoir comparer les états et vérifier si la grille est stable.
        etatPrecedent.resize(hauteur, std::vector<bool>(largeur));
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                etatPrecedent[i][j] = cellules[i][j]->estVivante;
            }
        }
    }
    bool estIdentiqueAEtatPrecedent() const { //Vérifie si l'état actuel de la grille est identique à l'état précédent.
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                if (cellules[i][j]->estVivante != etatPrecedent[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    Cellule* getCellule(int i, int j) { return cellules[i][j]; }
    int getLargeur() const { return largeur; }
    int getHauteur() const { return hauteur; }
    const std::vector<std::vector<Cellule*>>& getCellules() const { return cellules; }
};

class Simulation {
private:
    Grille& grille;

public:
    Simulation(Grille& grille) : grille(grille) {}

    void etapeSuivante() { //Sauvegarde l'état précédent de la grille, calcule l'état suivant pour chaque cellule et applique le nouvel état à chaque cellule.
        grille.sauvegarderEtatPrecedent();
        grille.sauvegarderGrilleDansFichier("grid_out.txt");
        for (int i = 0; i < grille.getHauteur(); ++i) {
            for (int j = 0; j < grille.getLargeur(); ++j) {
                grille.getCellule(i, j)->calculerEtatSuivant();
            }
        }
        for (int i = 0; i < grille.getHauteur(); ++i) {
            for (int j = 0; j < grille.getLargeur(); ++j) {
                grille.getCellule(i, j)->appliquerNouvelEtat();
            }
        }
    }
};

class Interface {
private:
    Grille& grille;
    Simulation& simulation;
    sf::RenderWindow window;
    sf::RectangleShape celluleShape;

public:
    Interface(Grille& grille, Simulation& simulation): grille(grille), simulation(simulation) { 
        sf::VideoMode mode = sf::VideoMode::getDesktopMode();// Obtention de la résolution de l'écran
        // Création de la fenêtre en mode plein écran
        window.create(mode, "Jeu de la Vie");
        // Calcul de la taille des cellules en fonction de la taille de la fenêtre et de la grille
        float largeurFenetre = static_cast<float>(mode.width);
        float hauteurFenetre = static_cast<float>(mode.height); 
        // Calcul de la taille des cellules en fonction de la grille
        float tailleCelluleX = largeurFenetre / grille.getLargeur();
        float tailleCelluleY = hauteurFenetre / grille.getHauteur();
        // Utilisation de la taille minimale pour que les cellules soient visibles sans dépasser la fenêtre
        float tailleCellule = std::min(tailleCelluleX, tailleCelluleY);
        celluleShape.setSize(sf::Vector2f(tailleCellule, tailleCellule));
        celluleShape.setOutlineColor(sf::Color::Black);
        celluleShape.setOutlineThickness(1.f);
    }
    void run() { //Gère  la boucle de l'interface graphique, ainsi que les touches claviers.
    bool estEnPause = false;
    simulation.etapeSuivante();

    while (window.isOpen() && !grille.estVide() && !grille.estIdentiqueAEtatPrecedent()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    estEnPause = !estEnPause;  // Alterne l'état de pause
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();  // Ferme la fenêtre
                }
            }
        }

        if (!estEnPause) {
            simulation.etapeSuivante();
            window.clear(sf::Color::White);
            for (int i = 0; i < grille.getHauteur(); ++i) {
                for (int j = 0; j < grille.getLargeur(); ++j) {
                    Cellule* cellule = grille.getCellule(i, j);
                    celluleShape.setFillColor(cellule->obstacle ? sf::Color::Red : (cellule->estVivante ? sf::Color::White : sf::Color::Black));
                    celluleShape.setPosition(j * celluleShape.getSize().x, i * celluleShape.getSize().y);
                    window.draw(celluleShape);
                }
            }
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Ajustez la vitesse si nécessaire
        }
    }
}

};

int main() {
    //Grille grille("grille1.txt");
    Grille grille(75,50);
    Simulation simulation(grille);
    grille.initialiserEtatPrecedent();
    //grille.obstacle(2, 2);
    int choix;
    std::cout << "Choisissez le mode d'affichage :\n";
    std::cout << "1. Terminal\n";
    std::cout << "2. Interface graphique\n";
    std::cin >> choix;
    if (choix == 1) {
        std::cout << "Nombre d'itération ? ";
        std::cin >> choix;
        int iteration = 1;
        std::cout << "---Grille initiale---" << std::endl;  
        grille.afficherDansTerminal();
        
        while (iteration != choix + 1 && !grille.estVide() && !grille.estIdentiqueAEtatPrecedent()) {
            std::cout << "---Grille itération " << iteration << " ---" << std::endl;
            simulation.etapeSuivante();
            grille.afficherDansTerminal();
            iteration++;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    } else if (choix == 2) {
        Interface interface(grille, simulation);
        interface.run();
    }
    return 0;
}
