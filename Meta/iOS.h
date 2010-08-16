#include <Core/Exceptions.h>
#include <Utils/Convert.h>
#define THROW() do { throw Core::Exception("Not implemented: "            \
                                           + std::string(__PRETTY_FUNCTION__) \
                                           + " in " + std::string(__FILE__) + ":" + Utils::Convert::ToString(__LINE__) \
); } while (0)


#ifndef __OBJC__
typedef void* id;
#endif
