// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OEI_O_S_ENVIRONMENT_H_
#define _OEI_O_S_ENVIRONMENT_H_

#include <Display/IEnvironment.h>
#include <Display/iOSFrame.h>
#include <Devices/iOSTouch.h>

namespace OpenEngine {
namespace Display {

using namespace Core;

/**
 * Short description.
 *
 * @class iOSEnvironment iOSEnvironment.h ons/iOS/Display/iOSEnvironment.h
 */
class iOSEnvironment : public IEnvironment {
private:
    iOSFrame* frame;
    Devices::iOSTouch* touch;
public:
    iOSEnvironment();
    IFrame& CreateFrame();
    Devices::IMouse* GetMouse();
    Devices::IKeyboard* GetKeyboard();
    Devices::IJoystick* GetJoystick();
    Devices::iOSTouch* GetTouch();
    
    void Handle(Core::ProcessEventArg);
    void Handle(Core::InitializeEventArg);
    void Handle(Core::DeinitializeEventArg);

    string GetResourcePath();
};

} // NS Display
} // NS OpenEngine

#endif // _OEI_O_S_ENVIRONMENT_H_
