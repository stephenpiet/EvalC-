#!/bin/bash

mv db.txt db.backup;
echo 0 > db.txt;

# Set of tests for the todo list binary

echo "Create : ./todo create --title \"Finish this project\" --priority urgent --advancement 75";
echo "OUTPUT : ";
./todo create --title "Finish this project" --priority urgent --advancement 75;

echo "Create : ./todo create --title \"Create a git repo\" --priority medium --advancement 0";
echo "OUTPUT : ";
./todo create --title "Create a git repo" --priority medium --advancement 0;

echo "Modify : ./todo modify --id 1 --desc \"Go to www.github.com\"";
echo "OUTPUT : ";
./todo modify --id 1 --desc "Go to www.github.com";

echo "Add subtask : ./todo add-subtask --base-id 0 --sub-id 1";
echo "OUTPUT : ";
./todo add-subtask --base-id 0 --sub-id 1;

echo "Close task : ./todo close --id 0";
echo "OUTPUT : ";
./todo close --id 0;

echo "Close task : ./todo close --id 1";
echo "OUTPUT : ";
./todo close --id 1;

echo "List : ./todo list --status open";
echo "OUTPUT : ";
./todo list --status open;

echo "Add comment : ./todo add-comment --id 0 --comment \"Now that the repo is created I can finish my project\"";
echo "OUTPUT : ";
./todo add-comment --id 0 --comment "Now that the repo is created I can finish my project";

echo "Close task : ./todo close --id 0";
echo "OUTPUT : ";
./todo close --id 0;

echo "Final output of ./todo list : ";
./todo list;

mv db.backup db.txt;
