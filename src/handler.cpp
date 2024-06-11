#include "../src/handler.hpp"
#include <cstdlib>
#include <iostream>
#include "errors.hpp"

LoginHandler::LoginHandler(System *system)
    : system(system) {}

Response *LoginHandler::callback(Request *req)
{

    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");
    try
    {

        string type = system->login(username, password);
        Response *res = Response::redirect("/homepage");
        res->setSessionId(type);
        return res;
    }
    catch (NotFoundError &e)
    {
        Response *res = Response::redirect(NOT_FOUND_PAGE);
        return res;
    }
    catch (PermissionDeniedError &e)
    {
        Response *res = Response::redirect(PERM_DENIED_PAGE);
        return res;
    }
}

AddCourseHandler::AddCourseHandler(System *system)
    : system(system) {}

Response *AddCourseHandler::callback(Request *req)
{
    if (req->getSessionId() != "admin")
    {
        Response *res = Response::redirect("/Permission_error");
        return res;
    }

    std::string course_id = req->getBodyParam("course_id");
    std::string professor_id = req->getBodyParam("professor_id");
    std::string capacity = req->getBodyParam("capacity");
    std::string time = req->getBodyParam("time");
    std::string exam_date = req->getBodyParam("exam_date");
    std::string class_number = req->getBodyParam("class_number");
    try
    {

        system->set_course(course_id, professor_id, capacity, time, exam_date, class_number);
        Response *res = Response::redirect("/homepage");
        return res;
    }
    catch (NotFoundError &e)
    {
        Response *res = Response::redirect(NOT_FOUND_PAGE);
        return res;
    }
    catch (PermissionDeniedError &e)
    {
        Response *res = Response::redirect(PERM_DENIED_PAGE);
        return res;
    }
    catch (BadRequestError &e)
    {
        Response *res = Response::redirect(BAD_REQ_PAGE);
        return res;
    }
}

LogoutHandler::LogoutHandler(System *system)
    : system(system) {}

Response *LogoutHandler::callback(Request *req)
{
    system->logout();
    Response *res;
    res->setSessionId("");
    return res;
}

HomePageHandler::HomePageHandler(System *system)
    : system(system) {}

