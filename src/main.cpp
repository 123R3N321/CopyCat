//#include <glad/glad.h>
#include "../library/include/glad/glad.h"

//#include <GLFW/glfw3.h>
#include "../library/include/GLFW/glfw3.h"



#include "world.h"



/**
 * Overall architecture of reworked engine:
 *
 *
 *                  place
 *                  player
 * main-->world-->  camera
 *                  ballManager
 */

int WINDOW_WIDTH = 980;
int WINDOW_HEIGHT = 540;
const char* WINDOW_TITLE = "Experimenting";

///////////////////////////////////////////////////////////Above are adjustible globals//////////////////////////////////
//////////////////////////////////////////////////////////Below are globals not meant to be tempered!
void initWindow();
void initOpenGL();
void timeKeeper();

GLFWwindow* window;
vec2 windowSize;

double currentFrame;
double deltaTime;
double prevFrame;
double renderAccum = 0.0;

int main() {
    //
    const double TARGET_FRAME = 0.016667;                   // 1/60, use this to make sure deltaTime goes to 1sec

    srand(time(0));

    GLuint gameMode = 1;
    cout << "------------We are runing main------------\n";
//    cin >> gameMode; try default param
    cout << "\n";
        
    initWindow();
    initOpenGL();
    cout<<"is Opengl prepped?"<<endl;
    World world(window, windowSize);
    cout<<"Seems like world init is problem"<<endl;

    currentFrame = glfwGetTime();
    prevFrame = currentFrame;

    //tentatively only support basic basic basics for readability and debugging
    world.askBallManagerToSetGameMode(gameMode);

    cout<<"right before loop?"<<endl;

    //the while loop game loop
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)
            && !glfwGetKey(window, GLFW_KEY_Q)) {
//        cout<<"am I in loop?"<<endl;
        //

        timeKeeper();

        if (renderAccum >= TARGET_FRAME) {
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderAccum -= TARGET_FRAME;

            world.Update(deltaTime);    //this might be causing trouble
            if (world.IsOver())
                break;
            world.Render();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    cout << "----------------------------world ended properly" << world.GetScore() << " ----------------------------" << endl;
    return 0;
}
/**
 * uses main file global params:
 *      WINDOW_WIDTH
 *      WINDOW_HEIGHT
 */
void initWindow() {



    //
    if (!glfwInit()) {
        cout << "Could not initialize GLFW" << endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glGetError();

    windowSize = vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.529f, 0.808f, 0.922f, 0.0f);
}

void timeKeeper(){
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
    renderAccum += deltaTime;
}