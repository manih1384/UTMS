// command.hpp
#pragma once
#include <vector>
#include <string>
#include "user.hpp"
#include "courses.hpp"
#include "units.hpp"
using namespace std;
class System;
class Command
{
public:
    virtual ~Command() = default;
    virtual string get_type() = 0;
    virtual vector<string> get_valid_commands() = 0;
    virtual void execute(string action) = 0;
    Command(System &system) : system(system) {}

protected:
    System &system;
    vector<string> commands;
};

class GetCommand : public Command
{
public:
    GetCommand(System &system) : Command(system) {}
    string get_type() override { return "GET"; }
    vector<string> get_valid_commands() override;
    void execute(string action) override;
    void get_courses();
    void personal_page(vector<string> line);
    void view_post(vector<string> line);
    void view_notifications(vector<string> line);

};

class PostCommand : public Command
{
public:
    PostCommand(System &system) : Command(system) {}
    string get_type() override { return "POST"; }
    vector<string> get_valid_commands() override;
    void login();
    void logout();
    void connect(string id);
    void execute(string action) override;
    void post(vector<string> line);
    void course_offer(vector<string> line);
private:
}; 

class PutCommand : public Command
{
public:
    PutCommand(System &system) : Command(system) {}
    string get_type() override;
    vector<string> get_valid_commands() override;
    void execute(string action) override;
};

class DeleteCommand : public Command
{
public:
    DeleteCommand(System &system) : Command(system) {}
    string get_type() override;
    vector<string> get_valid_commands() override;
    void execute(string action) override;
    void delete_post( vector<string> line);
};
