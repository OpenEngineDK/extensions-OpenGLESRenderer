# Create the extension library
ADD_LIBRARY(Extensions_OpenGLESRenderer
  Display/OpenGLES2/RenderCanvas.h
  Display/OpenGLES2/RenderCanvas.cpp
  Renderers/OpenGLES2/RenderingView.cpp
  Renderers/OpenGLES2/Renderer.cpp
  Renderers/OpenGLES2/LightRenderer.cpp
  Resources/OpenGLES2Shader.cpp
)

TARGET_LINK_LIBRARIES(
  Extensions_OpenGLESRenderer
  "-framework OpenGLES"
)
