// command.hpp
#pragma once
#include <vector>
#include <string>
#include "user.hpp"
#include "courses.hpp"
#include "units.hpp"
#include "utilityfunctions.hpp"
#include <memory>
using namespace std;
class System;
class Command
{
public:
    virtual ~Command() = default;
    virtual string get_type() = 0;
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
    void execute(string action) override;
    void get_courses(vector<string> line);
    void personal_page(vector<string> line);
    void view_post(vector<string> line);
    void view_notifications(vector<string> line);
    void view_courses(vector<string> line);
    void course_channel(vector<string> line);
    void course_post(vector<string> line);
};

class PostCommand : public Command
{
public:
    PostCommand(System &system) : Command(system) {}
    string get_type() override { return "POST"; }
    void login();
    void logout();
    void connect(string id);
    void execute(string action) override;
    void post(vector<string> line);
    void course_offer(vector<string> line);
    void profile_photo(vector<string> line);
    void course_post(vector<string> line);
    void ta_form(vector<string> line);
    void ta_request(vector<string> line);
    void close_ta_form(vector<string> line);
    void process_request(shared_ptr<Student> student,shared_ptr<Course> course);

private:
};

class PutCommand : public Command
{
public:
    PutCommand(System &system) : Command(system) {}
    string get_type() override;
    void execute(string action) override;
    void register_course(vector<string> line);
};

class DeleteCommand : public Command
{
public:
    DeleteCommand(System &system) : Command(system) {}
    string get_type() override;
    void execute(string action) override;
    void delete_post(vector<string> line);
    void delete_course(vector<string> line);
};
