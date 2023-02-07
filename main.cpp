#include "Projet.hpp"
using namespace travel;

int main()
{
    // Cree un instance de type projet
    Projet generic;

    // Charger les données des stations et des connctions
    generic.read_stations("s.csv");
    generic.read_connections("c.csv");

    // Demander à l'utlisateur de saisir les deux station de départ et d'arriver
    std::string _start(""), _end("");
    do{
        std::cout << "Saisisez le nom de la station de depart : "; std::cin >> _start;
        if(_start=="")
            std::cout << "Vous n'avez rien saisie!" << std::endl;
    }while(_start=="");

    do{
        std::cout << "Saisisez le nom de la station d'arriver : "; std::cin >> _end;
        if(_end=="")
            std::cout << "Vous n'avez rien saisie!" << std::endl;
    }while(_end=="");

    // Trouver le plus court chemin
    try{
    generic.compute_and_display_travel(_start, _end); //2062 //1638
    }catch(std::string const& s){
        std::cout << s << std::endl;
    }

    return EXIT_SUCCESS;
}

