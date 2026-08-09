#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Photon {
    double r;
    double theta;
    double phi;
};

double M = 1.0;
double E = 1.0;
double L = 3.0;
double dt = 0.002;

double radial_speed(double r) {
    double f = 1.0 - 2.0*M/r;
    double val = E*E - f*L*L/(r*r);
    if(val < 0) return 0.0;
    return sqrt(val);
}

void derivatives(const Photon &p, double &drdl, double &dthetadl, double &dphidl) {
    drdl = radial_speed(p.r);
    dthetadl = 0.0;
    dphidl = L/(p.r*p.r);
}

void rk4_step(Photon &p) {
    double dr1,dth1,dphi1, dr2,dth2,dphi2, dr3,dth3,dphi3, dr4,dth4,dphi4;
    derivatives(p, dr1,dth1,dphi1);
    Photon tmp = {p.r + 0.5*dt*dr1, p.theta + 0.5*dt*dth1, p.phi + 0.5*dt*dphi1};
    derivatives(tmp, dr2,dth2,dphi2);
    tmp = {p.r + 0.5*dt*dr2, p.theta + 0.5*dt*dth2, p.phi + 0.5*dt*dphi2};
    derivatives(tmp, dr3,dth3,dphi3);
    tmp = {p.r + dt*dr3, p.theta + dt*dth3, p.phi + dt*dphi3};
    derivatives(tmp, dr4,dth4,dphi4);
    p.r     += dt/6.0 * (dr1 + 2*dr2 + 2*dr3 + dr4);
    p.theta += dt/6.0 * (dth1 + 2*dth2 + 2*dth3 + dth4);
    p.phi   += dt/6.0 * (dphi1 + 2*dphi2 + 2*dphi3 + dphi4);
}

void spherical_to_cartesian(const Photon &p, float &x, float &y, float &z) {
    double sin_theta = sin(p.theta);
    x = float(p.r * sin_theta * cos(p.phi));
    y = float(p.r * sin_theta * sin(p.phi));
    z = float(p.r * cos(p.theta));
}

void perspectiveGL(double fovY, double aspect, double zNear, double zFar) {
    double fH = tan(fovY / 360.0 * M_PI) * zNear;
    double fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void drawSphere(double radius, int slices, int stacks) {
    for(int i=0; i<=stacks; i++){
        double theta = M_PI * i / stacks;
        for(int j=0; j<=slices; j++){
            double phi = 2*M_PI * j / slices;
            double x = radius * sin(theta) * cos(phi);
            double y = radius * sin(theta) * sin(phi);
            double z = radius * cos(theta);
            glVertex3f(x, y, z);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspect = (double)width / (double)height;
    perspectiveGL(60.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800,800,"White Hole 3D Room",NULL,NULL);
    if(!window){ glfwTerminate(); return -1;}

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glPointSize(2.0);

    std::vector<Photon> photons;
    int num_photons = 3000;
    for(int i=0;i<num_photons;i++){
        double theta0 = acos(2.0*(rand()/(double)RAND_MAX) - 1.0);
        double phi0   = 2*M_PI * (rand()/(double)RAND_MAX);
        photons.push_back({2.05, theta0, phi0});
    }

    double angle = 0.0;
    int width = 800, height = 800;
    framebuffer_size_callback(window, width, height);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslated(0,0,-12);
        glRotated(angle, 0,1,0);
        glRotated(20, 1,0,0);
        angle += 0.2;

        glColor3f(1,1,1);
        glBegin(GL_POINTS);
        for(auto &p : photons){
            rk4_step(p);
            float x,y,z;
            spherical_to_cartesian(p,x,y,z);
            glVertex3f(x/5.0f, y/5.0f, z/5.0f);
        }
        glEnd();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glColor4f(1,1,1,0.3f);
        glBegin(GL_POINTS);
        drawSphere(2.5/5.0, 60, 30);
        glEnd();

        glColor4f(1,1,1,1.0f);
        glBegin(GL_POINTS);
        drawSphere(2.0/5.0, 40, 20);
        glEnd();
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
