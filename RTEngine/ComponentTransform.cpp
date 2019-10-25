#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
}

ComponentTransform::ComponentTransform(GameObject * parent) : Component(TRANSFORM,parent)
{

}


ComponentTransform::~ComponentTransform()
{
}
