#!/bin/bash

# Demande à l'utilisateur le répertoire pour les tests
read -p "Entrez le répertoire pour les tests: " test_directory

# Demande le nom des tests
read -p "Entrez le nom des tests: " test_name

# Demande le nombre de tests (inférieur à 1000)
read -p "Entrez le nombre de tests (inférieur à 1000): " num_tests

# Vérifie que le nombre de tests est inférieur à 1000
if [ "$num_tests" -ge 1000 ]; then
    echo "Erreur: Le nombre de tests doit être inférieur à 1000."
    exit 1
fi

# Demande la taille de la grille
read -p "Entrez la taille de la grille: " grid_size

# Crée un répertoire pour les résultats des tests
results_directory="./tests/$test_directory"
mkdir -p "$results_directory"


# Exécute les tests
for ((i=1; i<=$num_tests; i++)); do
    # Pourcentage de remplissage aléatoire entre 0 et 100
	fill_percentage=$((RANDOM % 101))
	test_output="${test_name}_test$i"
	./src/takuzu -g$grid_size -N$fill_percentage > $results_directory/$test_output
    	echo "Test $i terminé. Résultats dans $test_output"
done

echo "Tous les tests terminés."

