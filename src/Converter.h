#ifndef __Converter_h__
#define __Converter_h__

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include "R3.h"
#include "Orientation.h"

class Converter {
    public:
        static Ogre::Vector3 toOgre(const R3 &);
        static Ogre::Vector3 toOgre(const double, const double, const double);
        static Ogre::Quaternion toOgre(const Orientation &);

        static R3 toPhysics(const Ogre::Vector3 &);
        static Orientation toPhysics(const Ogre::Quaternion &);
};

#endif
