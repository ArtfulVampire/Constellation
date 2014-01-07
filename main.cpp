#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "maths_funcs.h"

using namespace std;

camera myCamera, tempCamera;

mat4 modelMatrix;// = identity_mat4();
mat4 viewMatrix;// = look_at(vec3(30.,80.,100), vec3(0.,0.,0), vec3(0., 1., 0.));
mat4 projMatrix;// = perspective(67.0 * ONE_DEG_IN_RAD, 4./3., 1.0, 300.0);
mat4 allMatrix;// = identity_mat4();

int prevX=0, prevY=0;
int X,Y;
bool flagMouse = 0;
mat4 tempModelMatrix;

int CurrentWidth;
int CurrentHeight;

const char* shaderFromFile(char* path);
void drawCube(GLFWwindow * window, float radius, mat4 allMatrix);
void drawPolyhedron(GLFWwindow * window, vec3 center, float radius, vec3 color, mat4 allMatrix, const char* vertexShad, const char* fragmentShad);
int ReadCoordinates(float *** coords, const char* sourcePath);
void scrollHandler(GLFWwindow *, double X, double Y);
void mouseHandler(GLFWwindow *, int button, int press_release, int mods);
void cursorPosHandler(GLFWwindow *, double currX, double currY);
void keyboardHandler(GLFWwindow *, int button, int action, int mods);
void resizeHandler(GLFWwindow *, int width, int height);


int main (int argc, char* argv[])
{
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit ())
  {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  CurrentWidth = 800;
  CurrentHeight = 600;

  GLFWwindow* window = glfwCreateWindow(CurrentWidth, CurrentHeight, "Constellation", NULL, NULL);
  if (!window)
  {
      fprintf (stderr, "ERROR: could not open window with GLFW3\n");
      glfwTerminate();
      return -1;
  }

//  if(argc != 2)
//  {
//      fprintf (stdout, "usage: a.out <vertexShaderFile> <fragmentShaderFile>\n");
//      glfwTerminate();
//      return -1;
//  }

  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

  glfwSetScrollCallback(window, (GLFWscrollfun)scrollHandler);
  glfwSetCursorPosCallback(window, (GLFWcursorposfun)cursorPosHandler);
  glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)mouseHandler);
  glfwSetKeyCallback(window, (GLFWkeyfun)keyboardHandler);
  glfwSetWindowSizeCallback(window, (GLFWwindowsizefun)resizeHandler);

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"


//  glEnable (GL_CULL_FACE); // cull face
//  glCullFace (GL_BACK); // cull back face
//  glFrontFace (GL_CW); // GL_CCW for counter clock-wise


  const char* constVertexShaderString = shaderFromFile("/home/michael/Qt/Projects/myCppOpenGL2/polyhedronVertexShader.glsl");
  const char* constFragmentShaderString = shaderFromFile("/home/michael/Qt/Projects/myCppOpenGL2/polyhedronFragmentShader.glsl");
//  const char* constVertexShaderString = shaderFromFile(argv[0]);
//  const char* constFragmentShaderString = shaderFromFile(argv[1]);

  modelMatrix = identity_mat4();
  viewMatrix = look_at(vec3(0.,0.,100), vec3(0.,0.,0), vec3(0., 1., 0.));
  projMatrix = perspective(67.0 * ONE_DEG_IN_RAD, 4./3., 1.0, 300.0);
  allMatrix = projMatrix * viewMatrix *  modelMatrix;


  int numOfBalls = 0;
  float **coords;
//  numOfBalls = ReadCoordinates(&coords, "/home/michael/Qt/Projects/myCppOpenGL2/balls.txt");

  myCamera = camera(vec3(0.,0.,100), vec3(0.,0.,0), vec3(0., 1., 0.));

  time_t timeElapsed = time(NULL);
  time_t timeCurrent;
  while (!glfwWindowShouldClose (window))
  {
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      allMatrix = projMatrix * myCamera.camMatrix *  modelMatrix;



      drawCube(window, 1.0, allMatrix);
      for(int i = 0; i < numOfBalls; ++i)
      {
          drawPolyhedron(window, vec3(coords[i]), 0.2, vec3(0.1 + i*0.2, 0.0, 0.9 - i*0.2), allMatrix, constVertexShaderString, constFragmentShaderString);

      }

      glfwPollEvents ();

      glfwSwapBuffers (window);
      timeCurrent = time(NULL);
      if(timeCurrent-timeElapsed > 30)
      {
          glfwSetWindowShouldClose (window, 1);
      }
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}


