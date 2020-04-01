# Todo list manager

## Description


## Compiler

g++ -c main.cc todo.cc
g++ main.o todo.o -o todo
rm *.o


## Donnees

Le fichier db.txt doit etre initialise a '0'.

## Commandes

[A REECRIRE]

./todo create [params] -> creer une tache
./todo modif [params] -> modifier une tache
./todo list [params] -> liste des taches satisfiant les conditions
./todo close --id N -> fermer la tache d'identifiant N
./todo add-subtask --base-id A --sub-id B -> Ajout de la sous-tache B pour la tache A /\ B DOIT DEJA AVOIR ETE CREE AVEC ./todo create
./todo remove-subtask --base-id A --sub-id B -> Suppresion de la sous tache B pour la tache A
./todo add-comment --base-id A --comment "Comment" -> Ajout d'un commentaire pour la tache d'identifiant A
./todo remove-comment --base-id A --index N > Suppression du commentaire #N pour la tache d'identifiant A


### Parametres

--title "Titre ICI" -> titre
--desc "Desription ICI" -> description
--priority low|medium|high|urgent -> priorite
--status open|closed -> statut
--advancement N -> Avancement (N entier entre 0 et 100)
--id N -> identifiant N


