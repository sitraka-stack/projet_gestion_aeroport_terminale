#include "Aeroport.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

Aeroport::Aeroport(int nombrePistes, int capaciteParking) 
    : capaciteTotaleParking(capaciteParking), placesParkingOccupees(0),
      nombrePistesDisponibles(nombrePistes), totalAvionsAtterris(0), 
      totalAvionsDecolles(0), totalIncidentsEvites(0), meteoActuelle(ConditionMeteo::GrandSoleil) {}

void Aeroport::enregistrerDemandeAtterrissage(std::shared_ptr<Avion> avion) {
    avion->modifierSituation(SituationAvion::EnApprocheDansLeCiel);
    
    if (avion->verifierUrgence()) {
        fileUrgenceAtterrissage.push_back(avion);
        std::cout << "\n🚨 [ALERTE ROUGE] L'avion " << avion->obtenirNumeroVol() 
                  << " (" << avion->obtenirCompagnie() << ") manque cruellement de carburant (" 
                  << avion->obtenirNiveauCarburant() << "%) ! PRIORITÉ ABSOLUE !\n";
    } else {
        fileAttenteAtterrissagesNormaux.push(avion);
        std::cout << "\n✈️ [TOUR DE CONTRÔLE] Vol " << avion->obtenirNumeroVol() 
                  << " (" << avion->obtenirCompagnie() << ") en approche. Carburant : " 
                  << avion->obtenirNiveauCarburant() << "%\n";
    }
}

void Aeroport::enregistrerDemandeDecollage(std::shared_ptr<Avion> avion) {
    avion->modifierSituation(SituationAvion::StationneAuParking);
    fileAttenteDecollages.push(avion);
    placesParkingOccupees++; // Occupe une place au sol
    std::cout << "\n🛫 [PARKING] L'avion " << avion->obtenirNumeroVol() 
              << " est stationné et demande l'autorisation de décoller.\n";
}

void Aeroport::changerMeteoAleatoire() {
    int de = rand() % 3;
    if (de == 0) meteoActuelle = ConditionMeteo::GrandSoleil;
    else if (de == 1) meteoActuelle = ConditionMeteo::PluieFine;
    else meteoActuelle = ConditionMeteo::OrageViolent;
}

std::string Aeroport::obtenirDescriptionMeteo() const {
    switch (meteoActuelle) {
        case ConditionMeteo::GrandSoleil: return "☀️ Grand Soleil (Conditions optimales)";
        case ConditionMeteo::PluieFine: return "🌦️ Pluie Fine (Visibilité réduite)";
        case ConditionMeteo::OrageViolent: return "⛈️ Orage Violent (Ralentissement des pistes)";
    }
    return "Inconnue";
}

