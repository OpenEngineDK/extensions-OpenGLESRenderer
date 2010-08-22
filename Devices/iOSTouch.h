// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OEI_O_S_TOUCH_H_
#define _OEI_O_S_TOUCH_H_

#include <Core/EngineEvents.h>
#include <Core/IListener.h>
#include <Core/Event.h>

namespace OpenEngine {
namespace Devices {

using namespace Core;
/**
 * Short description.
 *
 * @class iOSTouch iOSTouch.cpp ons/iOS/Devices/iOSTouch.cpp
 */
    
    
class TouchMovedEventArg {
public:
    int dx;
    int dy;
};
    
class iOSTouch {
private:
    Event<TouchMovedEventArg> touchMovedEvent;
public:
    iOSTouch();
    

    IEvent<TouchMovedEventArg>& TouchMovedEvent() {return touchMovedEvent; }
};

} // NS Devices
} // NS OpenEngine

#endif // _OEI_O_S_TOUCH_H_
