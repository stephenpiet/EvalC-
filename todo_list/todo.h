// TODO app
//

#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>


class Task
{
public:
    Task(int id, std::string title, std::string desc, bool status, int advancement, int priority);
    void add_subtask(int id);
    int remove_subtask(int id);
    void add_comment(std::string com);
    void remove_comment(int index);
    void close();
    std::string to_string();

    // Attributes
    int id;
    std::string title;
    std::string desc;
    time_t creation_time;
    time_t close_time;
    bool status;
    int advancement;
    int priority;
    std::vector<std::string> comments;
    std::set<int> subtasks;
};


class TaskManager
{
public:
    TaskManager(std::string db_file);
    int create_task(Task temp);
    int modify_task(int id, Task temp, std::map<std::string, bool> is_changed);
    std::vector<Task *> list_task(Task temp, std::map<std::string, bool> is_changed);
    int close_task(int id);
    void del_task(int id);
    int add_subtask(int id_base, int id_sub);
    int remove_subtask(int id_base, int id_sub);
    int add_comment(int id_base, std::string com);
    int remove_comment(int id_base, int idx);
    int commit_to_db();
    Task *get(int id);

private:
    std::map<int, Task*> tasks;
    std::string db;

    int load();
    int dump();
};
