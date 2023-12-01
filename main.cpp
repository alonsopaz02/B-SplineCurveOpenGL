//PC03 COMPUTACION VISUAL
//ALUMNOS:
//Alvarado Torres Alexandra Luisa (21200002)
//Paz Guillen Jose Alonso (21200089)

#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif
#include <cmath>
#define NUM_MAXIMO_PTOSDCTRL 100
#include <algorithm>

// Definicion de las funciones para la curva B-Spline
float F0(float u)
{
    return (1 - u) * (1 - u) * (1 - u) / 6.0f;
}
float F1(float u)
{
    return (3 * u * u * u - 6 * u * u + 4) / 6.0f;
}
float F2(float u)
{
    return (-3 * u * u * u + 3 * u * u + 3 * u + 1) / 6.0f;
}
float F3(float u)
{
    return u * u * u / 6.0f;
}

//Variable para rotacion de camara de superficie reglada
//Se utiliza de manera adicional con las teclas X y C mayus
static float incrementoA = 25;

// Arreglo para almacenar los puntos de control
float puntosDeControl[NUM_MAXIMO_PTOSDCTRL][2]; // Usar un arreglo para almacenar los puntos de control
int numeroDePuntosDeControl = 0;

// Variables para los colores de la curva B-Spline
float rojoCurva = 0.0f;
float verdeCurva = 0.0f;
float azulCurva = 1.0f;

//Variable para los colores de la superficie reglada
float rojoSuperficie = 0.0f;
float verdeSuperficie = 0.0f;
float azulSuperficie = 1.0f;

// Variable para el grosor de la curva
float grosor = 2.0f;

//Variable de variacion de la altura de la superficie reglada
float variacionAltura =-0.1;

//Niveles verticales que tendra la superficie reglada
int nivelesSuperficie = 0;

//Variable de veces que se presiona la opcion de activar y desactivar ingreso de puntos de control
//Usada para manejar cuando vaciar el arreglo de puntos de control para iniciar uno nuevo
int pulsaciones = 0;


bool modoMoverPuntos = false; // Variable para activar o desactivar el mover puntos
bool modoAgregarPuntos = false; // Variable para activar o desactivar la agregacion de puntos
bool mostrarEjes = false; // Variable para activar o desactivar ejes 2D
bool mostrarPolilinea = false; // Variable para mostrar polilinea
bool mostrarCurvaBSpline = false; // Variable para mostrar la curva B-Spline


int puntoSeleccionado = -1; // Indice de punto seleccionado para mover

//Funciones prototipo
void dibujarLadoDerecho(void);
void dibujarSuperficieReglada(void);
void mostrarEjes2D(void);
void dibujarPuntosDeControl(void);
void dibujarPolilinea(void);
void dibujarCurvaBSpline(void);
void clickeoParaPuntos(int, int, int, int );
void movimientoDelMouse(int, int );


void dibujarLadoDerecho()
{


    //Dibujamos el tablero 3D
    //Para el tablero 3D se dibuja un prisma desde 0
    //Se opto por el prisma en lugar de un glutWireCube con escalamiento
    //Se da el efecto 3D de igual forma
    glColor3f(0.5, 0.7, 1.0);
    glBegin(GL_QUADS);
    //Tapa
    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 1.0);

    //Base
    glVertex3f(0.0, 0.05, -1.0); // Esquina inferior izquierda
    glVertex3f(1.0, 0.05, -1.0); // Esquina inferior derecha
    glVertex3f(1.0, 0.05, 1.0); // Esquina superior derecha
    glVertex3f(0.0, 0.05, 1.0); // Esquina superior izquierda

    //Lados
    glVertex3f(1.0, 0.05, 1.0);
    glVertex3f(1.0, 0, 1.0);
    glVertex3f(1.0, 0.05, -1.0);
    glVertex3f(1.0, 0, -1.0);

    glVertex3f(0.0, 0.05, -1.0);
    glVertex3f(0.0, 0, -1.0);
    glVertex3f(1.0, 0.05, -1.0);
    glVertex3f(1.0, 0, -1.0);
    glEnd();

    //Ejes 3D
    glLineWidth(1);
    glColor3f(1.0, 0.0, 0.0); // Eje X: Rojo
    glBegin(GL_LINES);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(1.25, 0.0, 0.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0); // Eje Y: Verde
    glBegin(GL_LINES);
    glVertex3f(0.5, 0, 0.0);
    glVertex3f(0.5, -0.75, 0.0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0); // Eje Z: Azul
    glBegin(GL_LINES);
    glVertex3f(0.5, 0.0, -2);
    glVertex3f(0.5, 0.0, 0);
    glEnd();

    //Bordes negros para el tablero 3D
    glLineWidth(2);
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(1.0, 0.05, 1.0);
    glVertex3f(1.0, 0.05, -1.0);
    glVertex3f(0.0, 0.05, -1.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(1.0, 0.05, 1.0);
    glVertex3f(1.0, 0, 1.0);
    glVertex3f(1.0, 0.05, -1.0);
    glVertex3f(1.0, 0, -1.0);
    glVertex3f(0.0, 0.05, -1.0);
    glVertex3f(0.0, 0, -1.0);

    glEnd();

    glLineWidth(1);

    //Trasladamos la superficie reglada a dibujar hacia la derecha
    glTranslatef(1,0,0);

    //Llamamos a la funcion para graficar
    dibujarSuperficieReglada();
}