void Aeroport::executerTourDeControle() {
    std::cout << "\n==================================================\n";
    std::cout << "        RAPPORT DU TOUR DE CONTRÔLE AÉRIEN        \n";
    std::cout << "==================================================\n";
    
    // On simule l'écoulement du temps en réduisant le carburant des avions en attente normale
    std::queue<std::shared_ptr<Avion>> fileTemporaire;
    while (!fileAttenteAtterrissagesNormaux.empty()) {
        auto avion = fileAttenteAtterrissagesNormaux.front();
        fileAttenteAtterrissagesNormaux.pop();
        avion->consommerCarburant();
        
        // S'il passe sous les 20%, il bascule en urgence absolue !
        if (avion->verifierUrgence()) {
            std::cout << "⚠️ [URGENCE DYNAMIQUE] Le vol " << avion->obtenirNumeroVol() 
                      << " vient de passer sous le seuil critique de carburant !\n";
            fileUrgenceAtterrissage.push_back(avion);
        } else {
            fileTemporaire.push(avion);
        }
    }
    fileAttenteAtterrissagesNormaux = fileTemporaire;

    // Changement météo
    changerMeteoAleatoire();
    std::cout << "Météo actuelle : " << obtenirDescriptionMeteo() << "\n";

    int pistesEffectives = nombrePistesDisponibles;
    if (meteoActuelle == ConditionMeteo::OrageViolent) {
        pistesEffectives = 1; 
        std::cout << "⚡ [SÉCURITÉ] Orage violent : une piste est fermée temporairement !\n";
    }

    while (pistesEffectives > 0) {
        // URGENCE ABSOLUE
        if (!fileUrgenceAtterrissage.empty()) {
            if (placesParkingOccupees < capaciteTotaleParking) {
                auto avionUrgent = fileUrgenceAtterrissage.front();
                fileUrgenceAtterrissage.erase(fileUrgenceAtterrissage.begin());
                avionUrgent->modifierSituation(SituationAvion::StationneAuParking);
                totalAvionsAtterris++;
                totalIncidentsEvites++;
                placesParkingOccupees++;
                std::cout << "🚨 [SAUVETAGE] L'avion d'urgence " << avionUrgent->obtenirNumeroVol() 
                          << " s'est posé en catastrophe ! Tout le monde est sain et sauf.\n";
            } else {
                std::cout << "🛑 [BOUCHON] Urgence signalée mais le parking est saturé ! Atterrissage repoussé d'un tour !\n";
            }
            pistesEffectives--;
        }
        // ATTERRISSAGES NORMAUX (Vérifie d'abord si le parking a de la place)
        else if (!fileAttenteAtterrissagesNormaux.empty()) {
            auto avionNormal = fileAttenteAtterrissagesNormaux.front();

            if (placesParkingOccupees < capaciteTotaleParking) {
                fileAttenteAtterrissagesNormaux.pop();
                avionNormal->modifierSituation(SituationAvion::StationneAuParking);
                totalAvionsAtterris++;
                placesParkingOccupees++;
                std::cout << "🛬 [PISTE] Atterrissage réussi pour le vol " << avionNormal->obtenirNumeroVol() 
                          << " (" << avionNormal->obtenirCompagnie() << "). Place de parking attribuée.\n";
                pistesEffectives--;
            } else {
                std::cout << "🛑 [TARMAC PLEIN] Le vol " << avionNormal->obtenirNumeroVol() 
                          << " doit patienter en altitude : le parking est complet (" << placesParkingOccupees << "/" << capaciteTotaleParking << ").\n";
                break; // Impossible d'atterrir faute de place
            }
        }
        // DÉCOLLAGES (Libèrent des places de parking !)
        else if (!fileAttenteDecollages.empty()) {
            auto avionDepart = fileAttenteDecollages.front();
            fileAttenteDecollages.pop();
            avionDepart->modifierSituation(SituationAvion::EnRouteVersDestination);
            totalAvionsDecolles++;
            libererPlaceParking(); // Libère la place sur le tarmac
            std::cout << "🛫 [PISTE] Le vol " << avionDepart->obtenirNumeroVol() 
                      << " a décollé avec succès ! Une place de parking vient de se libérer.\n";
            pistesEffectives--;
        }
        else {
            break; 
        }
    }
}

void Aeroport::libererPlaceParking() {
    if (placesParkingOccupees > 0) {
        placesParkingOccupees--;
    }
}

size_t Aeroport::compterUrgences() const { return fileUrgenceAtterrissage.size(); }
size_t Aeroport::compterAtterrissagesNormaux() const { return fileAttenteAtterrissagesNormaux.size(); }
size_t Aeroport::compterDecollages() const { return fileAttenteDecollages.size(); }
int Aeroport::obtenirPlacesParkingLibres() const { return capaciteTotaleParking - placesParkingOccupees; }
int Aeroport::obtenirTotalAtterris() const { return totalAvionsAtterris; }
int Aeroport::obtenirTotalDecolles() const { return totalAvionsDecolles; }
int Aeroport::obtenirTotalIncidents() const { return totalIncidentsEvites; }

void Aeroport::genererRapportFichierTxt() const {
    std::ofstream fichierRapport("rapport_aeroport.txt");
    if (fichierRapport.is_open()) {
        fichierRapport << "========================================\n";
        fichierRapport << "    RAPPORT OFFICIEL DE FIN DE SESSION   \n";
        fichierRapport << "========================================\n";
        fichierRapport << "Total des atterrissages réussis : " << totalAvionsAtterris << "\n";
        fichierRapport << "Total des décollages effectués   : " << totalAvionsDecolles << "\n";
        fichierRapport << "Urgences critiques gérées & sauvées : " << totalIncidentsEvites << "\n";
        fichierRapport << "Statut du tarmac à la fermeture   : " << placesParkingOccupees << " avions encore garés.\n";
        fichierRapport << "========================================\n";
        fichierRapport.close();
        std::cout << "\n📄 [RAPPORT SAUVEGARDÉ] Le fichier 'rapport_aeroport.txt' a été créé avec succès sur votre disque.\n";
    }
}