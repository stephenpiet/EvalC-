#include "todo.h"

#include <iostream>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <cmath>


void get_value(int& i, int n, char *argv[], std::string& val){      // sépare les guillemets des charactères 

    if (argv[i][0] != '"'){
        val = argv[i];
        i++;
        return;
    }
    
    bool cont = true;

    while(i < n && cont){
        std::string buf = argv[i];
        if (buf.at(0) == '"'){
            buf.erase(0,1);
        } else {
            buf = " " + buf;
        }
        if (buf.back() == '"'){
            buf.pop_back();
            cont = false;
        }
        val += buf;
        i++;
    } 

}

void parse_cli(int argc, char *argv[], Task& t, std::map<std::string, bool>& cli_provided, int &id, int &sub_id, std::string& com){         // création d'une interface de commandes

    int i = 0;
    while(i < argc){

        std::string field = argv[i];
        if (argv[i][0] != '-' || argv[i][1] != '-'){
            std::cout << "Argument error" << std::endl;
            exit(0);
        }
        i++;

        std::string value;
        get_value(i, argc, argv, value);

        field.erase(0,2);

        if (field == "title"){          // gestion des différentes options de modifications
                cli_provided["title"] = true;
                t.title = value;
        } else if (field == "desc" || field == "description") {
                cli_provided["desc"] = true;
                t.desc = value;
        } else if(field == "status") {
                cli_provided["status"] = true;
                if (value == "open" || value == "Open") t.status = 0;
                else if (value == "closed" || value == "Closed") t.status = 1;
        } else if (field == "advancement") {
                cli_provided["advancement"] = true;
                int adv = std::stoi(value);
                t.advancement = adv;
        } else if (field == "priority") {
                cli_provided["priority"] = true;
                int pri = 0;
                if (value == "low") pri = 1;
                else if (value == "med" || value == "medium") pri = 5;
                else if (value == "high") pri = 10;
                else if (value == "urgent") pri = 20;
                else pri = std::stoi(value);
                t.priority = pri;
        } else if (field == "id" || field == "ID" || field == "base-id") {
                id = std::stoi(value);
        } else if (field == "subtask-id" || field == "sub-id" || field == "index") {
                sub_id = std::stoi(value);
        } else if (field == "comment") {
                com = value;
        } else {
                std::cerr << "Unrecognized parameter '" << field << "'" << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {

    std::string db = "db.txt";  
    TaskManager todo = TaskManager(db);         //on charge les données du fichier db 

    if (argc < 2) return 0;

    std::map<std::string, bool> cli_provided;       //  initialisation des données
    Task t = Task(0, "", "", 0, 0, 0);
    int id = -1;
    int sub_id = -1;
    std::string com = ""; 

    std::string command = argv[1];

    if (command == "create") {          //gestion des différentes actions possibles
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            int new_id = todo.create_task(t); 
            std::cout << "New task created with ID " << new_id << std::endl;
            std::cout << todo.get(new_id)->to_string() << std::endl;
    } else if (command == "modify") {
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1){
                std::cerr << "--id parameter necessary for modify" << std::endl;
                exit(0);
            }
            Task *t_get = todo.get(id);
            if (t_get == 0){
                std::cerr << "No task with ID " << id << std::endl;
                exit(0);
            }
            todo.modify_task(id, t, cli_provided);
            std::cout << t_get->to_string() << std::endl;
    } else if (command == "list") {
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            std::vector<Task *> tasks = todo.list_task(t, cli_provided);
            std::cout << "Found " << tasks.size() << " results" << std::endl;
            for (Task *t : tasks){
                std::cout << t->to_string() << std::endl;
            }
    } else if (command == "close") {
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1){
                std::cerr << "--id parameter necessary for close" << std::endl;
                exit(0);
            }
            todo.close_task(id);
            std::cout << todo.get(id)->to_string() << std::endl;
    } else if (command == "add-subtask"){
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1 || sub_id == -1){
                std::cerr << "--base-id and --sub-id parameters necessary for adding a subtask" << std::endl;
                exit(0);
            }
            todo.add_subtask(id, sub_id);
            std::cout << todo.get(id)->to_string() << std::endl;
    } else if (command == "remove-subtask"){
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1 || sub_id == -1){
                std::cerr << "--base-id and --sub-id parameters necessary for removing a subtask" << std::endl;
                exit(0);
            }
            todo.remove_subtask(id, sub_id);
            std::cout << todo.get(id)->to_string() << std::endl;
    } else if (command == "add-comment"){
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1 || com == ""){
                std::cerr << "--id and --comment parameters necessary for adding a comment" << std::endl;
                exit(0);
            }
            todo.add_comment(id, com);
            std::cout << todo.get(id)->to_string() << std::endl;
    } else if (command == "remove-comment"){
            parse_cli(argc-2, argv+2, t, cli_provided, id, sub_id, com);
            if (id == -1 || sub_id == -1){
                std::cerr << "--id and --index parameters necessary for removing a comment" << std::endl;
                exit(0);
            }
            todo.remove_comment(id, sub_id);
            std::cout << todo.get(id)->to_string() << std::endl;
    } else {
            std::cerr << "Unknown command" << std::endl;
            exit(0);
    }
    
    todo.commit_to_db();
}


