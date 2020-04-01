// Todo app
//

#include <iostream>
#include <ctime>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

#include "todo.h"

Task::Task(int _id, std::string _title, std::string _desc, bool _status, int _advancement, int _priority) // création d'une classe tâche
{

    id = _id;
    title = _title;
    desc = _desc;
    creation_time = time(0);
    status = _status;
    advancement = _advancement;
    priority = _priority;
}


void Task::add_subtask(int id){     // ajout sous-tâche
    subtasks.insert(id);
}

int Task::remove_subtask(int id){   // supression sous-tâche
    return subtasks.erase(id);
}

void Task::add_comment(std::string com){    // ajout commentaire
    comments.push_back(com);
}

void Task::remove_comment(int index){   // suppression commentaire
    comments.erase(comments.begin() + index);
}

void Task::close(){     //fermer tâche
    close_time = time(0);
    advancement = 100;
    status = true;
}

std::string Task::to_string(){  //permet l'affichage des taches pour l'utilisateur

    std::string rslt = "ID : " + std::to_string(id) + "\n"; //convertit int en string
    rslt       += "Title : " + title + "\n";
    rslt       += "Description : " + desc + "\n";
    rslt       += "Creation time : " + std::string(ctime(&creation_time));
    rslt       += "Advancement : " + std::to_string(advancement) + "%\n";
    rslt       += "Priority : " + std::to_string(priority) + "\n";
    if (status) rslt += "Status : Closed\n";
    else rslt += "Status : Open\n";

    if (status){        
        rslt += "Close time : " + std::string(ctime(&close_time));
    }

    if (subtasks.size()){       
        rslt += "Subtasks : \n";
        for (int i : subtasks){
            rslt += "\tID " + std::to_string(i) + "\n";
        }
    }

    if (comments.size()){
        rslt += "Comments : \n";
        for(int i = 0; i < comments.size(); i++){
            rslt += "\tComment #" + std::to_string(i) + " : " + comments[i] + "\n";
        }
    }

    return rslt;
}


// Manage

TaskManager::TaskManager(std::string db_file){      // création d'une liste d'une tâche depuis le fichier db
    db = db_file;
    load();
}


Task* TaskManager::get(int id){     //permet de récupérer une tâche grace à l'ID

    std::map<int, Task*>::iterator it;     
    it = tasks.find(id);
    if (it == tasks.end()) return 0;

    return it->second;

}

int TaskManager::create_task(Task temp){         
    int new_id = tasks.size();
    tasks.insert(std::pair<int, Task*>(new_id, new Task(new_id, temp.title, temp.desc, temp.status, temp.advancement, temp.priority)));
    return new_id;
}

int TaskManager::modify_task(int id, Task temp, std::map<std::string, bool> is_changed){

    Task* t = get(id);
    if (!t) return 0;
    
    if (is_changed["title"]) t->title = temp.title;
    if (is_changed["desc"]) t->desc = temp.desc;
    if (is_changed["advancement"]) t->advancement = temp.advancement;
    if (is_changed["priority"]) t->priority = temp.priority;

    return 1;
}

std::vector<Task *> TaskManager::list_task(Task temp, std::map<std::string, bool> is_changed){ // retourne la liste de tâches qui correspondent à un critère
    
    std::vector<Task *> rtn;

    std::map<int, Task*>::iterator it = tasks.begin();

    while (it != tasks.end()){
        int id = it->first;
        Task *t = it->second;

        it++;

        if (is_changed["title"] && t->title != temp.title) continue;
        if (is_changed["desc"] && t->desc != temp.desc) continue;
        if (is_changed["advancement"] && t->advancement != temp.advancement) continue;
        if (is_changed["priority"] && t->priority != temp.priority) continue;
        if (is_changed["status"] && t->status != temp.status) continue;

        rtn.push_back(t);
    }

    return rtn;
}

