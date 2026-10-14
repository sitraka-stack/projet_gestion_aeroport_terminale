#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Aeroport.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Aeroport aeroport(4, 8);
    int idVol = 200;
    const std::string compagnies[] = {"Air Madagascar", "Air France", "Kenya Airways", "Ethiopian", "Cargo Express"};

    int selected_option = 0;
    std::vector<std::string> option_list = {
        "Ajouter un vol en approche",
        "Declencher une urgence (carburant critique)",
        "Ajouter un vol au parking (decollage)",
        "Executer un tour de controle",
        "Quitter"
    };

    auto menu = Menu(&option_list, &selected_option);
    auto screen = ScreenInteractive::TerminalOutput();

    auto component = CatchEvent(menu, [&](Event event) {
        if (event == Event::Return) {
            if (selected_option == 0) {
                std::string num = "MDG-" + std::to_string(idVol++);
                std::string comp = compagnies[rand() % 5];
                int carb = 60 + (rand() % 41);
                auto avion = std::make_shared<Avion>(num, comp, TypeAvion::CourtCourrier, carb);
                aeroport.ajouterAtterissage(avion);
            } 
            else if (selected_option == 1) {
                std::string num = "URG-" + std::to_string(idVol++);
                std::string comp = compagnies[rand() % 5];
                int carb = 10 + (rand() % 10);
                auto avion = std::make_shared<Avion>(num, comp, TypeAvion::CourtCourrier, carb);
                aeroport.ajouterAtterissage(avion);
            } 
            else if (selected_option == 2) {
                std::string num = "DEP-" + std::to_string(idVol++);
                std::string comp = compagnies[rand() % 5];
                int carb = 100;
                auto avion = std::make_shared<Avion>(num, comp, TypeAvion::LongCourrier, carb);
                aeroport.ajouterDecollage(avion);
            } 
            else if (selected_option == 3) {
                aeroport.executerTour();
            } 
            else if (selected_option == 4) {
                aeroport.exporterRapport();
                screen.Exit();
            }
            return true;
        }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
            text("TOUR DE CONTROLE - GESTION D'AEROPORT") | bold | color(Color::Cyan) | center,
            separator(),
            hbox({
                vbox({
                    text("STATISTIQUES EN TEMPS REEL") | bold,
                    separator(),
                    text("Urgences en approche : " + std::to_string(aeroport.getNbUrgences())),
                    text("Attente atterrissage : " + std::to_string(aeroport.getNbAtterrissages())),
                    text("Attente decollage    : " + std::to_string(aeroport.getNbDecollages())),
                    text("Parking libre        : " + std::to_string(aeroport.getParkingLibre()) + " / 8"),
                }) | border | flex,
                vbox({
                    text("BILAN GLOBAL") | bold,
                    separator(),
                    text("Atterris : " + std::to_string(aeroport.getTotalAtterris())),
                    text("Decolles : " + std::to_string(aeroport.getTotalDecolles())),
                    text("Urgences : " + std::to_string(aeroport.getTotalUrgences())),
                }) | border | flex,
            }),
            separator(),
            text("Utilisez les fleches HAUT/BAS pour choisir et ENTREE pour valider :") | dim,
            menu->Render() | border,
        }) | border;
    });

    screen.Loop(renderer);

    return 0;
}