void keyboardHandler(GLFWwindow * window_, int button, int action, int mods)
{
    float angle = 0.05;
    float angle2 = 0.005;
    float dist = 5.00;
    float dist2 = 0.5;
    if(action == GLFW_RELEASE) return;
    switch(button)
    {
    case GLFW_KEY_ESCAPE:
    {
        glfwSetWindowShouldClose (window_, 1);
    }
    case GLFW_KEY_A:
    {
        myCamera.rotateAboutTargetRL(-angle);
        break;
    }
    case GLFW_KEY_D:
    {
        myCamera.rotateAboutTargetRL(angle);
        break;
    }
    case GLFW_KEY_W:
    {
        myCamera.rotateAboutTargetUD(angle);
        break;
    }
    case GLFW_KEY_S:
    {
        myCamera.rotateAboutTargetUD(-angle);
        break;
    }
    case GLFW_KEY_Q:
    {
        myCamera.rotateAboutDirection(-angle);
        break;
    }
    case GLFW_KEY_E:
    {
        myCamera.rotateAboutDirection(angle);
        break;
    }
    case GLFW_KEY_LEFT:
    {
        myCamera.rotateFreeRL(angle2);
        break;
    }
    case GLFW_KEY_RIGHT:
    {
        myCamera.rotateFreeRL(-angle2);
        break;
    }
    case GLFW_KEY_UP:
    {
        myCamera.rotateFreeUD(angle2);
        break;
    }
    case GLFW_KEY_DOWN:
    {
        myCamera.rotateFreeUD(-angle2);
        break;
    }
    case GLFW_KEY_J:
    {
        myCamera.moveRL(dist2);
        break;
    }
    case GLFW_KEY_L:
    {
        myCamera.moveRL(-dist2);
        break;
    }
    case GLFW_KEY_I:
    {
        myCamera.moveUD(dist2);
        break;
    }
    case GLFW_KEY_K:
    {
        myCamera.moveUD(-dist2);
        break;
    }
    case GLFW_KEY_Z:
    {
        myCamera.zoom(dist);
        break;
    }
    case GLFW_KEY_X:
    {
        myCamera.zoom(-dist);
        break;
    }
    case GLFW_KEY_P:
    {
        print(myCamera);
    }
    case GLFW_KEY_END:
    {
        myCamera.setTarget(vec3(0., 0., 0.));
        break;
    }
    default:
    {
        break;
    }
    }

}


void drawCube(GLFWwindow * window, float radius, mat4 allMatrix)
{

    //cube, clockwise
    vec3 vertices[] =
    {
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},

        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, 0.5f}
    };

    vec3 colors[]=
    {
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},

        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f}
    };



    GLubyte indicies[] =
    {

        0, 3, 1,
        3, 2, 1,

        0,4,3,
        3,4,7,

        4,5,7,
        7,5,6,

        0,1,4,
        4,1,5,

        1,2,5,
        5,2,6,

        3,7,2,
        2,7,6
    };
    for(int i = 0; i < 8; ++i)
    {
        vertices[i] *= radius/2.;
    }


    unsigned int vao;
    unsigned int vbo_vertices;
    unsigned int vbo_colors;
    unsigned int IndexBufferId;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers (1, &vbo_vertices);
    glBindBuffer (GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices) * sizeof(vertices[0].v), &vertices[0].v, GL_STATIC_DRAW);

    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers (1, &vbo_colors);
    glBindBuffer (GL_ARRAY_BUFFER, vbo_colors);
    glBufferData (GL_ARRAY_BUFFER, sizeof (colors) * sizeof(colors[0].v), &colors[0].v, GL_STATIC_DRAW);

    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers (1, &IndexBufferId);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


    const char* constVertexShaderString = shaderFromFile("/home/michael/Qt/Projects/myCppOpenGL2/vertex_shader.glsl");
    const char* constFragmentShaderString = shaderFromFile("/home/michael/Qt/Projects/myCppOpenGL2/fragment_shader.glsl");

    unsigned int vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &constVertexShaderString, NULL);
    glCompileShader (vs);

    unsigned int fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &constFragmentShaderString, NULL);
    glCompileShader (fs);


    unsigned int shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);

    glLinkProgram (shader_programme);
  //  glEnable (GL_CULL_FACE); // cull face
  //  glCullFace (GL_BACK); // cull back face
  //  glFrontFace (GL_CW); // GL_CCW for counter clock-wise
    int allMatrixLocation = glGetUniformLocation (shader_programme, "allMat");
    int colorLocation = glGetUniformLocation (shader_programme, "vertexColor");
    glUseProgram (shader_programme);

