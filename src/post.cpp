#include "post.hpp"

Post::Post(int id, std::string title,  std::string message)
    : id(id), title(title), message(message) {}

int Post::get_id() const {
    return id;
}

std::string Post::get_title() const {
    return title;
}

std::string Post::get_message() const {
    return message;
}

Post::Post(int id, std::string title,  std::string message,std::string image)
    : id(id), title(title), message(message),image(image) {}
