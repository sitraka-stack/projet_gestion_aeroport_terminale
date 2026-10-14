#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "Aeroport.h"

void afficherInterfaceUtilisateur() {
    std::cout << "\n==================================================\n";
    std::cout << "       TOUR DE CONTRÔLE - SIMULATION PRO          \n";
    std::cout << "==================================================\n";
    std::cout << "1. Enregistrer un vol commercial en approche\n";
    std::cout << "2. Déclencher une urgence critique (Panne carburant)\n";
    std::cout << "3. Enregistrer un avion prêt au décollage au sol\n";
    std::cout << "4. Faire avancer le temps d'un tour unique\n";
    std::cout << "5. 🚀 Lancer la simulation automatique en continu\n";
    std::cout << "6. Quitter et générer le rapport de fin\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Votre choix (1 à 6) : ";
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Aéroport avec 2 pistes et 3 places de parking maximum
    Aeroport aeroport(2, 3);
    int choixUtilisateur = 0;
    int compteurVol = 200;

    const std::string compagnies[] = {"Air Madagascar", "Air France", "Kenya Airways", "Ethiopian Airlines", "Cargo Fret Express"};

    do {
        std::cout << "\n--------------------------------------------------\n";
        std::cout << " TABLEAU DE BORD EN TEMPS RÉEL :\n";
        std::cout << "- 🚨 Urgences en approche (Carburant critique) : " << aeroport.compterUrgences() << "\n";
        std::cout << "- ✈️  Avions en attente d'atterrissage normal   : " << aeroport.compterAtterrissagesNormaux() << "\n";
        std::cout << "- 🛫 Avions au sol en attente de décollage     : " << aeroport.compterDecollages() << "\n";
        std::cout << "- 🅿️  Places de parking disponibles (Tarmac)    : " << aeroport.obtenirPlacesParkingLibres() << " / 3\n";
        std::cout << "- 📊 Bilan : " << aeroport.obtenirTotalAtterris() << " atterrissages | " 
                  << aeroport.obtenirTotalDecolles() << " décollages | " 
                  << aeroport.obtenirTotalIncidents() << " urgences sauvées.\n";

        afficherInterfaceUtilisateur();
        
        if (!(std::cin >> choixUtilisateur)) {
            break;
        }

        switch (choixUtilisateur) {
            case 1: {
                std::string id = "MDG-" + std::to_string(compteurVol++);
                std::string compagnie = compagnies[rand() % 5];
                TypeAvion type = (rand() % 2 == 0) ? TypeAvion::CourtCourrier : TypeAvion::LongCourrier;
                int carburant = 50 + (rand() % 51); // Entre 50% et 100%
                
                auto avion = std::make_shared<Avion>(id, compagnie, type, carburant);
                aeroport.enregistrerDemandeAtterrissage(avion);
                break;
            }
            case 2: {
                std::string id = "URG-" + std::to_string(compteurVol++);
                std::string compagnie = compagnies[rand() % 5];
                int carburantFaible = 10 + (rand() % 10); // Entre 10% et 19%
                
                auto avionUrgent = std::make_shared<Avion>(id, compagnie, TypeAvion::CourtCourrier, carburantFaible);
                aeroport.enregistrerDemandeAtterrissage(avionUrgent);
                break;
            }
            case 3: {
                std::string id = "DEP-" + std::to_string(compteurVol++);
                std::string compagnie = compagnies[rand() % 5];
                auto avionDepart = std::make_shared<Avion>(id, compagnie, TypeAvion::LongCourrier, 100);
                aeroport.enregistrerDemandeDecollage(avionDepart);
                break;
            }
            case 4: {
                aeroport.executerTourDeControle();
                break;
            }
            case 5: {
                std::cout << "\n🚀 Lancement du mode automatique (Appuyez sur Ctrl+C dans le terminal pour stopper).\n";
                for (int tour = 1; tour <= 5; ++tour) {
                    std::cout << "\n--- Tour automatique n°" << tour << " ---\n";
                    aeroport.executerTourDeControle();
                    // Pause de 2 secondes entre chaque tour pour laisser l'humain lire
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
                break;
            }
            case 6: {
                aeroport.genererRapportFichierTxt();
                std::cout << "\nFermeture de la tour de contrôle. Au revoir !\n";
                break;
            }
            default:
                std::cout << "\n[Erreur] Option non reconnue. Choisissez entre 1 et 6.\n";
        }
    } while (choixUtilisateur != 6);

    return 0;
}