#pragma once
#include <exception>
#include <iostream>
#include <string>
using namespace std;

class NotFoundError : public exception {
public:
    const char*what() const noexcept override;
};

class EmptyError : public exception {
public:
    const char*what() const noexcept override;
};

class PermissionDeniedError : public exception {
public:
    const char*what() const noexcept override;
};

class BadRequestError : public exception {
public:
    const char*what() const noexcept override;
};