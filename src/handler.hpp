#ifndef HANDLERS_HPP_INCLUDE
#define HANDLERS_HPP_INCLUDE

#include <map>
#include <string>
#include "system.hpp"
#include "../server/server.hpp"
#include "global.hpp"



class LoginHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    LoginHandler(System*system);
private:
    System*system;
};

class LogoutHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    LogoutHandler(System*system);
private:
    System*system;
};






class HomePageHandler: public RequestHandler {
public:
    Response*callback(Request*req) override;
    HomePageHandler(System*system);
private:
    System*system;
};


class PostHandler: public RequestHandler {
public:
    Response*callback(Request*req) override;
    PostHandler(System*system, Server & server);
private:
    System*system;
    Server& server;
};


class ProfileHandler: public RequestHandler {
public:
    Response*callback(Request*req) override;
    ProfileHandler(System*system,Server &server);
private:
    System*system;
    Server& server;
};




class FindUserHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    FindUserHandler(System*system,Server &server);
private:
    System*system;
    Server& server;
};


class PersonalPageHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    PersonalPageHandler(shared_ptr<User> user,System*system);
private:
    System*system;
    shared_ptr<User> user;
};


class AddCourseHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    AddCourseHandler(System*system);
private:
    System*system;
};


class ShowAllCoursesHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    ShowAllCoursesHandler(System*system);
private:
    System*system;
};



class TakeCourseHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    TakeCourseHandler(System*system);
private:
    System*system;
};

class ShowMyCoursesHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    ShowMyCoursesHandler(System*system);
private:
    System *system;
};


class DeleteCourseHandler : public RequestHandler {
public:
    Response*callback(Request*) override;
    DeleteCourseHandler(System*system);
private:
    System*system;
};


#endif // HANDLERS_HPP_INCLUDE

