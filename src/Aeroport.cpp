#include "Aeroport.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

Aeroport::Aeroport(int pistes, int parking) 
    : capaciteParking(parking), parkingOccupe(0), pistesDisponibles(pistes),
      totalAtterris(0), totalDecolles(0), totalUrgencesTraitees(0), meteoActuelle(Meteo::Clair) {}

void Aeroport::ajouterAtterissage(std::shared_ptr<Avion> avion) {
    avion->setSituation(SituationAvion::EnApproche);
    if (avion->estUrgent()) {
        fileUrgences.push_back(avion);
    } else {
        fileAtterrissages.push(avion);
    }
}

void Aeroport::ajouterDecollage(std::shared_ptr<Avion> avion) {
    avion->setSituation(SituationAvion::Stationne);
    fileDecollages.push(avion);
    parkingOccupe++;
}

void Aeroport::changerMeteo() {
    int r = rand() % 3;
    if (r == 0) meteoActuelle = Meteo::Clair;
    else if (r == 1) meteoActuelle = Meteo::Pluie;
    else meteoActuelle = Meteo::Orage;
}

std::string Aeroport::getMeteoTexte() const {
    switch (meteoActuelle) {
        case Meteo::Clair: return "Clair";
        case Meteo::Pluie: return "Pluie fine";
        case Meteo::Orage: return "Orage (1 piste fermee)";
    }
    return "Normal";
}

void Aeroport::executerTour() {
    std::queue<std::shared_ptr<Avion>> temp;
    while (!fileAtterrissages.empty()) {
        auto avion = fileAtterrissages.front();
        fileAtterrissages.pop();
        avion->consommerCarburant();
        if (avion->estUrgent()) {
            fileUrgences.push_back(avion);
        } else {
            temp.push(avion);
        }
    }
    fileAtterrissages = temp;

    changerMeteo();

    int pistesEffectives = pistesDisponibles;
    if (meteoActuelle == Meteo::Orage) {
        pistesEffectives = 1;
    }

    while (pistesEffectives > 0) {
        if (!fileUrgences.empty()) {
            if (parkingOccupe < capaciteParking) {
                auto avion = fileUrgences.front();
                fileUrgences.erase(fileUrgences.begin());
                avion->setSituation(SituationAvion::Stationne);
                totalAtterris++;
                totalUrgencesTraitees++;
                parkingOccupe++;
            }
            pistesEffectives--;
        }
        else if (!fileAtterrissages.empty()) {
            auto avion = fileAtterrissages.front();
            if (parkingOccupe < capaciteParking) {
                fileAtterrissages.pop();
                avion->setSituation(SituationAvion::Stationne);
                totalAtterris++;
                parkingOccupe++;
                pistesEffectives--;
            } else {
                break;
            }
        }
        else if (!fileDecollages.empty()) {
            auto avion = fileDecollages.front();
            fileDecollages.pop();
            avion->setSituation(SituationAvion::EnRoute);
            totalDecolles++;
            if (parkingOccupe > 0) parkingOccupe--;
            pistesEffectives--;
        }
        else {
            break;
        }
    }
}

size_t Aeroport::getNbUrgences() const { return fileUrgences.size(); }
size_t Aeroport::getNbAtterrissages() const { return fileAtterrissages.size(); }
size_t Aeroport::getNbDecollages() const { return fileDecollages.size(); }
int Aeroport::getParkingLibre() const { return capaciteParking - parkingOccupe; }
int Aeroport::getTotalAtterris() const { return totalAtterris; }
int Aeroport::getTotalDecolles() const { return totalDecolles; }
int Aeroport::getTotalUrgences() const { return totalUrgencesTraitees; }

void Aeroport::exporterRapport() const {
    std::ofstream fichier("rapport_aeroport.txt");
    if (fichier.is_open()) {
        fichier << "RAPPORT DE FIN DE SIMULATION\n";
        fichier << "----------------------------\n";
        fichier << "Atterrisages reussis : " << totalAtterris << "\n";
        fichier << "Decollages effectues : " << totalDecolles << "\n";
        fichier << "Urgences gerees      : " << totalUrgencesTraitees << "\n";
        fichier << "Avions encore au sol : " << parkingOccupe << "\n";
        fichier.close();
    }
}