#include "main.h"
#include "timer.h"
#include "player.h"
#define debug(x) cerr << #x << " is " << x << endl;

using namespace std;

GLMatrices Matrices;
GLuint     programID;
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
vector<Boomerang> myboomerangs;
vector< pair <Beams,Beams> > mypairbeams;


float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
int f = 0;
Timer t60(1.0 / 60);
Timer t1(1.0 / 4);
Timer magapp(12.0 / 1);
Timer magdisapp(3.94/1);

bool magpres = 0;


/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye (0, 1.5*150, 25);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 1.5*150, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    base.draw(VP);
    magnet.draw(VP);

    for(int i=0;i<mycoins.size();i++)
        mycoins[i].draw(VP);
    for(int i=0;i<mybeams.size();i++)
        mybeams[i].draw(VP);
    for(int i=0;i<myballoons.size();i++)
        myballoons[i].draw(VP);
    for(int i=0;i<myboomerangs.size();i++)
        myboomerangs[i].draw(VP);
    for(int i=0;i<myboomerangs.size();i++)
    {
        mypairbeams[i].first.draw(VP);
        mypairbeams[i].second.draw(VP);    
    }   
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int water = glfwGetKey(window, GLFW_KEY_SPACE);
    
    if (left) {
       ball1.tick(DIR_LEFT);
    }
    if (right) {
        ball1.tick(DIR_RIGHT);
    }
    if(up){
        ball1.tick(DIR_UP);
    }
    if(!(up or left or right)){
        ball1.tick(DIR_DOWN);
    }
    if(water and t1.processTick()){
        Balloons balloons1 = Balloons(ball1.position.x+50.0f,ball1.position.y+50.0f,ball1.speedx,ball1.speedy,20.0f,COLOR_BLUE);
        myballoons.push_back(balloons1);
       // cout<<myballoons.size()<<endl;
    }
}

