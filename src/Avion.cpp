#include "Avion.h"

Avion::Avion(const std::string &numeroVol, const std::string &compagnie, TypeAvion type, int carburant) 
    : numeroVol(numeroVol), compagnieAerienne(compagnie), typeAvion(type), 
      situation(SituationAvion::EnApprocheDansLeCiel), niveauCarburantPourcent(carburant), 
      estEnUrgenceAbsolue(carburant < 20) {}

std::string Avion::obtenirNumeroVol() const { return numeroVol; }
std::string Avion::obtenirCompagnie() const { return compagnieAerienne; }
TypeAvion Avion::obtenirTypeAvion() const { return typeAvion; }

std::string Avion::obtenirTypeAvionTexte() const {
    switch (typeAvion) {
        case TypeAvion::CourtCourrier: return "Court-Courrier";
        case TypeAvion::LongCourrier: return "Gros Porteur";
        case TypeAvion::FretCargo: return "Avion Fret";
    }
    return "Standard";
}

SituationAvion Avion::obtenirSituation() const { return situation; }
void Avion::modifierSituation(SituationAvion nouvelleSituation) { situation = nouvelleSituation; }
int Avion::obtenirNiveauCarburant() const { return niveauCarburantPourcent; }

void Avion::consommerCarburant() {
    if (niveauCarburantPourcent > 5) {
        niveauCarburantPourcent -= 8; // Consommation par tour en vol
    }
    if (niveauCarburantPourcent < 20) {
        estEnUrgenceAbsolue = true;
    }
}

bool Avion::verifierUrgence() const { return estEnUrgenceAbsolue; }