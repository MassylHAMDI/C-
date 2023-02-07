#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>


#include "Generic_mapper.hpp"
#include "Generic_connection_parser.hpp"
using namespace travel;



namespace travel {

    class Generic_station_parser_derivative : public Generic_mapper {
    public:
        std::vector<std::pair<uint64_t, uint64_t>> compute_travel(uint64_t _start, uint64_t _end) override;
        std::vector<std::pair<uint64_t, uint64_t>> compute_and_display_travel(uint64_t _start, uint64_t _end) override;

        std::vector<std::pair<uint64_t, uint64_t>> compute_travel(const std::string&, const std::string&) override;
        std::vector<std::pair<uint64_t, uint64_t>> compute_and_display_travel(const std::string& _start, const std::string& _end) override;

        void read_connections(const std::string& _filename) override;
        void read_stations(const std::string& _filename) override;

    };


    void Generic_station_parser_derivative::read_stations(const std::string& _filename) {
        std::string name_file(_filename);
        std::ifstream in(name_file.c_str());

        if (in.fail())
            std::cerr << "ERROR : Unable to read the file " << std::endl;
        else {
            // On ne prent pas en compte la 1ère ligne
            std::string row; std::getline(in, row, '\n');
            travel::Station station;
            while (!in.eof()) {  //in.good()
                std::getline(in, station.name, ',');
                std::getline(in, station.line_id, ',');
                std::getline(in, station.line_name, ',');
                std::getline(in, station.address, '\n');
                this->stations_hashmap.insert(std::make_pair(stoi(station.line_id), station));
            }
        } in.close();

        /// Affichage des -- station--
        for(auto const& it : this->stations_hashmap)
            std::cout << it.first << " : " << it.second << std::endl;
    }



    void Generic_station_parser_derivative::read_connections(const std::string& _filename) {
        std::string name_file(_filename);
        std::ifstream in(name_file.c_str());

        std::string from_stop_id;
        std::string to_stop_id;
        std::string min_transfer_time;

        if (in.fail())
            std::cerr << "ERROR : Unable to read the file " << std::endl;
        else {
            // On ne prend pas en compte la 1ère ligne
            std::string row; std::getline(in, row,  '\n');
            while (!in.eof()) { // in.good()
                std::getline(in, from_stop_id, ',');
                std::getline(in, to_stop_id, ',');
                std::getline(in, min_transfer_time, '\n');

                uint64_t from_stop_id_int =  std::strtoul(from_stop_id.c_str(), NULL, 10);
                uint64_t to_stop_id_int = std::strtoul(to_stop_id.c_str(), NULL, 10);
                uint64_t min_transfer_time_int = std::strtoul(min_transfer_time.c_str(), NULL, 10);

                if (connections_hashmap.count(from_stop_id_int)) {
                    this->connections_hashmap.at(from_stop_id_int)[to_stop_id_int] = min_transfer_time_int;
                }

                else {
                    std::unordered_map<uint64_t,uint64_t> temp_hashmap;
                    temp_hashmap[to_stop_id_int] = min_transfer_time_int;
                    this->connections_hashmap[from_stop_id_int] = temp_hashmap;
                }
            }
        } in.close();

          /// Affichage des -- affichage des conections --
        for (auto IT : this->connections_hashmap)
        {
            std::cout << "ID1 : " << IT.first << ", ";
            for (auto ITB : IT.second)
            {
                std::cout << "ID2 : " << ITB.first << ", TIME : " << ITB.second << std::endl;
            }
        }
    }