void tick_elements() {
    //camera_rotation_angle += 1;
    if(magapp.processTick())
    {
        cout<<"appear"<<endl;
        magpres=1;
        magnet.position.x = -350+rand()%700;
        magnet.position.y =  0+rand()%700;
    }
    if(magdisapp.processTick())
    {
        cout<<"disappear"<<endl;
        magpres=0;
        magnet.position.x = 900;
        magnet.position.y = 900;
    }    
    if(magpres)
    {
        if(magnet.position.x - ball1.position.x > 0)
            ball1.position.x += 5;
        
        if(magnet.position.x - ball1.position.x < 0)
            ball1.position.x -= 5;

        if(magnet.position.y - ball1.position.y > 0)
        {
            ball1.speedy = 0.0;
            ball1.position.y += 5;
        }

        if(magnet.position.y - ball1.position.y < 0)
        {

            ball1.speedy = 0.0;
            ball1.position.y -= 5;
        }  
    }

    for(int i=0;i<mycoins.size();i++)
        mycoins[i].tick();
    
    for(int i=0;i<mybeams.size();i++)
        mybeams[i].tick(SINGLE);
    
    for(int i=0;i<myballoons.size();i++)
        myballoons[i].tick();
    
    for(int i=0;i<myboomerangs.size();i++)
        myboomerangs[i].tick();
    
    for(int i=0;i<mypairbeams.size();i++)
    {
        mypairbeams[i].first.tick(DOUBLE_TOP);
        mypairbeams[i].second.tick(DOUBLE_BOTTOM);
    }
    //cout<<myboomerangs.size()<<endl;
    
    
    bounding_box_t boxplayer = ball1.box;
    
    for(int i = 0; i < mycoins.size(); i++)
    {
        bounding_box_t boxcoin = mycoins[i].box;
        if(detect_collision(boxplayer,boxcoin))
        {
            printf("collide\n");
            printf("x position ball %f\n", ball1.position.x);
            printf("y position ball %f\n", ball1.position.y);
            printf("x position ball %f\n", mycoins[i].position.x);
            printf("y position ball %f\n", mycoins[i].position.y);
          //  usleep(100000000);
            mycoins.erase(mycoins.begin()+i);
        }
    }
    
    for(int i = 0; i < mybeams.size(); i++)
    {
        bounding_box_t boxbeam = mybeams[i].box;
        if(detect_collision(boxplayer,boxbeam))
        {
            printf("collide\n");
            printf("x position ball %f\n", ball1.position.x);
            printf("y position ball %f\n", ball1.position.y);
            printf("x position ball %f\n", mybeams[i].position.x);
            printf("y position ball %f\n", mybeams[i].position.y);
          //  usleep(100000000);
            mybeams.erase(mybeams.begin()+i);
        }
    }
    for(int i = 0; i < mypairbeams.size(); i++)
    {
        bounding_box_t boxbeam1 = mypairbeams[i].first.box;
        bounding_box_t boxbeam2 = mypairbeams[i].second.box;

        if(detect_collision(boxplayer,boxbeam1) or detect_collision(boxplayer,boxbeam2))
        {
            printf("collide\n");
            printf("x position ball %f\n", ball1.position.x);
            printf("y position ball %f\n", ball1.position.y);
            mypairbeams.erase(mypairbeams.begin()+i);
        }
    }
    
    for(int i = 0; i < myboomerangs.size(); i++)
    {
        bounding_box_t boxboomerang = myboomerangs[i].box;
        if(detect_collision(boxplayer,boxboomerang))
        {
            printf("collide\n");
            printf("x position ball %f\n", ball1.position.x);
            printf("y position ball %f\n", ball1.position.y);
            printf("x position ball %f\n", myboomerangs[i].position.x);
            printf("y position ball %f\n", myboomerangs[i].position.y);
          //  usleep(100000000);
            myboomerangs.erase(myboomerangs.begin()+i);
        }
        if(myboomerangs[i].position.y<0)
            myboomerangs.erase(myboomerangs.begin()+i);
    }

    for(int i = 0; i < mybeams.size(); i++)
    {
        for(int j = 0 ; j < myballoons.size();j++)
        {
            bounding_box_t boxbeam = mybeams[i].box;
            bounding_box_t boxballoon = myballoons[j].box;
            
            if(detect_collision(boxballoon,boxbeam))
            {
                printf("collide\n");
                printf("x position ball %f\n", ball1.position.x);
                printf("y position ball %f\n", ball1.position.y);
                printf("x position ball %f\n", mybeams[i].position.x);
                printf("y position ball %f\n", mybeams[i].position.y);
                mybeams.erase(mybeams.begin()+i);
                myballoons.erase(myballoons.begin()+j);
            }

            if(myballoons[j].position.y <= -45.0f)
                myballoons.erase(myballoons.begin()+j);
        }
    }


    // printf("x position %f\n", ball1.position.x);
    // printf("x position %f\n", ball1.position.x);
    // printf("y position %f\n", ball1.position.y);
    // printf("x speed %f\n", ball1.speedx);
    // printf("y speed %f\n", ball1.speedy);
    // printf("acc %f\n", ball1.accy);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    
    for(int i=0;i<1000;i+=2)
    {
        Coins coin1;
        if(rand()%3>=1)
            coin1 = Coins(500*i+rand()%25,rand()%600+100,30,COLOR_NEONGREEN);
        else
            coin1 = Coins(500*i+rand()%25,rand()%600+100,20,COLOR_SILVER);

        mycoins.push_back(coin1);
    }
    
    for(int i=0;i<1000;i+=2)
    {
        Beams beam1;
        beam1 = Beams(500*i+rand()%25,rand()%600+200,rand()%180,COLOR_BEAM);
        mybeams.push_back(beam1);
    }
    
    for(int i=2;i<1000;i+=4)
    {
        Beams beam1;
        Beams beam2;

        beam1 = Beams(400*i,400,90,COLOR_RED,1);
        beam2 = Beams(400*i,200,90,COLOR_RED,1);

        mypairbeams.push_back(make_pair(beam1,beam2));
    }

    for(int i=2;i<1000;i+=8)
    {
        Boomerang boom1;
        boom1 = Boomerang(500*i+rand()%25,700,COLOR_BLACK);
        myboomerangs.push_back(boom1);
    }

    
   
    magnet   = Magnets(900,900,30,COLOR_DARKBLUE);
    ball1    = Ball(0, 0, COLOR_RED);
    base     = Floor(COLOR_GREEN); 
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
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

pair<float,float> myrotate(float angle, float x , float y,float origx,float origy)
{
    pair <float,float> rotated_point;
    if(f)
    {
        debug(angle);
        debug(x);
        debug(y);
    }
    rotated_point.first = (x-origx)*cos(angle)-(y-origy)*sin(angle)+origx;
    rotated_point.second = (x-origx)*sin(angle)+(y-origy)*cos(angle)+origy;
    return rotated_point;
}

bool line_eq(pair<float,float> line1 , pair<float,float> line2 , pair<float,float> pnt)
{
    float y = pnt.second;
    float y1 = line1.second;
    float y2 = line2.second;
    float x = pnt.first;
    float x1 = line1.first;
    float x2 = line2.first;

    float myvar = ((y-y1)*(x2-x1))-((y2-y1)*(x-x1));
    // if(f)
    // {
    //     debug(y);
    //     debug(y1);
    //     debug(y2);
    //     debug(x);
    //     debug(x1);
    //     debug(x2);

    //     debug(myvar);
    // }
    return (myvar >= 0 ) ? true : false;
    
}

bool lines_collision( pair<float,float> line1 , pair<float,float> line2,pair<float,float> same1,
pair<float,float> same2,pair<float,float> diff1,pair<float,float> diff2,pair<float,float> diff3,
pair<float,float> diff4 )
{

    bool s1 = line_eq(line1,line2,same1);
    bool s2 = line_eq(line1,line2,same2);

    bool d1 = line_eq(line1,line2,diff1);
    bool d2 = line_eq(line1,line2,diff2);
    bool d3 = line_eq(line1,line2,diff3);
    bool d4 = line_eq(line1,line2,diff4);
    // if(f)
    // {
    //     debug(s1);
    //     debug(s2);

    //     debug(d1);
    //     debug(d2);
    //     debug(d3);
    //     debug(d4);
    // }

    if(s1==s2 and d1==d2 and d2==d3 and d3==d4 and s1!=d1)
        return true;
    else
        return false;
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    
    pair<float,float> ap1,ap2,ap3,ap4,bp1,bp2,bp3,bp4; 
    
    ap1 = myrotate(a.angle,a.x+(a.width/2),a.y+(a.height/2),a.x,a.y); 
    ap2 = myrotate(a.angle,a.x+(a.width/2),a.y-(a.height/2),a.x,a.y); 
    ap3 = myrotate(a.angle,a.x-(a.width/2),a.y-(a.height/2),a.x,a.y); 
    ap4 = myrotate(a.angle,a.x-(a.width/2),a.y+(a.height/2),a.x,a.y); 

    bp1 = myrotate(b.angle,b.x+(b.width/2),b.y+(b.height/2),b.x,b.y); 
    bp2 = myrotate(b.angle,b.x+(b.width/2),b.y-(b.height/2),b.x,b.y); 
    bp3 = myrotate(b.angle,b.x-(b.width/2),b.y-(b.height/2),b.x,b.y); 
    bp4 = myrotate(b.angle,b.x-(b.width/2),b.y+(b.height/2),b.x,b.y); 

    if ( (abs(a.x - b.x) * 2 < (a.width + b.width)) && (abs(a.y - b.y) * 2 < (a.height + b.height)) )
    {
        printf("collision\n");
    //    f=1;
        debug(ball1.position.x);
        debug(ball1.position.y);
        ap1 = myrotate(a.angle,a.x+(a.width/2),a.y+(a.height/2),a.x,a.y); 
        debug(ap1.first);
        debug(ap1.second);
        debug(ap2.first);
        debug(ap2.second);
        debug(ap3.first);
        debug(ap3.second);
        debug(ap4.first);
        debug(ap4.second);
        debug(bp1.first);
        debug(bp1.second);
        debug(bp2.first);
        debug(bp2.second);
        debug(bp3.first);
        debug(bp3.second);
        debug(bp4.first);
        debug(bp4.second);
        lines_collision(ap1,ap2,ap3,ap4,bp1,bp2,bp3,bp4);
       // usleep(1000000000);
    }   

    if(lines_collision(ap1,ap2,ap3,ap4,bp1,bp2,bp3,bp4))
        return false;    
    
    if(lines_collision(ap2,ap3,ap4,ap1,bp1,bp2,bp3,bp4))
        return false;
    
    if(lines_collision(ap3,ap4,ap1,ap2,bp1,bp2,bp3,bp4))
        return false;
    
    if(lines_collision(ap4,ap1,ap2,ap3,bp1,bp2,bp3,bp4))
        return false;
    
    if(lines_collision(bp1,bp2,bp3,bp4,ap1,ap2,ap3,ap4))
        return false;    
    
    if(lines_collision(bp2,bp3,bp4,bp1,ap1,ap2,ap3,ap4))
        return false;
    
    if(lines_collision(bp3,bp4,bp1,bp2,ap1,ap2,ap3,ap4))
        return false;
    
    if(lines_collision(bp4,bp1,bp2,bp3,ap1,ap2,ap3,ap4))
        return false;
    
        // printf("collision\n");
        // f=1;
        // debug(ball1.position.x);
        // debug(ball1.position.y);
        // debug(ap1.first);
        // debug(ap1.second);
        // debug(ap2.first);
        // debug(ap2.second);
        // debug(ap3.first);
        // debug(ap3.second);
        // debug(ap4.first);
        // debug(ap4.second);
        // debug(bp1.first);
        // debug(bp1.second);
        // debug(bp2.first);
        // debug(bp2.second);
        // debug(bp3.first);
        // debug(bp3.second);
        // debug(bp4.first);
        // debug(bp4.second);
        // lines_collision(ap1,ap2,ap3,ap4,bp1,bp2,bp3,bp4);
        // usleep(1000000000);
    return true;
    
}


void reset_screen() {
    float top    = screen_center_y + 600 / screen_zoom;
    float bottom = screen_center_y - 600 / screen_zoom;
    float left   = screen_center_x - 600 / screen_zoom;
    float right  = screen_center_x + 600 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
