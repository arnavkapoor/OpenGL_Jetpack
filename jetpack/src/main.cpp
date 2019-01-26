#include "main.h"
#include "timer.h"
#include "player.h"
#define debug(x) cerr << #x << " is " << x << endl;

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Floor base;
Magnets magnet;
vector<Coins> mycoins;
vector<Beams> mybeams;
vector<Balloons> myballoons;
vector<Balloons> dragonball;
vector<Boomerang> myboomerangs;
vector<pair<Beams, Beams> > mypairbeams;
vector<Score> mydigs;
vector<Bonus> mybonus;
vector<Semicircle> mysemi;
Dragon smaug;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
int f = 0;
Timer t60(1.0 / 60);
Timer t15(1.0 / 15);
Timer t1(1.0 / 4);
Timer tdragon(1.0 / 2);

Timer magapp(30.0 / 1);
Timer magdisapp(4.96 / 1);
int score = 0;
int lives = 10;
bool magpres = 0;
int inside = 0;
int level = 1;
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    char buffer[50];
    sprintf(buffer, "lives: %d", lives);
    if (smaug.lives < 0)
        sprintf(buffer, "YOU HAVE WON !!!!!!!!!!!");
    glfwSetWindowTitle(window, buffer);
    if (lives == 0)
        quit(window);
    // clear the color and depth in the frame buffer
    reset_screen();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(0, 1.5 * 150, 25);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target(0, 1.5 * 150, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up(0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP; // MVP = Projection * View * Model
    for (int i = 0; i < mysemi.size(); i++)
        mysemi[i].draw(VP);

    if (level == 2 and smaug.lives >= 0)
        smaug.draw(VP);

    // Scene render
    ball1.draw(VP);
    base.draw(VP);
    magnet.draw(VP);

    for (int i = 0; i < mycoins.size(); i++)
        mycoins[i].draw(VP);
    for (int i = 0; i < mybeams.size(); i++)
        mybeams[i].draw(VP);
    for (int i = 0; i < myballoons.size(); i++)
        myballoons[i].draw(VP);
    for (int i = 0; i < dragonball.size(); i++)
        dragonball[i].draw(VP);

    for (int i = 0; i < myboomerangs.size(); i++)
        myboomerangs[i].draw(VP);
    for (int i = 0; i < mybonus.size(); i++)
        mybonus[i].draw(VP);

    for (int i = 0; i < myboomerangs.size(); i++)
    {
        mypairbeams[i].first.draw(VP);
        mypairbeams[i].second.draw(VP);
    }

    for (int i = 0; i < mydigs.size(); i++)
        mydigs[i].draw(VP);
}

void tick_input(GLFWwindow *window)
{
    if (!inside)
    {
        int left = glfwGetKey(window, GLFW_KEY_LEFT);
        int right = glfwGetKey(window, GLFW_KEY_RIGHT);
        int up = glfwGetKey(window, GLFW_KEY_UP);
        int water = glfwGetKey(window, GLFW_KEY_SPACE);

        if (left)
        {
            ball1.tick(DIR_LEFT);
        }
        if (right)
        {
            ball1.tick(DIR_RIGHT);
        }
        if (up)
        {
            ball1.tick(DIR_UP);
        }
        if (!(up or left or right))
        {
            ball1.tick(DIR_DOWN);
        }
        if (water and t1.processTick())
        {
            Balloons balloons1 = Balloons(ball1.position.x + 50.0f, ball1.position.y + 50.0f, ball1.speedx, ball1.speedy, 20.0f, COLOR_BLUE);
            myballoons.push_back(balloons1);
        }
    }
}

void render_score(int hundreds, int tens, int ones)
{
    mydigs.clear();
    vector<int> vals;
    vals.push_back(hundreds);
    vals.push_back(tens);
    vals.push_back(ones);
    vector<int> valid[10];

    int arr1[] = {0, 1, 2, 3, 4, 7, 8, 9};
    int arr2[] = {0, 2, 3, 5, 6, 7, 8, 9};
    int arr3[] = {0, 4, 5, 6, 8, 9};
    int arr4[] = {0, 2, 6, 8};
    int arr5[] = {0, 2, 3, 5, 6, 8, 9};
    int arr6[] = {0, 1, 3, 4, 5, 6, 7, 8, 9};
    int arr7[] = {2, 3, 4, 5, 6, 8, 9};

    valid[1].insert(valid[1].end(), arr1, arr1 + (sizeof(arr1) / sizeof(arr1[0])));
    valid[2].insert(valid[2].end(), arr2, arr2 + (sizeof(arr2) / sizeof(arr2[0])));
    valid[3].insert(valid[3].end(), arr3, arr3 + (sizeof(arr3) / sizeof(arr3[0])));
    valid[4].insert(valid[4].end(), arr4, arr4 + (sizeof(arr4) / sizeof(arr4[0])));
    valid[5].insert(valid[5].end(), arr5, arr5 + (sizeof(arr5) / sizeof(arr5[0])));
    valid[6].insert(valid[6].end(), arr6, arr6 + (sizeof(arr6) / sizeof(arr6[0])));
    valid[7].insert(valid[7].end(), arr7, arr7 + (sizeof(arr7) / sizeof(arr7[0])));

    for (int i = 0; i < vals.size(); i++)
    {
        int xoffset = 50 * i;
        int yoffset = -150;
        int myvalue = vals[i];
        for (int j = 1; j <= 7; j++)
        {
            for (int k = 0; k < valid[j].size(); k++)
            {
                if (valid[j][k] == myvalue and j == 1)
                    mydigs.push_back(Score(xoffset + 10, yoffset + 10, 90));

                if (valid[j][k] == myvalue and j == 2)
                    mydigs.push_back(Score(xoffset, yoffset + 20, 0));

                if (valid[j][k] == myvalue and j == 3)
                    mydigs.push_back(Score(xoffset - 10, yoffset + 10, 90));

                if (valid[j][k] == myvalue and j == 4)
                    mydigs.push_back(Score(xoffset - 10, yoffset - 10, 90));

                if (valid[j][k] == myvalue and j == 5)
                    mydigs.push_back(Score(xoffset, yoffset - 20, 0));

                if (valid[j][k] == myvalue and j == 6)
                    mydigs.push_back(Score(xoffset + 10, yoffset - 10, 90));

                if (valid[j][k] == myvalue and j == 7)
                    mydigs.push_back(Score(xoffset, yoffset, 0));
            }
        }
    }
}

void tick_elements()
{
    if (level == 2 and tdragon.processTick() and smaug.lives >= 0)
    {
        float srcx = smaug.position.x - 45;
        float srcy = smaug.position.y - 45;
        float tarx = ball1.position.x;
        float tary = ball1.position.y;
        Balloons enemy1 = Balloons(srcx, srcy, -12 + (tarx - srcx) * 0.05, -20 + (tary + 100 - srcy) * (0.05), 10, COLOR_BLOOD);
        dragonball.push_back(enemy1);
    }

    if (score < 0)
        score = 0;
    if (score > 20 and level == 1)
    {
        mybeams.clear();
        myballoons.clear();
        mybonus.clear();
        myboomerangs.clear();
        mypairbeams.clear();
        mysemi.clear();
        mycoins.clear();
        level = 2;
        smaug.position.x = 500;
        smaug.position.y = 500;
        smaug.box.x = 500;
        smaug.box.y = 500;
    }
    int hundreds = (score / 100) % 10;
    int tens = (score / 10) % 10;
    int ones = (score / 1) % 10;

    render_score(hundreds, tens, ones);
    if (magapp.processTick() and level == 1)
    {
        magpres = 1;
        magnet.position.x = -350 + rand() % 700;
        magnet.position.y = 0 + rand() % 700;
    }
    if (magdisapp.processTick())
    {
        magpres = 0;
        magnet.position.x = 900;
        magnet.position.y = 900;
    }
    if (magpres)
    {
        if (magnet.position.x - ball1.position.x > 0)
            ball1.position.x += 5;

        if (magnet.position.x - ball1.position.x < 0)
            ball1.position.x -= 5;

        if (magnet.position.y - ball1.position.y > 0)
        {
            ball1.speedy += 0.3;
            ball1.position.y += 5;
        }

        if (magnet.position.y - ball1.position.y < 0)
        {
            ball1.speedy -= 0.1;
            ball1.position.y -= 5;
        }
    }

    for (int i = 0; i < mycoins.size(); i++)
        mycoins[i].tick();

    for (int i = 0; i < mybeams.size(); i++)
        mybeams[i].tick(SINGLE);

    for (int i = 0; i < myballoons.size(); i++)
        myballoons[i].tick();
    for (int i = 0; i < dragonball.size(); i++)
        dragonball[i].tick();

    for (int i = 0; i < myboomerangs.size(); i++)
        myboomerangs[i].tick();

    for (int i = 0; i < mybonus.size(); i++)
        mybonus[i].tick();

    for (int i = 0; i < mysemi.size(); i++)
        mysemi[i].tick();

    for (int i = 0; i < mypairbeams.size(); i++)
    {
        mypairbeams[i].first.tick(DOUBLE_TOP);
        mypairbeams[i].second.tick(DOUBLE_BOTTOM);
    }
    
    bounding_box_t boxplayer = ball1.box;

    if (level == 2)
    {
        bounding_box_t boxdragon = smaug.box;
        for (int i = 0; i < myballoons.size(); i++)
        {
            bounding_box_t boxballoon = myballoons[i].box;
            if (detect_collision(boxdragon, boxballoon))
            {
                smaug.lives--;
                myballoons.erase(myballoons.begin() + i);
            }
        }
    }
    inside = 0;
    for (int i = 0; i < mysemi.size(); i++)
    {
        bounding_box_t boxsemi = mysemi[i].box;
        if (detect_collision(boxplayer, boxsemi))
        {
            ball1.speedy = 0;
            ball1.speedx = 0;

            float x = ball1.position.x;
            float h = boxsemi.x;
            float r = mysemi[i].radius;
            float k = boxsemi.y - (r / 2);
            float y = (pow(double(r * r - (x - h) * (x - h)), 0.500)) + k;
            if (isnan(y))
                y = ball1.position.y;
            debug(y);
            ball1.position.y = y;
            inside = 1;
        }
    }
    for (int i = 0; i < mycoins.size(); i++)
    {
        bounding_box_t boxcoin = mycoins[i].box;
        if (detect_collision(boxplayer, boxcoin))
        {
            score += mycoins[i].score;
            debug(score);
            mycoins.erase(mycoins.begin() + i);
        }
    }

    for (int i = 0; i < mybeams.size(); i++)
    {
        bounding_box_t boxbeam = mybeams[i].box;
        if (detect_collision(boxplayer, boxbeam))
        {
            if (!inside)
            {
                score -= 1;
                lives -= 1;
            }
            mybeams.erase(mybeams.begin() + i);
        }
    }

    for (int i = 0; i < dragonball.size(); i++)
    {
        bounding_box_t boxbeam = dragonball[i].box;
        if (detect_collision(boxplayer, boxbeam))
        {
            lives -= 1;
            dragonball.erase(dragonball.begin() + i);
        }
    }

    for (int i = 0; i < mypairbeams.size(); i++)
    {
        bounding_box_t boxbeam1 = mypairbeams[i].first.box;
        bounding_box_t boxbeam2 = mypairbeams[i].second.box;

        if (detect_collision(boxplayer, boxbeam1) or detect_collision(boxplayer, boxbeam2))
        {
            if (!inside)
            {
                score -= 2;
                lives -= 1;
            }
            mypairbeams.erase(mypairbeams.begin() + i);
        }
    }

    for (int i = 0; i < myboomerangs.size(); i++)
    {
        bounding_box_t boxboomerang = myboomerangs[i].box;
        if (detect_collision(boxplayer, boxboomerang))
        {
            if (!inside)
            {
                score -= 1;
                lives -= 1;
            }
            myboomerangs.erase(myboomerangs.begin() + i);
        }
        if (myboomerangs[i].position.y < 0)
            myboomerangs.erase(myboomerangs.begin() + i);
    }

    for (int i = 0; i < mybonus.size(); i++)
    {
        bounding_box_t boxboomerang = mybonus[i].box;
        if (detect_collision(boxplayer, boxboomerang))
        {
            score += mybonus[i].score;
            lives += mybonus[i].score;
            mybonus.erase(mybonus.begin() + i);
        }
        if (mybonus[i].position.y < 0)
            mybonus.erase(mybonus.begin() + i);
    }

    for (int i = 0; i < mybeams.size(); i++)
    {
        for (int j = 0; j < myballoons.size(); j++)
        {
            bounding_box_t boxbeam = mybeams[i].box;
            bounding_box_t boxballoon = myballoons[j].box;

            if (detect_collision(boxballoon, boxbeam))
            {
                score += 1;
                mybeams.erase(mybeams.begin() + i);
                myballoons.erase(myballoons.begin() + j);
            }

            if (myballoons[j].position.y <= -45.0f)
                myballoons.erase(myballoons.begin() + j);
        }
    }

    for (int i = 0; i < dragonball.size(); i++)
    {
        for (int j = 0; j < myballoons.size(); j++)
        {
            bounding_box_t boxdragon = dragonball[i].box;
            bounding_box_t boxballoon = myballoons[j].box;

            if (detect_collision(boxdragon, boxballoon))
            {
                dragonball.erase(dragonball.begin() + i);
                myballoons.erase(myballoons.begin() + j);
            }

            if (myballoons[j].position.y <= -45.0f)
                myballoons.erase(myballoons.begin() + j);
        }
        if (dragonball[i].position.y <= -45.0f)
            dragonball.erase(dragonball.begin() + i);
    }

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    smaug = Dragon(5700, 400, COLOR_GOLD);
    for (int i = 0; i < 1000; i += 2)
    {
        Coins coin1;
        if (rand() % 3 >= 1)
            coin1 = Coins(500 * i + rand() % 25, rand() % 600 + 100, 30, COLOR_NEONGREEN, 1);
        else
            coin1 = Coins(500 * i + rand() % 25, rand() % 600 + 100, 30, COLOR_SILVER, 2);

        mycoins.push_back(coin1);
    }

    for (int i = 0; i < 1000; i += 2)
    {
        Beams beam1;
        beam1 = Beams(500 * i + rand() % 25, rand() % 600 + 200, rand() % 180, COLOR_BEAM);
        mybeams.push_back(beam1);
    }

    for (int i = 2; i < 1000; i += 4)
    {
        Beams beam1;
        Beams beam2;

        beam1 = Beams(400 * i, 400, 90, COLOR_RED, 1);
        beam2 = Beams(400 * i, 200, 90, COLOR_RED, 1);

        mypairbeams.push_back(make_pair(beam1, beam2));
    }

    for (int i = 2; i < 1000; i += 8)
    {
        Boomerang boom1;
        boom1 = Boomerang(500 * i + rand() % 25, 700, COLOR_BLACK);
        myboomerangs.push_back(boom1);
    }

    for (int i = 4; i <= 100; i += 15)
    {
        Semicircle semi1 = Semicircle(500 * i + rand() % 25, 300 + rand() % 200, 150, COLOR_BLACK);
        mysemi.push_back(semi1);
    }

    for (int i = 3; i < 1000; i += 4)
    {
        if (rand() % 2)
        {
            Bonus bonus1 = Bonus(500 * i + rand() % 25, 250, 20, 1, COLOR_BLACK);
            mybonus.push_back(bonus1);
        }
        else
        {
            Bonus bonus1 = Bonus(500 * i + rand() % 25, 200, 30, 2, COLOR_GOLD);
            mybonus.push_back(bonus1);
        }
    }

    magnet = Magnets(5000, 5000, 30, COLOR_DARKBLUE);
    ball1 = Ball(0, 0, COLOR_RED);
    base = Floor(COLOR_GREEN);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL(window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

pair<float, float> myrotate(float angle, float x, float y, float origx, float origy)
{
    pair<float, float> rotated_point;

    rotated_point.first = (x - origx) * cos(angle) - (y - origy) * sin(angle) + origx;
    rotated_point.second = (x - origx) * sin(angle) + (y - origy) * cos(angle) + origy;
    return rotated_point;
}

bool line_eq(pair<float, float> line1, pair<float, float> line2, pair<float, float> pnt)
{
    float y = pnt.second;
    float y1 = line1.second;
    float y2 = line2.second;
    float x = pnt.first;
    float x1 = line1.first;
    float x2 = line2.first;

    float myvar = ((y - y1) * (x2 - x1)) - ((y2 - y1) * (x - x1));

    return (myvar >= 0) ? true : false;
}

bool lines_collision(pair<float, float> line1, pair<float, float> line2, pair<float, float> same1,
                     pair<float, float> same2, pair<float, float> diff1, pair<float, float> diff2, pair<float, float> diff3,
                     pair<float, float> diff4)
{

    bool s1 = line_eq(line1, line2, same1);
    bool s2 = line_eq(line1, line2, same2);

    bool d1 = line_eq(line1, line2, diff1);
    bool d2 = line_eq(line1, line2, diff2);
    bool d3 = line_eq(line1, line2, diff3);
    bool d4 = line_eq(line1, line2, diff4);

    if (s1 == s2 and d1 == d2 and d2 == d3 and d3 == d4 and s1 != d1)
        return true;
    else
        return false;
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{

    pair<float, float> ap1, ap2, ap3, ap4, bp1, bp2, bp3, bp4;

    ap1 = myrotate(a.angle, a.x + (a.width / 2), a.y + (a.height / 2), a.x, a.y);
    ap2 = myrotate(a.angle, a.x + (a.width / 2), a.y - (a.height / 2), a.x, a.y);
    ap3 = myrotate(a.angle, a.x - (a.width / 2), a.y - (a.height / 2), a.x, a.y);
    ap4 = myrotate(a.angle, a.x - (a.width / 2), a.y + (a.height / 2), a.x, a.y);

    bp1 = myrotate(b.angle, b.x + (b.width / 2), b.y + (b.height / 2), b.x, b.y);
    bp2 = myrotate(b.angle, b.x + (b.width / 2), b.y - (b.height / 2), b.x, b.y);
    bp3 = myrotate(b.angle, b.x - (b.width / 2), b.y - (b.height / 2), b.x, b.y);
    bp4 = myrotate(b.angle, b.x - (b.width / 2), b.y + (b.height / 2), b.x, b.y);

    if (lines_collision(ap1, ap2, ap3, ap4, bp1, bp2, bp3, bp4))
        return false;

    if (lines_collision(ap2, ap3, ap4, ap1, bp1, bp2, bp3, bp4))
        return false;

    if (lines_collision(ap3, ap4, ap1, ap2, bp1, bp2, bp3, bp4))
        return false;

    if (lines_collision(ap4, ap1, ap2, ap3, bp1, bp2, bp3, bp4))
        return false;

    if (lines_collision(bp1, bp2, bp3, bp4, ap1, ap2, ap3, ap4))
        return false;

    if (lines_collision(bp2, bp3, bp4, bp1, ap1, ap2, ap3, ap4))
        return false;

    if (lines_collision(bp3, bp4, bp1, bp2, ap1, ap2, ap3, ap4))
        return false;

    if (lines_collision(bp4, bp1, bp2, bp3, ap1, ap2, ap3, ap4))
        return false;

   
    return true;
}

void reset_screen()
{
    float top = screen_center_y + 600 / screen_zoom;
    float bottom = screen_center_y - 600 / screen_zoom;
    float left = screen_center_x - 600 / screen_zoom;
    float right = screen_center_x + 600 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
