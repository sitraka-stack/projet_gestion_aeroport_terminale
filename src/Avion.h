#pragma once
#include <string>

enum class TypeAvion {
    CourtCourrier,
    LongCourrier,
    FretCargo
};

enum class SituationAvion {
    EnApprocheDansLeCiel,
    StationneAuParking,
    EnRouteVersDestination
};

class Avion {
private:
    std::string numeroVol;
    std::string compagnieAerienne;
    TypeAvion typeAvion;
    SituationAvion situation;
    int niveauCarburantPourcent;
    bool estEnUrgenceAbsolue;

public:
    Avion(const std::string &numeroVol, const std::string &compagnie, TypeAvion type, int carburant);
    
    std::string obtenirNumeroVol() const;
    std::string obtenirCompagnie() const;
    TypeAvion obtenirTypeAvion() const;
    std::string obtenirTypeAvionTexte() const;
    SituationAvion obtenirSituation() const;
    void modifierSituation(SituationAvion nouvelleSituation);
    
    int obtenirNiveauCarburant() const;
    void consommerCarburant(); // Décrémente automatiquement
    bool verifierUrgence() const;
};