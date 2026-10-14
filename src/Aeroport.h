#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include "Avion.h"

enum class ConditionMeteo {
    GrandSoleil,
    PluieFine,
    OrageViolent
};

class Aeroport {
private:
    std::vector<std::shared_ptr<Avion>> fileUrgenceAtterrissage;
    std::queue<std::shared_ptr<Avion>> fileAttenteAtterrissagesNormaux;
    std::queue<std::shared_ptr<Avion>> fileAttenteDecollages;
    
    // Gestion des places de parking (Portes de tarmac)
    int capaciteTotaleParking;
    int placesParkingOccupees;

    int nombrePistesDisponibles;
    int totalAvionsAtterris;
    int totalAvionsDecolles;
    int totalIncidentsEvites;
    ConditionMeteo meteoActuelle;

public:
    Aeroport(int nombrePistes = 2, int capaciteParking = 3);
    
    void enregistrerDemandeAtterrissage(std::shared_ptr<Avion> avion);
    void enregistrerDemandeDecollage(std::shared_ptr<Avion> avion);
    
    void changerMeteoAleatoire();
    std::string obtenirDescriptionMeteo() const;
    
    void executerTourDeControle();
    void libererPlaceParking(); // Libère une place quand un avion décolle
    
    size_t compterUrgences() const;
    size_t compterAtterrissagesNormaux() const;
    size_t compterDecollages() const;
    int obtenirPlacesParkingLibres() const;
    int obtenirTotalAtterris() const;
    int obtenirTotalDecolles() const;
    int obtenirTotalIncidents() const;
    
    void genererRapportFichierTxt() const;
};