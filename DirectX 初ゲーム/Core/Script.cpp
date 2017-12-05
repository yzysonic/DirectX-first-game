#include "Script.h"
#include "Object.h"

Script::Script(ObjectBase& object) : object(object), transform(*object.getTransform())
{
}