Response *HomePageHandler::callback(Request *req)
{
    Response *res = new Response();
    if (req->getSessionId() != "student" && req->getSessionId() != "admin" && req->getSessionId() != "professor")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    res->setHeader("Content-Type", "text/html");
    shared_ptr<User> user = system->get_user();
    std::string userName = user->get_name();
    std::string userMajor = user->get_major_name();
    std::string userId = user->get_id();
    std::string userPhoto = user->get_photo();
    std::string sessionId = req->getSessionId();

    std::string body;

    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";

    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    body += "  <title>Home - UTMS</title>";
    body += "  <link rel=\"stylesheet\" href=\"/style.css\">";
    body += "</head>";

    body += "<body>";
    body += "  <header>";
    body += "    <h1>Welcome to UTMS</h1>";
    body += "  </header>";
    body += "  <main>";
    body += "    <section class=\"user-info\">";
    body += "      <img src=\"" + userPhoto + "\" alt=\"Profile Picture\" class=\"profile-pic\">";
    body += "      <h2>" + userName + "</h2>";
    body += "      <p><strong>Major:</strong> " + userMajor + "</p>";
    body += "      <p><strong>ID:</strong> " + userId + "</p>";
    body += "    </section>";
    body += "    <section class=\"activities\">";
    body += "      <h2>Available Activities</h2>";
    body += "      <ul>";
    body += "        <li><a href=\"/send_post\" class=\"button\">Send post</a></li>";
    body += "        <li><a href=\"/set_profile\" class=\"button\">Set profile</a></li>";
    body += "        <li><a href=\"/find_user\" class=\"button\">Find user</a></li>";
    body += "        <li><a href=\"/all_courses\" class=\"button\">Show All courses</a></li>";
    if (req->getSessionId() == "admin")
    {
        body += "        <li><a href=\"/add_course\" class=\"button\">Add course</a></li>";
    }
    else if(req->getSessionId()=="student"){
        body += "        <li><a href=\"/take_course\" class=\"button\">Take course</a></li>";
        body += "        <li><a href=\"/delete_course\" class=\"button\">Delete course</a></li>";
        body += "        <li><a href=\"/my_courses\" class=\"button\">My courses</a></li>";
    }

    body += "        <li><a href=\"/logout\" class=\"button\">Logout</a></li>";
    body += "      </ul>";
    body += "  </main>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}

PostHandler::PostHandler(System *system, Server &server)
    : system(system), server(server) {}

Response *PostHandler::callback(Request *req)
{

    Response *res = new Response();
    if (req->getSessionId() != "student" && req->getSessionId() != "admin" && req->getSessionId() != "professor")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    shared_ptr<User> user = system->get_user();

    std::string title = req->getBodyParam("title");
    std::string message = req->getBodyParam("message");
    std::string image = req->getBodyParam("image");
    std::string image_name = user->get_id() + to_string(user->get_next_post_id());

    utils::writeToFile(image, "images/" + image_name);
    system->post(title, message, SLASH + image_name + PNG);

    server.get(SLASH + image_name + PNG, new ShowImage("images/" + image_name));
    res = Response::redirect("/homepage");
    return res;
}

ProfileHandler::ProfileHandler(System *system, Server &server)
    : system(system), server(server) {}

Response *ProfileHandler::callback(Request *req)
{
    Response *res = Response::redirect("/homepage");
    if (req->getSessionId() != "student" && req->getSessionId() != "admin" && req->getSessionId() != "professor")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    shared_ptr<User> user = system->get_user();
    std::string image_name = user->get_id();
    std::string image = req->getBodyParam("image");
    utils::writeToFile(image, "images/" + image_name);
    user->set_photo(SLASH + image_name + PNG);
    server.get(SLASH + image_name + PNG, new ShowImage("images/" + image_name));

    return res;
}

FindUserHandler::FindUserHandler(System *system, Server &server)
    : system(system), server(server) {}

Response *FindUserHandler::callback(Request *req)
{

    std::string username = req->getBodyParam("username");
    try
    {

        shared_ptr<User> user = system->find_user(username);
        server.get("/personal_page_" + user->get_id(), new PersonalPageHandler(user, system));
        Response *res = Response::redirect("/personal_page_" + user->get_id());

        return res;
    }
    catch (NotFoundError &e)
    {
        Response *res = Response::redirect(NOT_FOUND_PAGE);
        return res;
    }
}

PersonalPageHandler::PersonalPageHandler(shared_ptr<User> user, System *system)
    : system(system), user(user) {}

Response *PersonalPageHandler::callback(Request *req)
{

    Response *res = new Response();
    if (req->getSessionId() != "student" && req->getSessionId() != "admin" && req->getSessionId() != "professor")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    std::string userName = user->get_name();
    std::string userMajor = user->get_major_name();
    std::string userId = user->get_id();
    std::string userPhoto = user->get_photo();
    std::string sessionId = req->getSessionId();
    vector<shared_ptr<UT_media>> posts = user->get_posts();
    std::string body;
    res->setHeader("Content-Type", "text/html");
    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";

    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    body += "  <title>Home - UTMS</title>";
    body += "  <link rel=\"stylesheet\" href=\"/style.css\">";
    body += "</head>";

    body += "<body>";
    body += "  <header>";
    body += "    <h1>Welcome to UTMS</h1>";
    body += "  </header>";
    body += "  <main>";
    body += "    <section class=\"user-info\">";
    body += "      <img src=\"" + userPhoto + "\" alt=\"Profile Picture\" class=\"profile-pic\">";
    body += "      <h2>" + userName + "</h2>";
    if (user->type() == "professor")
    {
        shared_ptr<Professor> prof = dynamic_pointer_cast<Professor>(user);
        body += "      <p><strong>Major:</strong> " + prof->get_position() + "</p>";
    }
    else if (user->type() == "student")
    {
        shared_ptr<Student> student = dynamic_pointer_cast<Student>(user);
        body += "      <p><strong>term:</strong> " + to_string(student->get_semester()) + "</p>";
    }
    for (shared_ptr<UT_media> each_post : posts)
    {
        shared_ptr<Post> post = dynamic_pointer_cast<Post>(each_post);
        body += "      <h3>" + post->get_title() + "</h3>";
        body += "      <h4>" + post->get_message() + "</h4>";
        if (post->get_image() != "/.png")
        {
            body += "      <img src=\"" + post->get_image() + "\" alt=\"Profile Picture\" class=\"profile-pic\">";
        }
    }
    body += "    </section>";
    body += "    <section class=\"activities\">";
    body += "      <ul>";
    body += "        <li><a href=\"/homepage\" class=\"button\">Go to home</a></li>";
    body += "      </ul>";
    body += "  </main>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}

ShowAllCoursesHandler::ShowAllCoursesHandler(System *system)
    : system(system) {}

Response *ShowAllCoursesHandler::callback(Request *req)
{
    Response *res = new Response();
    if (req->getSessionId() != "student" && req->getSessionId() != "admin" && req->getSessionId() != "professor")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    res->setHeader("Content-Type", "text/html");
    std::string body;
    std::vector<std::shared_ptr<Course>> courses = system->get_courses();
    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";

    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    body += "  <title>All Courses - UTMS</title>";
    body += "  <link rel=\"stylesheet\" href=\"/style.css\">";
    body += "</head>";

    body += "<body>";
    body += "  <header>";
    body += "    <h1>All Courses</h1>";
    body += "  </header>";
    body += "  <main>";
    body += "    <section class=\"courses\">";
    body += "      <h2>Courses List</h2>";

    for (const auto &course : courses)
    {
        body += "      <div class=\"course\">";
        body += "        <h3>" + course->get_name() + "</h3>";
        body += "        <p><strong>Course id:</strong> " + std::to_string(course->get_id()) + "</p>";
        body += "        <p><strong>Professor:</strong> " + course->get_professor() + "</p>";
        body += "        <p><strong>Capacity:</strong> " + std::to_string(course->get_capacity()) + "</p>";
        body += "        <p><strong>Class Time:</strong> " + course->get_time() + "</p>";
        body += "        <p><strong>Exam Date:</strong> " + course->get_exam_date() + "</p>";
        body += "        <p><strong>Class Number:</strong> " + to_string(course->get_class_num()) + "</p>";
        body += "      </div>";
    }

    body += "    </section>";
    body += "  </main>";
    body += "  <footer>";
    body += "    <p>&copy; 2024 UTMS</p>";
    body += "  </footer>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}

TakeCourseHandler::TakeCourseHandler(System *system)
    : system(system) {}

Response *TakeCourseHandler::callback(Request *req)
{
    Response *res = new Response();
    if (req->getSessionId() != "student")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }

    std::string course_id = req->getBodyParam("course_id");
    try
    {

        system->take_course(course_id);
        res = Response::redirect("/homepage");
        return res;
    }
    catch (NotFoundError &e)
    {
        res = Response::redirect(NOT_FOUND_PAGE);
        return res;
    }
    catch (PermissionDeniedError &e)
    {
        res = Response::redirect(PERM_DENIED_PAGE);
        return res;
    }
    catch (BadRequestError &e)
    {
        res = Response::redirect(BAD_REQ_PAGE);
        return res;
    }
}





DeleteCourseHandler::DeleteCourseHandler(System *system)
    : system(system) {}

Response *DeleteCourseHandler::callback(Request *req)
{
    Response *res = new Response();
    if (req->getSessionId() != "student")
    {
        res = Response::redirect(PERM_DENIED_PAGE);
        return res;
    }

    std::string course_id = req->getBodyParam("course_id");
    try
    {

       system->delete_course(course_id);
        res = Response::redirect("/homepage");
        return res;
    }
    catch (NotFoundError &e)
    {
        res = Response::redirect(NOT_FOUND_PAGE);
        return res;
    }
    catch (PermissionDeniedError &e)
    {
        res = Response::redirect(PERM_DENIED_PAGE);
        return res;
    }
    catch (BadRequestError &e)
    {
        res = Response::redirect(BAD_REQ_PAGE);
        return res;
    }
}




ShowMyCoursesHandler::ShowMyCoursesHandler(System *system)
    : system(system) {}

Response *ShowMyCoursesHandler::callback(Request *req)
{
    Response *res = new Response();
    if (req->getSessionId() != "student")
    {
        res = Response::redirect("/Permission_error");
        return res;
    }
    res->setHeader("Content-Type", "text/html");
    std::string body;
    std::vector<std::shared_ptr<Course>> courses = system->current_user->get_courses();
    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";
    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    body += "  <title>My Courses - UTMS</title>";
    body += "  <link rel=\"stylesheet\" href=\"/style.css\">";
    body += "</head>";

    body += "<body>";
    body += "  <header>";
    body += "    <h1>My Courses</h1>";
    body += "  </header>";
    body += "  <main>";
    body += "    <section class=\"courses\">";
    body += "      <h2>Courses List</h2>";

    for (const auto &course : courses)
    {
        body += "      <div class=\"course\">";
        body += "        <h3>" + course->get_name() + "</h3>";
        body += "        <p><strong>Course id:</strong> " + std::to_string(course->get_id()) + "</p>";
        body += "        <p><strong>Professor:</strong> " + course->get_professor() + "</p>";
        body += "        <p><strong>Capacity:</strong> " + std::to_string(course->get_capacity()) + "</p>";
        body += "        <p><strong>Class Time:</strong> " + course->get_time() + "</p>";
        body += "        <p><strong>Exam Date:</strong> " + course->get_exam_date() + "</p>";
        body += "        <p><strong>Class Number:</strong> " + to_string(course->get_class_num()) + "</p>";
        body += "      </div>";
    }

    body += "    </section>";
    body += "  </main>";
    body += "  <footer>";
    body += "    <p>&copy; 2024 UTMS</p>";
    body += "  </footer>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}