int TaskManager::close_task(int id){

    // Vérifie que toutes les tâches sont fermées
    Task *t = get(id);
    if (!t) return 0;

    for (int id_sub : t->subtasks){
        Task *t_sub = get(id_sub);
        if (!t_sub) continue;
        if (t_sub->status) continue;

        std::cout << "You need to complete Task #" << std::to_string(id_sub) << " in order to complete this task" << std::endl;
        return 0;
    }

    t->close();
    return 1;
}

void TaskManager::del_task(int id){
    tasks.erase(id);
}

int TaskManager::add_subtask(int id_base, int id_sub){

    Task *t_base = get(id_base);
    if (!t_base) return 0;

    t_base->add_subtask(id_sub);
    return 1;
}


int TaskManager::remove_subtask(int id_base, int id_sub){

    Task *t_base = get(id_base);
    if (!t_base) return 0;

    return t_base->remove_subtask(id_sub);
}


int TaskManager::add_comment(int id_base, std::string com){

    Task *t_base = get(id_base);
    if (!t_base) return 0;

    t_base->add_comment(com);
    return 1;
}


int TaskManager::remove_comment(int id_base, int idx){

    Task *t_base = get(id_base);
    if (!t_base) return 0;

    t_base->remove_comment(idx);
    return 1;
}


int TaskManager::commit_to_db(){        // exécute dump
    return dump();
}



int TaskManager::dump(){        //stockage de données dans le fichier db

    std::ofstream db_file;
    db_file.open(db);

    db_file << tasks.size() << std::endl;
    
    std::map<int, Task*>::iterator it = tasks.begin();
    while (it != tasks.end()){
        int id = it->first;
        Task *t = it->second;
        it++;

        // On écrit dans le fichier
        
        db_file << t->id << std::endl;
        db_file << t->title << std::endl;
        db_file << t->desc << std::endl;
        db_file << t->status << std::endl;
        db_file << t->advancement << std::endl;
        db_file << t->priority << std::endl;
        db_file << t->creation_time << std::endl;
        
        if (t->status) db_file << t->close_time << std::endl;
        
        db_file << t->comments.size() << std::endl;
        for (std::string com : t->comments){
            db_file << com << std::endl;
        }

        db_file << t->subtasks.size() << std::endl;
        for (int id_sub : t->subtasks){
            db_file << id_sub << std::endl;
        }
    }

    db_file.close();
    return 1;
}


int TaskManager::load(){        //on récupère les données de db, et on reconstruit la liste de tâches

    std::ifstream db_file;
    db_file.open(db);

    std::string buf;
    int number_of_tasks = 0;

    std::getline(db_file, buf);
    number_of_tasks = std::stoi(buf);

    for (int i = 0; i < number_of_tasks; i++){

        int id;
        std::string title;
        std::string desc;
        bool status;
        int advancement;
        int priority;
        time_t creation_time;
        time_t close_time;
        
        int subtask_count;
        int comment_count;

        std::getline(db_file, buf);
        id = std::stoi(buf);
        
        std::getline(db_file, title);
        std::getline(db_file, desc);

        std::getline(db_file, buf);
        status = std::stoi(buf);

        std::getline(db_file, buf);
        advancement = std::stoi(buf);

        std::getline(db_file, buf);
        priority = std::stoi(buf);

        std::getline(db_file, buf);
        creation_time = std::stoll(buf);
        
        if (status){
            std::getline(db_file, buf);
            close_time = std::stoll(buf);
        }

        Task *t = new Task(id, title, desc, status, advancement, priority);         // on creer une nouvelke tache avec données récupérée

        t->creation_time = creation_time;
        if (status) t->close_time = close_time;

        std::getline(db_file, buf);
        comment_count = std::stoi(buf);

        for (int i = 0; i < comment_count; i++){
            std::string com;
            std::getline(db_file, com);
            t->add_comment(com);
        }

        std::getline(db_file, buf);
        subtask_count = std::stoi(buf);

        for (int i = 0; i < subtask_count; i++){
            int id_sub;
            std::getline(db_file, buf);
            id_sub = std::stoi(buf);         
            t->add_subtask(id_sub);
        }

        tasks[id] = t;
    }

    db_file.close();
    return 1;
}
