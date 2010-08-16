// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_RENDER_CANVAS_H_
#define _OE_RENDER_CANVAS_H_

#include <Display/IRenderCanvas.h>

namespace OpenEngine {
namespace Display {
namespace OpenGLES2 {

/**
 * Short description.
 *
 * @class RenderCanvas RenderCanvas.h ns/iOS/Display/OpenGLES2/RenderCanvas.h
 */
class RenderCanvas : public IRenderCanvas {
private:
    unsigned int width;
    unsigned int height;
public:

    RenderCanvas();

    void Handle(Display::InitializeEventArg arg);
    void Handle(Display::ProcessEventArg arg);
    void Handle(Display::ResizeEventArg arg);
    void Handle(Display::DeinitializeEventArg arg);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    void SetWidth(const unsigned int width);
    void SetHeight(const unsigned int height);
    ITexture2DPtr GetTexture();

};

} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_RENDER_CANVAS_H_
