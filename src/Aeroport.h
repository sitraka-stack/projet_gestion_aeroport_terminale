#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include "Avion.h"

enum class Meteo {
    Clair,
    Pluie,
    Orage
};

class Aeroport {
private:
    std::vector<std::shared_ptr<Avion>> fileUrgences;
    std::queue<std::shared_ptr<Avion>> fileAtterrissages;
    std::queue<std::shared_ptr<Avion>> fileDecollages;
    
    int capaciteParking;
    int parkingOccupe;
    int pistesDisponibles;
    
    int totalAtterris;
    int totalDecolles;
    int totalUrgencesTraitees;
    Meteo meteoActuelle;

public:
    Aeroport(int pistes = 4, int parking = 8);
    
    void ajouterAtterissage(std::shared_ptr<Avion> avion);
    void ajouterDecollage(std::shared_ptr<Avion> avion);
    
    void changerMeteo();
    std::string getMeteoTexte() const;
    
    void executerTour();
    
    size_t getNbUrgences() const;
    size_t getNbAtterrissages() const;
    size_t getNbDecollages() const;
    int getParkingLibre() const;
    int getTotalAtterris() const;
    int getTotalDecolles() const;
    int getTotalUrgences() const;
    
    void exporterRapport() const;
};