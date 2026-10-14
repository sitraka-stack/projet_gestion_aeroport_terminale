#include "Avion.h"

Avion::Avion(const std::string &num, const std::string &comp, TypeAvion t, int carb) 
    : numeroVol(num), compagnie(comp), type(t), situation(SituationAvion::EnApproche), 
      carburant(carb), urgence(carb < 20) {}

std::string Avion::getNumeroVol() const { return numeroVol; }
std::string Avion::getCompagnie() const { return compagnie; }

std::string Avion::getTypeTexte() const {
    switch (type) {
        case TypeAvion::CourtCourrier: return "Court-courrier";
        case TypeAvion::LongCourrier: return "Gros porteur";
        case TypeAvion::FretCargo: return "Fret cargo";
    }
    return "Standard";
}

SituationAvion Avion::getSituation() const { return situation; }
void Avion::setSituation(SituationAvion sit) { situation = sit; }
int Avion::getCarburant() const { return carburant; }

void Avion::consommerCarburant() {
    if (carburant > 5) {
        carburant -= 8;
    }
    if (carburant < 20) {
        urgence = true;
    }
}

bool Avion::estUrgent() const { return urgence; }