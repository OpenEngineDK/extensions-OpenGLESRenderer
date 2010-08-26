
# Create the extension library
ADD_LIBRARY(Extensions_iOS
  Core/iOSEngine.h
  Core/iOSEngine.mm
  Core/OEDelegate.h
  Core/OEDelegate.mm
  Display/OEViewController.h
  Display/OEViewController.m
  Display/OEView.h
  Display/OEView.mm
  Display/iOSEnvironment.h
  Display/iOSEnvironment.mm
  Display/iOSFrame.h
  Display/iOSFrame.mm
  Display/OpenGLES2/RenderCanvas.cpp
  Devices/iOSTouch.h
  Devices/iOSTouch.cpp
  Renderers/OpenGLES2/RenderingView.cpp
  Renderers/OpenGLES2/Renderer.cpp
  Renderers/OpenGLES2/LightRenderer.cpp
  Resources/OpenGLES2Shader.cpp
)
