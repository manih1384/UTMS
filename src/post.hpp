#pragma once
#ifndef POST_HPP
#define POST_HPP

#include <string>

class Post {
public:
    Post(int id,  std::string title,  std::string message);

    int get_id() const;
    std::string get_title() const;
    std::string get_message() const;

private:
    int id;
    std::string title;
    std::string message;
};

#endif // POST_HPP
