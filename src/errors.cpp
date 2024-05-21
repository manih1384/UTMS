#include "errors.hpp"






const char* NotFoundError::what() const noexcept{
        return "Not found";
    }



const char* BadRequestError::what() const noexcept{
        return "Bad request";
    }

const char* PermissionDeniedError::what() const noexcept{
        return "Permission denied";
    }

const char* EmptyError::what() const noexcept{
        return "Empty";
    }