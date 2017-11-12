#include "Script.h"
#include "Object.h"

Script::Script(Object& object) : object(object), transform(*object.getTransform())
{
}