//    glUniform4fv (colorLocation, 1, vec4(0.5, 0.5, 0.5, 1.0).v);
    glUniformMatrix4fv (allMatrixLocation, 1, GL_FALSE, allMatrix.m);
    glUseProgram (shader_programme);
    glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_BYTE, (GLvoid*)0);
    glUseProgram (shader_programme);


}


void drawPolyhedron(GLFWwindow * window, vec3 center, float radius, vec3 color, mat4 allMatrix, const char* vetrexShad, const char* fragmentShad)
{
    //cube, clockwise
    vec3 vertices[] =
    {
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},

        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, 0.5f}
    };



    GLubyte indicies[] =
    {

        0, 3, 1,
        3, 2, 1,

        0,4,3,
        3,4,7,

        4,5,7,
        7,5,6,

        0,1,4,
        4,1,5,

        1,2,5,
        5,2,6,

        3,7,2,
        2,7,6
    };
    for(int i = 0; i < 8; ++i)
    {
        vertices[i] *= radius/2.;
        vertices[i] += center;
    }


    unsigned int vao;
    unsigned int vbo_vertices;
    unsigned int IndexBufferId;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers (1, &vbo_vertices);
    glBindBuffer (GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices) * sizeof(vertices[0].v), &vertices[0].v, GL_STATIC_DRAW);

    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers (1, &IndexBufferId);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


//    const char* constVertexShaderString = shaderFromFile("polyhedronVertexShader.glsl");
//    const char* constFragmentShaderString = shaderFromFile("polyhedronFragmentShader.glsl");

    unsigned int vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vetrexShad, NULL);
    glCompileShader (vs);

    unsigned int fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragmentShad, NULL);
    glCompileShader (fs);


    unsigned int shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);

    glLinkProgram (shader_programme);
  //  glEnable (GL_CULL_FACE); // cull face
  //  glCullFace (GL_BACK); // cull back face
  //  glFrontFace (GL_CW); // GL_CCW for counter clock-wise
    int allMatrixLocation = glGetUniformLocation (shader_programme, "allMat");
    int colorLocation = glGetUniformLocation (shader_programme, "vertexColor");
    glUseProgram (shader_programme);

    glUniformMatrix4fv (allMatrixLocation, 1, GL_FALSE, allMatrix.m);
    glUniform4fv (colorLocation, 1, vec4(color, 1.0).v);


    glUseProgram (shader_programme);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_BYTE, (GLvoid*)0);


    glUseProgram (shader_programme);

}


int ReadCoordinates(float *** coords, const char* sourcePath)
{
    float helpFloat;
    mat4 centerCoords;


    int numberOfBalls = -1;
    FILE * src = fopen(sourcePath, "r");
    if(src == NULL)
    {
        fprintf(stderr, "Cannot open source file");
        exit(EXIT_FAILURE);
    }

    fscanf(src, "%d", &numberOfBalls);
    (*coords) = new float * [numberOfBalls];
    vec4 vecCoords[numberOfBalls];
    for(int i = 0 ; i < numberOfBalls; ++i)
    {
        (*coords)[i] = new float [4];
    }
    for(int i = 0 ; i < numberOfBalls; ++i)
    {
        for(int j = 0 ; j < 3; ++j)
        {
            fscanf(src, "%f", &((*coords)[i][j]));
        }
        (*coords)[i][3] = 1.;
        vecCoords[i] = vec4((*coords)[i]);
    }

    //translation
    centerCoords = identity_mat4();
    for(int i = 0 ; i < 3; ++i)
    {
        helpFloat = 0.;
        for(int j = 0 ; j < numberOfBalls; ++j)
        {
            helpFloat += (*coords)[j][i];
        }
        helpFloat /= numberOfBalls;
        centerCoords.m[12+i] = -helpFloat;
    }
    for(int i = 0; i < numberOfBalls; ++i)
    {
        vecCoords[i] = centerCoords*vecCoords[i];
    }
    helpFloat = 0.;
    for(int i = 0 ; i < numberOfBalls; ++i)
    {
        helpFloat = fmax(helpFloat, length(vec3(vecCoords[i])));
    }
    for(int i = 0 ; i < numberOfBalls; ++i)
    {
        vecCoords[i] /= helpFloat;
    }


    for(int i = 0 ; i < numberOfBalls; ++i)
    {
        for(int j = 0 ; j < 4; ++j)
        {
            (*coords)[i][j] = vecCoords[i][j];
        }
    }
    fclose(src);
    return numberOfBalls;
}

