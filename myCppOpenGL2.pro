TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    maths_funcs.cpp

#LIBS += -L/usr/lib/x86_64-linux-gnu -lGL
#LIBS += -L/usr/lib/x86_64-linux-gnu -lGLU
#LIBS += -L/usr/lib/x86_64-linux-gnu -lglut
LIBS += -L/usr/lib64 -lGLEW
#LIBS += -L/usr/local/lib -lglfw3

LIBS += -lGL -lGLU -lglut -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

OTHER_FILES += \
    vertex_shader.glsl \
    fragment_shader.glsl \
    polyhedronVertexShader.glsl \
    polyhedronFragmentShader.glsl

HEADERS += \
    maths_funcs.h
