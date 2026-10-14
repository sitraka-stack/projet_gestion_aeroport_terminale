#pragma once
#include <string>

enum class TypeAvion {
    CourtCourrier,
    LongCourrier,
    FretCargo
};

enum class SituationAvion {
    EnApproche,
    Stationne,
    EnRoute
};

class Avion {
private:
    std::string numeroVol;
    std::string compagnie;
    TypeAvion type;
    SituationAvion situation;
    int carburant;
    bool urgence;

public:
    Avion(const std::string &num, const std::string &comp, TypeAvion t, int carb);
    
    std::string getNumeroVol() const;
    std::string getCompagnie() const;
    std::string getTypeTexte() const;
    SituationAvion getSituation() const;
    void setSituation(SituationAvion sit);
    
    int getCarburant() const;
    void consommerCarburant();
    bool estUrgent() const;
};