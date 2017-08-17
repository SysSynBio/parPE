#include "parpeException.h"

ParPEException::ParPEException(const char *message) : message(message) {}

ParPEException::ParPEException(const std::string &message) : message(message) {}

const char *ParPEException::what() const noexcept { return message.c_str(); }