//Metodo para dibujar la superficie reglada
void dibujarSuperficieReglada()
{
    //Debemos dibujar siempre y cuando este activada la opcion para mostrar la curva
    //Dibujar cuando los puntos de control sean mas o igual que 4
    if (mostrarCurvaBSpline && numeroDePuntosDeControl >= 4)
    {
        //Damos color
        glColor3f(rojoSuperficie, verdeSuperficie, azulSuperficie);

        //Aumento de u para agarrar el siguiente par de puntos
        float aumento=0.1;

        //Dibujamos los cuadrilateros
        glBegin(GL_QUADS);
        for (int i = 0; i < numeroDePuntosDeControl - 3; ++i)
        {
            for (float u = 0; u <= 1; u += aumento)
            {
                float x1 = F0(u) * puntosDeControl[i][0] + F1(u) * puntosDeControl[i + 1][0] +
                           F2(u) * puntosDeControl[i + 2][0] + F3(u) * puntosDeControl[i + 3][0];
                float y1 = F0(u) * puntosDeControl[i][1] + F1(u) * puntosDeControl[i + 1][1] +
                           F2(u) * puntosDeControl[i + 2][1] + F3(u) * puntosDeControl[i + 3][1];

                float x2 = F0(u+aumento) * puntosDeControl[i][0] + F1(u+aumento) * puntosDeControl[i + 1][0] +
                           F2(u+aumento) * puntosDeControl[i + 2][0] + F3(u+aumento) * puntosDeControl[i + 3][0];
                float y2 = F0(u+aumento) * puntosDeControl[i][1] + F1(u+aumento) * puntosDeControl[i + 1][1] +
                           F2(u+aumento) * puntosDeControl[i + 2][1] + F3(u+aumento) * puntosDeControl[i + 3][1];
                //Recordar que nuestra curva se situa sobre el eje XZ
                glVertex3f(x1, 0,y1);
                glVertex3f(x1, variacionAltura*nivelesSuperficie,y1);

                    glVertex3f(x2, variacionAltura*nivelesSuperficie,y2);
                    glVertex3f(x2, 0,y2);

            }
        }
        glEnd();

        //Grosor de la linea de la superficie reglada
        glLineWidth(2);

        //Las lineas de division de la superficie
        glColor3f(0,0,0);
        for(int j=0; j<=nivelesSuperficie; j++)
        {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < numeroDePuntosDeControl - 3; ++i)
            {
                for (float u = 0; u <= 1; u += 0.2)
                {
                    float x = F0(u) * puntosDeControl[i][0] + F1(u) * puntosDeControl[i + 1][0] +
                              F2(u) * puntosDeControl[i + 2][0] + F3(u) * puntosDeControl[i + 3][0];
                    float y = F0(u) * puntosDeControl[i][1] + F1(u) * puntosDeControl[i + 1][1] +
                              F2(u) * puntosDeControl[i + 2][1] + F3(u) * puntosDeControl[i + 3][1];
                    glVertex3f(x, variacionAltura*j,y);
                    glVertex3f(x, 0,y);
                    glVertex3f(x, variacionAltura*j,y);
                }
            }
            glEnd();
        }

    }
}

