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
 *
 * This experiment abandons input system,
 * input is processed each time it is needed and not encapsulated
 */

int WINDOW_WIDTH = 980;
int WINDOW_HEIGHT = 540;
const char* WINDOW_TITLE = "Experimenting";
float FRAME_RATE = 60.0f;   //fps

GLuint gameMode = 1;    //meant to have more possible vals in future

float BALL_RED = 0.8;   //adjust the target ball color
float BALL_BLUE = 0.2;
float BALL_GREEN = 0.2;
///////////////////////////////////////////////////////////Above are adjustible globals//////////////////////////////////
//////////////////////////////////////////////////////////Below are globals not meant to be tempered!

/*
 * init functions must be called in this order before main(),
 * non-init functions must be called in game while loop inside main(),
 */
void initWindow();
void initOpenGL();
void initTimeKeeper();
////////main()///////////
void timeKeeper();
void timeRegulatedUpdateRender(World &world);
/////utilities//////////
void callTime(int gap); //call out the current time at interval of gap

GLFWwindow* window;
vec2 windowSize;

float currentFrame;
float deltaTime;
float prevFrame;
float renderAccumulator = 0.0;
const float frameDuration = 1.0f/FRAME_RATE;

int main() {
///    srand(time(0)); optional random seed.
    cout << "=======We are runing main=======\n";
///    cin >> gameMode; try default param
    cout << "\n";
        
    initWindow();
    initOpenGL();
    World world(window, windowSize, BALL_RED, BALL_GREEN, BALL_BLUE);    //this is the only object main() creates
    //tentatively only support basic basic basics for readability and debugging
    world.askBallManagerToSetGameMode(gameMode);
    initTimeKeeper();
    //the while loop game loop, quit via esc, q, click close
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwGetKey(window, GLFW_KEY_Q)){
        timeKeeper();//first thing we do is to keep updating time


        if (world.askBallManagerIfGameOver()){break;}  //somehow original code put this inside timeRegulated update and render
        timeRegulatedUpdateRender(world);   //this will in turn update and render all other components in the world
        //glfw code that is the same as sdl2
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    cout << "=======world ended======="
    << '\n'<<
    "You killed: "<<world.GetScore() << " Balls." << endl;
    return 0;
}
/**
 * uses main file global params:
 *      WINDOW_WIDTH
 *      WINDOW_HEIGHT
 *
 * function updates:
 *      window
 */
void initWindow() {
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

void initTimeKeeper(){
    currentFrame = glfwGetTime();
    prevFrame = currentFrame;
}

void timeKeeper(){
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
    renderAccumulator += deltaTime;
}


/**
 * This is update and render in one,
 *
 * note that the @param world
 * is such that it in turn updates and renders
 * everything in the world
 *
 */
void timeRegulatedUpdateRender(World &world){
    if (renderAccumulator >= frameDuration) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderAccumulator -= frameDuration;
        world.Update(deltaTime);    //this might be causing trouble
        world.Render();
    }
}