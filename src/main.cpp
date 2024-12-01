#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <random>

const int W_WIDTH = 800;
const int W_HEIGHT = 800;

// const float X_MAX = 600;
// const float Y_MAX = 600;

struct Torus
{
	float R;
	float r;
	int numc;
	int numt;
	std::vector<float> spawpoint;
	std::vector<float> Anglerotacion = {0.0, 0.0, 0.0};
	std::vector<float> color = {1.0, 1.0, 1.0};
	int eje = 0;

	GLuint ID;
};

std::vector<Torus> TorusVector;

int toroideSeleccionado = -1;

void init(void);
void display(void);
void reshape(int w, int h);
void procesarSeleccion(int x, int y);
void mostrarMenu(int value);

void torus(Torus &);

void rotateTorus(int);
void CreateTorus();

void RenderTorus();

std::vector<float> CreateRandomSpawpoint();

// Variables globales para el estado de la cámara
float Position[3] = {0.0f, 0.0f, 40.0f};
float Orientation[3] = {0.0f, 0.0f, -1.0f};
float Up[3] = {0.0f, 1.0f, 0.0f};
float sensitivity = 0.001f;
bool isDragging = false;
int lastMouseX, lastMouseY;

static void torus(Torus &torus)
{
	int i, j, k;
	double s, t, x, y, z, twopi;
	twopi = 2 * M_PI;
	for (i = 0; i < torus.numc; i++)
	{
		glBegin(GL_LINE_LOOP);
		for (j = 0; j <= torus.numt; j++)
		{
			for (k = 1; k >= 0; k--)
			{
				s = (i + k) % torus.numc + 0.5;
				t = j % torus.numt;
				x = (torus.R + torus.r * cos(s * twopi / torus.numc)) * cos(t * twopi / torus.numt);
				y = (torus.R + torus.r * cos(s * twopi / torus.numc)) * sin(t * twopi / torus.numt);
				z = torus.r * sin(s * twopi / torus.numc);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
}

void RenderTorus()
{

	for (Torus torus : TorusVector)
	{
		glColor3fv(torus.color.data());

		// Configurar el nombre único del toroide
		glLoadName(torus.ID);

		glPushMatrix();

		glTranslatef(torus.spawpoint[0], torus.spawpoint[1], torus.spawpoint[2]);
		glRotatef(torus.Anglerotacion[0], 1.0f, 0.0f, 0.0f);
		glRotatef(torus.Anglerotacion[1], 0.0f, 1.0f, 0.0f);
		glRotatef(torus.Anglerotacion[2], 1.0f, 0.0f, 1.0f);
		glCallList(torus.ID);

		glPopMatrix();
	}
}

void CreateTorus()
{

	Torus torus1{2.0, 0.5, 20, 100};
	torus1.spawpoint = CreateRandomSpawpoint();
	torus1.Anglerotacion = {0.0, 0.0, 0.0};
	torus1.eje = 0;

	torus1.ID = glGenLists(1);

	glNewList(torus1.ID, GL_COMPILE);
	torus(torus1);
	glEndList();

	Torus torus2{2.0, 0.5, 20, 100};
	torus2.spawpoint = CreateRandomSpawpoint();
	torus2.Anglerotacion = {0.0, 0.0, 0.0};
	torus2.eje = 0;

	torus2.ID = glGenLists(1);

	glNewList(torus2.ID, GL_COMPILE);
	torus(torus2);
	glEndList();

	TorusVector.emplace_back(torus1);
	TorusVector.emplace_back(torus2);

	std::cout << "Torus created 1 ID : " << torus1.ID << std::endl;
	std::cout << "Torus created 2 ID : " << torus2.ID << std::endl;
}

void rotateTorus(int value)
{

	for (Torus &torus : TorusVector)
	{
		if (torus.eje == 0)
		{
			torus.Anglerotacion[0] += 0.5;
			if (torus.Anglerotacion[0] > 360.0)
			{
				torus.Anglerotacion[0] -= 360.0;
			}
		}
		else if (torus.eje == 1)
		{
			torus.Anglerotacion[1] += 0.5;
			if (torus.Anglerotacion[1] > 360.0)
			{
				torus.Anglerotacion[1] -= 360.0;
			}
		}
		else if (torus.eje == 2)
		{
			torus.Anglerotacion[2] += 0.5;
			if (torus.Anglerotacion[2] > 360.0)
			{
				torus.Anglerotacion[2] -= 360.0;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, rotateTorus, 0);
}

std::vector<float> CreateRandomSpawpoint()
{
	std::vector<float> spawpoint;
	// Crear un generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 20); // Cambia el rango según sea necesario

	spawpoint.push_back(dis(gen));
	spawpoint.push_back(dis(gen));
	spawpoint.push_back(dis(gen));

	return spawpoint;
}
// Función para normalizar un vector
void normalize(float *v)
{
	float magnitude = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (magnitude > 0.0f)
	{
		v[0] /= magnitude;
		v[1] /= magnitude;
		v[2] /= magnitude;
	}
}

// Función para calcular el producto cruzado de dos vectores
void cross(const float *a, const float *b, float *result)
{
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
}
void submenuCambiarEje(int value)
{
	std::cout << "Cambiando eje de rotación... valor " << value << "\n";
	if (toroideSeleccionado >= 0)
	{
		auto &findtorus = *std::find_if(TorusVector.begin(), TorusVector.end(),
										[](const Torus &t)
										{ return t.ID == toroideSeleccionado; });

		findtorus.eje = value;
	}
}
void subMenuColor(int value)
{
	if (toroideSeleccionado >= 0)
	{
		auto &findtorus = *std::find_if(TorusVector.begin(), TorusVector.end(),
										[](const Torus &t)
										{ return t.ID == toroideSeleccionado; });

		switch (value)
		{
		case 1:
			findtorus.color = {0.0, 0.5, 0.4};
			break;
		case 2:
			findtorus.color = {0.5, 0.1, 0.2};
			break;
		case 3:
			findtorus.color = {0.2, 0.6, 0.7};
			break;
		}
	}
}
void mostrarMenu(int opcion)
{
	if (toroideSeleccionado >= 0)
	{
		// Encontrar el toroide seleccionado
		auto &findtorus = *std::find_if(TorusVector.begin(), TorusVector.end(),
										[](const Torus &t)
										{ return t.ID == toroideSeleccionado; });

		switch (opcion)
		{

		case 3:
			TorusVector.erase(std::remove_if(TorusVector.begin(), TorusVector.end(),
											 [selectedID = toroideSeleccionado](const Torus &t)
											 { return t.ID == toroideSeleccionado; }),
							  TorusVector.end());
			toroideSeleccionado = -1;
			break;
		case 4:
			Torus newtorus{2.0, 0.5, 100, 100};
			newtorus.spawpoint = CreateRandomSpawpoint();
			newtorus.Anglerotacion = {0.0, 0.0, 0.0};
			newtorus.color = {1.0, 1.0, 1.0};
			newtorus.eje = 0;
			newtorus.ID = glGenLists(1);
			std::cout << "Creating new torus ID: " << newtorus.ID << std::endl;

			glNewList(newtorus.ID, GL_COMPILE);
			torus(newtorus);
			glEndList();

			TorusVector.emplace_back(newtorus);

			std::cout << "Torus created ID : " << newtorus.ID << std::endl;
			break;
		}
	}
	else
	{
		switch (opcion)
		{
		case 4:
			Torus newtorus{2.0, 0.5, 20, 100};
			newtorus.spawpoint = CreateRandomSpawpoint();
			newtorus.Anglerotacion = {0.0, 0.0, 0.0};

			newtorus.ID = glGenLists(1);
			std::cout << "Creating new torus ID: " << newtorus.ID << std::endl;

			glNewList(newtorus.ID, GL_COMPILE);
			torus(newtorus);
			glEndList();

			TorusVector.emplace_back(newtorus);

			std::cout << "Torus created ID : " << newtorus.ID << std::endl;
			break;
		}
	}

	glutPostRedisplay();
}

// Función para manejar clics del mouse
void handleMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			isDragging = true;
			lastMouseX = x;
			lastMouseY = y;
		}
		else if (state == GLUT_UP)
		{
			isDragging = false;
		}
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{

		procesarSeleccion(x, y); // Realizar picking dentro del frustum de la cámara

		if (toroideSeleccionado >= 0)
		{
			std::cout << "Toroide seleccionado ID: " << toroideSeleccionado << "\n";

			int menuEJE = glutCreateMenu(submenuCambiarEje);
			glutAddMenuEntry("Eje x", 0);
			glutAddMenuEntry("Eje y", 1);
			glutAddMenuEntry("Eje z", 2);

			int colorMenu = glutCreateMenu(subMenuColor);
			glutAddMenuEntry("1", 1);
			glutAddMenuEntry("2", 2);
			glutAddMenuEntry("3", 3);

			int menu = glutCreateMenu(mostrarMenu);
			glutAddSubMenu("Cambiar eje de rotación", menuEJE);
			glutAddSubMenu("Cambiar color", colorMenu);
			glutAddMenuEntry("Eliminar", 3);
			glutAddMenuEntry("Crear Toroide", 4);

			glutSetMenu(menu);
			glutAttachMenu(GLUT_RIGHT_BUTTON);
		}
		else
		{
			std::cout << "No se seleccionó ningún toroide.\n";
		}
	}
}

// Función para manejar el movimiento del mouse mientras se arrastra
void handleMouseDrag(int x, int y)
{
	if (isDragging)
	{
		int deltaX = x - lastMouseX;
		int deltaY = y - lastMouseY;

		// Actualizar las posiciones ultima del mouse

		lastMouseX = x;
		lastMouseY = y;

		float rotX = sensitivity * deltaY;
		float rotY = sensitivity * -deltaX;

		// Rotación en el eje Y (alrededor del vector Up)
		float right[3];
		cross(Orientation, Up, right);
		normalize(right);

		Orientation[0] += rotY * right[0];
		Orientation[1] += rotY * right[1];
		Orientation[2] += rotY * right[2];
		normalize(Orientation);

		// Rotación en el eje X (alrededor del vector Right)
		float up[3];
		cross(right, Orientation, up);
		normalize(up);

		Orientation[0] += rotX * up[0];
		Orientation[1] += rotX * up[1];
		Orientation[2] += rotX * up[2];
		normalize(Orientation);
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	float speed = 0.1f; // Velocidad de movimiento

	switch (key)
	{
	case 'c' | 'C': // Reiniciar vista
		glMatrixMode(GL_MODELVIEW);

		Orientation[0] = 0.0f;
		Orientation[1] = 0.0f;
		Orientation[2] = -1.0f;

		float centerX = Position[0] + Orientation[0];
		float centerY = Position[1] + Orientation[1];
		float centerZ = Position[2] + Orientation[2];

		gluLookAt(Position[0], Position[1], Position[2], // Posición de la cámara
				  centerX, centerY, centerZ,			 // Punto al que mira la cámara
				  Up[0], Up[1], Up[2]);

		break;
	}

	glutPostRedisplay(); // Actualiza la pantalla
}

void UpdateCamera()
{

	float centerX = Position[0] + Orientation[0];
	float centerY = Position[1] + Orientation[1];
	float centerZ = Position[2] + Orientation[2];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Position[0], Position[1], Position[2], // Posición de la cámara
			  centerX, centerY, centerZ,			 // Punto al que mira la cámara
			  Up[0], Up[1], Up[2]);					 // Vector "arriba"
}

// --------- FUNCIONES DEFAULT

void procesarSeleccion(int x, int y)
{
	std::cout << "Procesando selección...\n";

	GLuint selectBuf[64]; // Buffer para almacenar objetos seleccionados
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(64, selectBuf); // Configurar buffer de selección
	glRenderMode(GL_SELECT);

	glInitNames(); // Inicializa la pila de nombres
	glPushName(0); // Nombre inicial en la pila

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Ajusta la matriz de proyección al área del cursor
	gluPickMatrix(x, viewport[3] - y, 5.0, 5.0, viewport);
	gluPerspective(45.0, (float)viewport[2] / viewport[3], 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);

	// Renderizar en modo selección
	RenderTorus();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Procesar resultados del picking
	GLint hits = glRenderMode(GL_RENDER);
	std::cout << "Hits: " << hits << "\n";
	if (hits > 0)
	{
		GLuint minZ = selectBuf[1];
		GLuint selectedID = selectBuf[3];
		for (int i = 1; i < hits; i++)
		{
			if (selectBuf[i * 4 + 1] < minZ)
			{
				minZ = selectBuf[i * 4 + 1];
				selectedID = selectBuf[i * 4 + 3];
			}
		}
		// Guardar el toroide seleccionado
		toroideSeleccionado = selectedID;
		std::cout << "Toroide seleccionado ID: " << selectedID << "\n";
	}
	else
	{
		std::cout << "Ningún toroide seleccionado\n";
		toroideSeleccionado = -1; // Ningún objeto seleccionado
	}
}

// Menú jerárquico

void display(void)
{

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateCamera();

	RenderTorus();

	glutSwapBuffers();

	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("a");
	init();

	glutDisplayFunc([]
					{ display(); });

	glutMouseFunc(handleMouseClick);
	glutMotionFunc(handleMouseDrag);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glutTimerFunc(10, rotateTorus, 0);

	glutMainLoop();
	return 0;
}

void init(void)
{
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	glShadeModel(GL_SMOOTH);

	CreateTorus();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}