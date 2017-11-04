#include "Converter.h"

#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "R3.h"
#include "Orientation.h"

using namespace Ogre;

Vector3 Converter::toOgre(const R3 & v)
{
    return Vector3(v[1], v[2], v[0]);
}

Vector3 Converter::toOgre(const double x, const double y, const double z)
{
    return toOgre(R3(x, y, z));
}

Quaternion Converter::toOgre(const Orientation & o)
{
    return Quaternion(o[0], o[2], o[3], o[1]);
}

R3 Converter::toPhysics(const Ogre::Vector3 & v)
{
    return R3(v[2], v[0], v[1]);
}

Orientation Converter::toPhysics(const Ogre::Quaternion & q)
{
    return Orientation(q[0], q[3], q[1], q[2]);
}