const char* shaderFromFile(char* path)
{
    char* tempString = new char [50];
    char* shaderString = new char [300];
    int currentIndex = 0;
    FILE * shad = fopen(path, "r");
    if(shad == NULL)
    {
        cout<<"Cannot open file\n"<<endl;
        return (const char*)NULL;
    }
    while(1)
    {

        fgets(tempString, 50, shad);
        if(feof(shad)) break;
        for(int i = 0; i < strlen(tempString); ++i)
        {
            shaderString[currentIndex++] = tempString[i];
        }
    }
    shaderString[currentIndex] = '\0';
    fclose(shad);

    delete []tempString;
    return shaderString;
}


void scrollHandler(GLFWwindow *, double X, double Y)
{
    myCamera.zoom(Y);
}

void mouseHandler(GLFWwindow *, int button, int press_release, int mods)
{
    if(press_release == 1)
    {
        prevX = X;
        prevY = Y;
        tempCamera = myCamera;
        tempModelMatrix = modelMatrix;
        flagMouse = 1;
        if(button == 4 || button == 5)
        {
            myCamera = camera(vec3(0,0,100), vec3(0,0,0), vec3(0,1,0));
            flagMouse = 0;
            modelMatrix = identity_mat4();
        }
    }
    if(press_release == 0)
    {
        flagMouse = 0;
    }
}

void cursorPosHandler(GLFWwindow *, double currX, double currY)
{
    X = currX;
    Y = currY;

    if(prevX==X && prevY==Y) return;
    if(!flagMouse) return;

    myCamera = tempCamera;
    modelMatrix = tempModelMatrix;

    vec3 vect1;
    vec3 vect2;
    vec3 vect3;
    float ang, ang0, ang1;


    vect3.v[0] = X - prevX;
    vect3.v[1] = Y - prevY;
    vect3.v[2] = 0.;
//    printf("%.2lf %.2lf\n", vect3.v[0], vect3.v[1]);


    vect2.v[0] = (prevX + X)/2. - CurrentWidth/2.;
    vect2.v[1] = (prevY + Y)/2. - CurrentHeight/2.;
//    printf("%.2lf %.2lf\n", vect2.v[0], vect2.v[1]);

//    vect1.v[2] = sqrt(vect2.v[0]*vect2.v[0] + vect2.v[1]*vect2.v[1]) * sin(acos((vect3.v[0]*vect2.v[0] + vect3.v[1]*vect2.v[1])/(sqrt(vect2.v[0]*vect2.v[0] + vect2.v[1]*vect2.v[1]) * sqrt(vect3.v[0]*vect3.v[0] + vect3.v[1]*vect3.v[1]))));



    vect1.v[0] = -vect3.v[1];
    vect1.v[1] =  vect3.v[0];
    vect1.v[2] = 0.;
    vect1.v[2] = sqrt(vect2.v[0]*vect2.v[0] + vect2.v[1]*vect2.v[1]) * sin(acos((vect1.v[1]*vect2.v[0] - vect1.v[0]*vect2.v[1])/(sqrt(vect2.v[0]*vect2.v[0] + vect2.v[1]*vect2.v[1]) * sqrt(vect1.v[0]*vect1.v[0] + vect1.v[1]*vect1.v[1]))));
    vect1.v[2] *= 2.;
    if(dot(vect1, vect2) < 0.) vect1.v[2] *=-1;

    vect1 = normalise(vect1); //-1 for right-hand rule rotation
    ang = (vect3.v[0]* vect3.v[0] + vect3.v[1]* vect3.v[1]) / (CurrentHeight*CurrentHeight + CurrentWidth*CurrentWidth)*3.; // norm coeff?
    myCamera.rotateAboutAxis(vect1, ang);

}

void resizeHandler(GLFWwindow * window, int width, int height)
{
    CurrentHeight = height;
    CurrentWidth = width;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
    projMatrix = perspective(67.0 * ONE_DEG_IN_RAD, float(width)/height, 1.0f, 300.0f);
}
