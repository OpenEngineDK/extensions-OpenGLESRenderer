// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OEI_O_S_ENGINE_H_
#define _OEI_O_S_ENGINE_H_

#include <Core/TickEngine.h>
#include <Meta/iOS.h>


namespace OpenEngine {
namespace Core {
/**
 * Short description.
 *
 * @class iOSEngine iOSEngine.h ons/iOS/Core/iOSEngine.h
 */
class iOSEngine : public TickEngine {
private:
    id pool;
public:
    iOSEngine();
    virtual void Start();
    void DoStart();
};

extern iOSEngine* _iOSEngine;

} // NS Core
} // NS OpenEngine

#endif // _OEI_O_S_ENGINE_H_
