cmake_minimum_required(VERSION 3.10)
project(RATP_PROJET)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Afficher des informations de débogage
message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "Project binary dir: ${PROJECT_BINARY_DIR}")

# Inclure les répertoires d'en-têtes
include_directories(${PROJECT_SOURCE_DIR}/include)

# Récupérer tous les fichiers sources
file(GLOB SOURCES "src/*.cpp")

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Copier le dossier data dans le répertoire de build
file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${PROJECT_BINARY_DIR}/Debug )

# Ajouter une cible personnalisée pour vérifier le contenu du dossier data
add_custom_target(check_data_folder
    COMMAND ${CMAKE_COMMAND} -E echo "Contents of build/data:"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/list_directory.cmake"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

# Faire dépendre l'exécutable de la vérification du dossier data
add_dependencies(${PROJECT_NAME} check_data_folder)

# Définir le répertoire de travail pour l'exécutable
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
)

# Ajouter des options de compilation pour le débogage
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-DDEBUG -g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Afficher un message à la fin de la configuration
message(STATUS "CMake configuration completed. You can now build the project.")