void mostrarEjes2D()
{
    //Si flag activada
    if (mostrarEjes)
    {
        glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex2f(-0.75, 0.0); // Eje X
        glVertex2f(-0.25, 0.0);
        glColor3f(0,0,1);
        glVertex2f(-0.5, -0.5); // Eje Y
        glVertex2f(-0.5, 0.5);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex2f(-0.25, 0.03);
        glVertex2f(-0.25, -0.03);
        glVertex2f(-0.23, 0.0);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0,0,1);
        glVertex2f(-0.52, 0.5); // Eje Y
        glVertex2f(-0.48, 0.5);
        glVertex2f(-0.5, 0.54);
        glEnd();
    }
}

void dibujarPuntosDeControl()
{
    glColor3f(1.0, 0.0, 0.0); // Color rojo
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < numeroDePuntosDeControl; ++i)
    {
        glVertex2f(puntosDeControl[i][0], puntosDeControl[i][1]);
    }
    glEnd();
}

void dibujarPolilinea()
{
    if (mostrarPolilinea && numeroDePuntosDeControl > 1)
    {
        glLineWidth(2);
        glColor3f(1.0, 0.6, 0.0); // Color para la polilínea
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numeroDePuntosDeControl; ++i)
        {
            glVertex2f(puntosDeControl[i][0], puntosDeControl[i][1]);
        }
        glEnd();
        glLineWidth(1);
    }
}

