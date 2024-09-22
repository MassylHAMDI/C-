# Projet RATP - Système de Navigation du Métro Parisien

## Description
Ce projet implémente un système de navigation pour le réseau du métro parisien. Il permet aux utilisateurs de trouver le chemin optimal entre deux stations, en prenant en compte les connexions et les temps de transfert.
## Démonstration
Une vidéo de démonstration du projet:

https://github.com/user-attachments/assets/2fd1c7ee-cafd-435e-87be-44ebb864ddb9

## Fonctionnalités
- Chargement des données des stations et des connexions à partir de fichiers CSV
- Calcul du chemin le plus court entre deux stations
- Interface graphique pour une interaction facile avec l'utilisateur
- Affichage détaillé du trajet, incluant les changements de ligne et les temps de transfert

## Prérequis
- C++17 ou supérieur
- CMake 3.10 ou supérieur

## Installation
1. Clonez le dépôt :
   ```
   git clone https://github.com/votre-nom/RATP-PROJET.git
   cd RATP-PROJET
   ```

2. Créez un répertoire de build et naviguez dedans :
   ```
   mkdir build && cd build
   ```

3. Configurez le projet avec CMake :
   ```
   cmake ..
   ```

4. Compilez le projet :
   ```
   cmake --build .
   ```

## Utilisation
Lancez l'exécutable généré. Dans l'interface graphique :
1. Entrez le nom de la station de départ
2. Entrez le nom de la station d'arrivée
3. Cliquez sur "Trouver le chemin"
4. Le résultat s'affichera, montrant le trajet optimal

## Structure du Projet
- `src/` : Contient les fichiers source (.cpp)
- `include/` : Contient les fichiers d'en-tête (.hpp)
- `data/` : Contient les fichiers CSV avec les données des stations et des connexions
- `CMakeLists.txt` : Fichier de configuration CMake
- `README.md` : Ce fichier
