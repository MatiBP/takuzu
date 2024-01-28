#!/bin/bash


TAKUZU_EXECUTABLE="./src/takuzu"



number=1


read -e -p "Entrez le répertoire pour les tests: " test_directory

# Parcourez les fichiers de grille de test
for test_file in ./tests/$test_directory/*; do
    echo "################# DEBUT TEST $number #########################"
    echo "Testing grid: $test_file"
    $TAKUZU_EXECUTABLE "$test_file"
    echo
    echo "################# FIN TEST $number #########################"
    echo
    ((number++))
done