    std::vector<std::pair<uint64_t, uint64_t> > Generic_station_parser_derivative::compute_travel(uint64_t _start, uint64_t _end) {

        /*  Cette variable sera utilisée pour initiaiser les distances des toutes les stations
         *  par rapport à la station source
         */
        const uint64_t INF (10000000);

        /*  cette variable contient l'identifiant de la station courrante
         *  par defaut elle prend l'identifiant de la stationd de depart
         */
         uint64_t stat_courante(_start);

        /*  cette variable est "true" si la station final est trouvée si non false
         */
        bool trouvee = false;

        /*  De préférance est de travailler avec une référence constante de "connections_hashmap" et "station_hashmap"
         *  pour eviter de modifier leurs comptenus. pour ce fair on va utiliser la méthode get_connection_hashmap() et get_station_hashmap()
         */
        std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> > connections_hashmap = get_connections_hashmap();
        std::unordered_map<uint64_t, Station> stations_hashmap = get_stations_hashmap();

        /*  Un vecteur de paires pour sauvgarder les station à emprintées pour allée
         *  d'une station de départ à une station d'arrivée
         */
        std::vector<std::pair<uint64_t, uint64_t>> stations_empruntees;

        /*  "unordred_map" contenant les identidiants ainsi que les distances de chaque
         *  station par rapport à la station de départ.
         *  key : idenstifiant de la station
         *  val : distance de la station par rapport a la station de départ
         */
        std::unordered_map<uint64_t, uint64_t> dist;

        /*  Initialisation distances  de la station de depart par rapport à elle meme à 0
         *  Initialisation des distances la station de depart par rapport à toutes les stations à INF
         */
        for (auto const&station : stations_hashmap)
            dist[station.first] = INF;
        dist[_start] = 0;

        /*  Un "unordred_map" pour stocker toutes les stations visitée lors des iteration de
         *  l'algorithme de djikstra
         */
        std::unordered_map<uint64_t, uint64_t> stations_visitees;

        /*  Dans un vetor<int> on sauvgarde les identifients des stations non visitées
         *  ainsi que leur distance par defaut le vector contien toutes les sations et leur distance
         *  par rapport à la station de départ
         */
        std::vector<uint64_t> stations_non_visitees;
        for(auto const&station : stations_hashmap)
            stations_non_visitees.emplace_back(station.first);

    /*  Cette boucle cherche le la station final et stock toute les station visitées
     *  dans "un unordred_map"
     */
    while(stations_non_visitees.size()){

        std::vector<std::pair<uint64_t, uint64_t>> dist_id;
        for (auto const& station : stations_non_visitees)
            dist_id.emplace_back(std::make_pair(dist[station], station));

        /*  trier le vecteir dans l'ordre croissant pour récuperer facilement la
         *  station la plus proche
         */
        std::sort(dist_id.begin(), dist_id.end());
        uint64_t stat_courante((*(dist_id.begin())).second);

        // Quitez la boucle si on trouve la station final
        if (stat_courante == _end){
            trouvee = true;
            break;
        }

        /*  Supprimer la station récuperée dans le vecreur stations_non_visitees
         */
        auto it = std::find(stations_non_visitees.begin(), stations_non_visitees.end(), stat_courante);
        stations_non_visitees.erase(it);

        /*  Stocker toutes les station visitées par l'lgorithme de djikstra, pour au final
         * recuperer le plus court chemin à emprinter
         */
        for (auto const &station : connections_hashmap[stat_courante]){
            if (dist[station.first] > (dist[stat_courante] + station.second)){
                dist[station.first] = (dist[stat_courante] + station.second);
                stations_visitees[station.first] = stat_courante;
            }
        }
    }


    /*  Cette boucle nous permet de récupérer toutes les stations à emprinté pour
     *  pour arriver enfin à la station finale (plus court chemin)
     */
    stat_courante = _end;
    while(stat_courante != _start) {
        stations_empruntees.insert(stations_empruntees.begin(), std::make_pair(stat_courante, dist[stat_courante]));
        stat_courante = stations_visitees[stat_courante];
    }

        return stations_empruntees;
   }



    std::vector<std::pair<uint64_t,uint64_t> > Generic_station_parser_derivative::compute_and_display_travel(uint64_t _start, uint64_t _end){

        std::vector<std::pair<uint64_t,uint64_t> > path = compute_travel(_start, _end);
        std::unordered_map<uint64_t, Station> stations_hashmap = get_stations_hashmap();

        std::cout << "Le meilleur moyen de se rendre de "
                  << stations_hashmap.at(_start).name       << " (ligne "
                  << stations_hashmap.at(_start).line_name << ") a "
                  << stations_hashmap.at(_end).name   << " (ligne "
                  << stations_hashmap.at(_end).line_name << ") "
                  << "est : " << std::endl;

        std::cout << "------------------------------------------------------------" << std::endl;
        for (int i = 0; i < (int)path.size()-1; i++){

            uint64_t station_id1(path[i].first);
            Station station1(stations_hashmap[station_id1]);

            uint64_t station_id2(path[i+1].first);
            Station station2(stations_hashmap[station_id2]);

            bool afficher(true);
            if (i>0) {
                uint64_t station_id(path[i-1].first);
                Station station(stations_hashmap[station_id]);
                afficher = station1.line_name != station.line_name;
            }
            if(afficher or (i==path.size()-2) or !i) {
                std::cout << "Marcher jusqu'a " << station1.name << " ligne " << station1.line_name << " (" << path[i].second << " sec)" << std::endl;
                std::cout << "Prendre la ligne " << station1.line_name << " " << station1.address << std::endl;
                // std::cout << "De " << station1.name << " a " << station1.name <<" (" << path[i+1].second << " sec)" << std::endl;

                for (int j(i); j < (int)path.size()-2; ++j){
                    if(stations_hashmap[path[j].first].line_name != stations_hashmap[path[j+1].first].line_name) {
                        std::cout << "De " << station1.name << " a " << stations_hashmap[path[j+1].first].name <<" (" << path[i+1].second << " sec)" << std::endl;
                        break;
                    }
                }

            }

        }
        return path;
    }


    std::vector<std::pair<uint64_t, uint64_t> > Generic_station_parser_derivative::compute_travel(const std::string& _start, const std::string& _end) {
        // deux variable pour recuperer les identifiants des stations d'etrers
        uint64_t start_id(0), end_id(0);

        // une boucke pour trouver
        for (auto station : this->stations_hashmap){
            if (station.second.name == _start)
                start_id = station.first;
            if (station.second.name == _end)
                end_id = station.first;
        }

        if (!start_id or !end_id)
            throw "Stations introuvables";
        return compute_travel(start_id, end_id);
    }


    std::vector<std::pair<uint64_t, uint64_t> > Generic_station_parser_derivative::compute_and_display_travel(const std::string& _start, const std::string& _end) {
        uint64_t start_id(0), end_id(0);

        for(auto station : this->stations_hashmap){
            if (station.second.name == _start)
                start_id = station.first;

            if (station.second.name == _end)
                end_id = station.first;
        }

        if (!start_id or !end_id)
            throw "Stations introuvables";

        return compute_and_display_travel(start_id, end_id);
    }
}
