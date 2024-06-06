#include "errors.hpp"
#include <iostream>





const char*NotFoundError::what() const noexcept{
        return "Not Found";
    }



const char*BadRequestError::what() const noexcept{
        return "Bad Request";
    }

const char*PermissionDeniedError::what() const noexcept{
        return "Permission Denied";
    }

const char*EmptyError::what() const noexcept{
        return "Empty";
    }