void dibujarCurvaBSpline()
{
    //Dibujo de la curva con la formula dada en el PDF
    if (mostrarCurvaBSpline && numeroDePuntosDeControl >= 4)
    {
        glLineWidth(grosor);
        glColor3f(rojoCurva, verdeCurva, azulCurva);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numeroDePuntosDeControl - 3; ++i)
        {
            for (float u = 0; u <= 1; u += 0.01)
            {
                float x = F0(u) * puntosDeControl[i][0] + F1(u) * puntosDeControl[i + 1][0] +
                          F2(u) * puntosDeControl[i + 2][0] + F3(u) * puntosDeControl[i + 3][0];
                float y = F0(u) * puntosDeControl[i][1] + F1(u) * puntosDeControl[i + 1][1] +
                          F2(u) * puntosDeControl[i + 2][1] + F3(u) * puntosDeControl[i + 3][1];
                glVertex2f(x, y);
            }
        }
        glEnd();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
    //Dibujaremos una linea vertical que separara ambos lados de la pantalla
    glColor3f(0.0, 1.0, 0.0); // Color verde para la línea
    glBegin(GL_LINES);
    glVertex2f(0.0, -1.0); // Extremo inferior
    glVertex2f(0.0, 1.0);  // Extremo superior
    glEnd();

    //Lado izquierdo
    glPushMatrix();
    mostrarEjes2D();
    dibujarPuntosDeControl();
    dibujarPolilinea();
    dibujarCurvaBSpline();
    glPopMatrix();

    // Lado derecho
    //Transformaciones para mostrar perspectiva
    glPushMatrix();
    glRotatef(170,0,1,0);
    glRotatef(180,0,0,1);
    glRotatef(incrementoA,1.0,0.0,0.0);
    glTranslatef(0.15,0,0);
    glScalef(0.7, 0.7, 0.7);
    dibujarLadoDerecho();
    glPopMatrix();
    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void clickeoParaPuntos(int button, int state, int x, int y)
{
    if(x<glutGet(GLUT_WINDOW_WIDTH)/2)
    {
        if (!modoMoverPuntos && modoAgregarPuntos && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
                numeroDePuntosDeControl < NUM_MAXIMO_PTOSDCTRL)
        {
            // Normalizacion para que los puntos esten entre 1 y -1
            float mouseX = (2.0 * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0;
            float mouseY = 1.0 - (2.0 * y) / glutGet(GLUT_WINDOW_HEIGHT);

            if (mouseX >= -1.0 && mouseX <= 1.0 && mouseY >= -1.0 && mouseY <= 1.0)
            {
                puntosDeControl[numeroDePuntosDeControl][0] = mouseX;
                puntosDeControl[numeroDePuntosDeControl][1] = mouseY;
                numeroDePuntosDeControl++;

                glutPostRedisplay();
            }
        }
        else if (modoMoverPuntos && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            // Caso para poder mover los puntos (se selecciona la coordenada para el indice de un punto)
            float mouseX = (2.0 * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0;
            float mouseY = 1.0 - (2.0 * y) / glutGet(GLUT_WINDOW_HEIGHT);

            for (int i = 0; i < numeroDePuntosDeControl; ++i)
            {
                float distancia = sqrt((mouseX - puntosDeControl[i][0]) * (mouseX - puntosDeControl[i][0]) +
                                       (mouseY - puntosDeControl[i][1]) * (mouseY - puntosDeControl[i][1]));
                if (distancia < 0.05)
                {
                    puntoSeleccionado = i;
                    break;
                }
            }
        }
        else if (modoMoverPuntos && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        {
            //Cuando se suelta el mouse el punto se deja de seleccionar
            puntoSeleccionado = -1;
        }
    }
}



void movimientoDelMouse(int x, int y)
{
    if(x<glutGet(GLUT_WINDOW_WIDTH)/2 && modoMoverPuntos && puntoSeleccionado != -1)
    {
        if (modoMoverPuntos && puntoSeleccionado != -1)
        {
            // Movemos el punto seleccionado con el mouse
            float mouseX = (2.0 * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0;
            float mouseY = 1.0 - (2.0 * y) / glutGet(GLUT_WINDOW_HEIGHT);

            // Tenemos que verificar que las coordenadas esten dentro de los limites de la pantalla
            mouseX = std::max(-1.0f, std::min(1.0f, mouseX));
            mouseY = std::max(-1.0f, std::min(1.0f, mouseY));

            puntosDeControl[puntoSeleccionado][0] = mouseX;
            puntosDeControl[puntoSeleccionado][1] = mouseY;

            glutPostRedisplay();
        }
    }
}

//CREACION DE MENUS
void menu_principal(int value)
{
    switch (value)
    {
    case 9:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void menu_ingresar(int opcion)
{
    switch(opcion)
    {
    case 1:
        if(pulsaciones%2==0)
        {
            //Cuando limpiamos la matriz, reiniciamos todas las opciones
            modoAgregarPuntos = true;
            mostrarEjes = false;
            mostrarPolilinea = false;
            mostrarCurvaBSpline = false;
            modoMoverPuntos = false;
            pulsaciones++;
            for (int i = 0; i < NUM_MAXIMO_PTOSDCTRL; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    puntosDeControl[i][j] = 0.0f; // Establece cada punto a 0 otra vez
                }
            }
            numeroDePuntosDeControl = 0; // Restablece el número de puntos de control a 0
        }
        break;
    case 2:
        if(pulsaciones%2==1)
        {
            modoAgregarPuntos = false;
            pulsaciones++;
        }
        break;
    }
    glutPostRedisplay();

}

void menu_poli(int opcion)
{
    switch(opcion)
    {
    case 1:
        mostrarPolilinea = true;
        break;
    case 2:
        mostrarPolilinea = false;
        break;
    }
    glutPostRedisplay();
}

void menu_curva(int opcion)
{
    switch(opcion)
    {
    case 1:
        mostrarCurvaBSpline = true;
        break;
    case 2:
        mostrarCurvaBSpline = false;
        break;
    }
    glutPostRedisplay();
}

void menu_moverpuntos(int opcion)
{
    switch(opcion)
    {
    case 1:
        modoMoverPuntos = true;
        break;
    case 2:
        modoMoverPuntos = false;
        break;
    }

}

void menu_ejes(int opcion)
{
    switch(opcion)
    {
    case 1:
        mostrarEjes = true;
        break;
    case 2:
        mostrarEjes = false;
        break;
    }
    glutPostRedisplay();
}

void menu_color(int opcion)
{
    switch(opcion)
    {
    case 1:
        rojoCurva = 1.0f;
        verdeCurva = 0.0f;
        azulCurva = 0.0f; // Rojo
        break;
    case 2:
        rojoCurva = 0.0f;
        verdeCurva = 1.0f;
        azulCurva = 0.0f; // Verde
        break;
    case 3:
        rojoCurva = 0.0f;
        verdeCurva = 0.0f;
        azulCurva = 1.0f; // Azul
        break;
    }
    glutPostRedisplay();
}

void menu_grosor(int opcion)
{
    switch(opcion)
    {
    case 1:
        grosor = 2.0f; // Delgado
        break;
    case 2:
        grosor = 5.0f; // Mediano
        break;
    case 3:
        grosor = 7.0f; // Grueso
        break;
    }
    glutPostRedisplay();

}

void menu_superficie(int opcion)
{
    switch(opcion)
    {
    case 1:
        rojoSuperficie = 1.0f;
        verdeSuperficie = 0.0f;
        azulSuperficie = 0.0f; // Rojo
        break;
    case 2:
        rojoSuperficie = 0.0f;
        verdeSuperficie = 1.0f;
        azulSuperficie = 0.0f; // Verde
        break;
    case 3:
        rojoSuperficie = 0.0f;
        verdeSuperficie = 0.0f;
        azulSuperficie = 1.0f; // Azul
        break;
    }
    glutPostRedisplay();

}

void teclado(unsigned char tecla,int x,int y)
{
    switch(tecla)
    {
    case 'C' :
        incrementoA = incrementoA + 1;
        glutPostRedisplay();
        break;
    case 'X' :
        incrementoA = incrementoA - 1;
        glutPostRedisplay();
        break;
    case 'A' :
        nivelesSuperficie=nivelesSuperficie+1;
        glutPostRedisplay();
        break;
    case 'D' :
        if(nivelesSuperficie>0)
        {
            nivelesSuperficie=nivelesSuperficie-1;
        }
        glutPostRedisplay();
        break;
    }
}

void createMenu()
{
    int submenu_ingresar=glutCreateMenu(menu_ingresar);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);

    int submenu_poli=glutCreateMenu(menu_poli);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);

    int submenu_curva=glutCreateMenu(menu_curva);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);

    int submenu_moverpuntos=glutCreateMenu(menu_moverpuntos);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);

    int submenu_ejes=glutCreateMenu(menu_ejes);
    glutAddMenuEntry("Activado",1);
    glutAddMenuEntry("Desactivado",2);

    int submenu_color=glutCreateMenu(menu_color);
    glutAddMenuEntry("Rojo",1);
    glutAddMenuEntry("Verde",2);
    glutAddMenuEntry("Azul",3);

    int submenu_grosor=glutCreateMenu(menu_grosor);
    glutAddMenuEntry("Delgado",1);
    glutAddMenuEntry("Mediano",2);
    glutAddMenuEntry("Grueso",3);

    int submenu_superficie=glutCreateMenu(menu_superficie);
    glutAddMenuEntry("Rojo",1);
    glutAddMenuEntry("Verde",2);
    glutAddMenuEntry("Azul",3);

    int menu=glutCreateMenu(menu_principal);
    glutAddSubMenu("Ingresar puntos de control",submenu_ingresar);
    glutAddSubMenu("Mostrar polilinea",submenu_poli);
    glutAddSubMenu("Mostrar Curva B-Spline",submenu_curva);
    glutAddSubMenu("Mover puntos de control",submenu_moverpuntos);
    glutAddSubMenu("Mostrar ejes coordenados 2D",submenu_ejes);
    glutAddSubMenu("Cambiar color de la curva",submenu_color);
    glutAddSubMenu("Cambiar el grosor de la curva",submenu_grosor);
    glutAddSubMenu("Cambiar el color de superficie reglada",submenu_superficie);
    glutAddMenuEntry("Salir",9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(750, 500);
    glutCreateWindow("PC03 Alvarado Torres, Paz Guillen");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(clickeoParaPuntos);
    glutMotionFunc(movimientoDelMouse);
    glutKeyboardFunc(teclado);
    createMenu();
    glutMainLoop();
    return 0;
}
