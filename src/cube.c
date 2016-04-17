/*
* Name: Chase Counsell
* Project: COMP 5400 Project 4
* File: cube.c
* Date: 3/1/2016
* Description: Functions for drawing a cube, changing the color, and toggling bounding box.
*/

#include<GL\glut.h>
#include<math.h>
#include "cube.h"

typedef GLfloat point[3];

GLfloat vertices[][3] = { { -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },{ 0.5, 0.5, -0.5 },
						  { -0.5, 0.5, -0.5 },{ -0.5, -0.5, 0.5 },{ 0.5, -0.5, 0.5 },
						  { 0.5, 0.5, 0.5 },{ -0.5, 0.5, 0.5 } };

GLfloat normals[][3] = { { -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },{ 0.5, 0.5, -0.5 },
						 { -0.5, 0.5, -0.5 },{ -0.5, -0.5, 0.5 },{ 0.5, -0.5, 0.5 },
						 { 0.5, 0.5, 0.5 },{ -0.5, 0.5, 0.5 } };

GLfloat colors[][3] = { { 1.0, 0.80, 0.58 },{ 1.0, 0.0, 0.0 },{ 0.37, 0.15, 0.02 },
						{ 0.0, 0.5, 0.0 },{ 0.0, 0.0, 0.5 },{ 0.5, 0.5, 0.5 },
						{ 1.0, 1.0, 1.0 },{ 0.94, 0.43, 0.12 },{ 0.0, 0.0, 0.0 },
						{ 0.0, 0.7, 0.0 }};

enum color { skin, red, brown, green, blue, gray, white, orange, black, grass };
static int current_color = skin;
static int is_bounding_enabled = 0;
static GLfloat normal[3];
static const int r = 2;

void calculate_normal(point a, point b, point c)
{
	GLfloat v1[3] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
	GLfloat v2[3] = { c[0] - a[0], c[1] - a[1], c[2] - a[2] };
	GLfloat vx = v1[1] * v2[2] - v1[2] * v2[1];
	GLfloat vy = v1[2] * v2[0] - v1[0] * v2[2];
	GLfloat vz = v1[0] * v2[1] - v1[1] * v2[0];

	GLfloat l = sqrt(vx*vx + vy*vy + vz*vz);

	normal[0] = vx / l;
	normal[1] = vy / l;
	normal[2] = vz / l;
}

void triangle(point a, point b, point c)
{
	calculate_normal(a, b, c);
	glBegin(GL_POLYGON);
		glNormal3fv(normal);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	glEnd();
}

void divide_triangle(point a, point b, point c, int r)
{
	point v1, v2, v3;
	int j;

	if (r > 0)
	{
		for (j = 0; j < 3; j++) v1[j] = (a[j] + b[j])/2.0;
		for (j = 0; j < 3; j++) v2[j] = (a[j] + c[j])/2.0;
		for (j = 0; j < 3; j++) v3[j] = (b[j] + c[j])/2.0;

		divide_triangle(a, v1, v2, r - 1);
		divide_triangle(c, v2, v3, r - 1);
		divide_triangle(b, v3, v1, r - 1);
		divide_triangle(v1, v3, v2, r - 1);
	}
	else
	{
		triangle(a, b, c);
	}
}

void square(int r)
{
	glColor3fv(colors[current_color]);

	divide_triangle(vertices[4], vertices[5], vertices[7], r);
	divide_triangle(vertices[5], vertices[6], vertices[7], r);
}

void draw_cube()
{
	glPushMatrix();
	square(r);
	glRotatef(90, 0, 1, 0);
	square(r);
	glRotatef(90, 0, 1, 0);
	square(r);
	glRotatef(90, 0, 1, 0);
	square(r);
	glRotatef(90, 1, 0, 0);
	square(r);
	glRotatef(180, 1, 0, 0);
	square(r);
	glPopMatrix();
}

void cube_face(int a, int b, int c, int d)
{

	calculate_normal(vertices[a], vertices[b], vertices[d]);
	calculate_normal(vertices[b], vertices[c], vertices[a]);
	calculate_normal(vertices[c], vertices[d], vertices[b]);
	calculate_normal(vertices[d], vertices[a], vertices[c]);

	/* draw a polygon via list of vertices */
	glColor3fv(colors[current_color]);

	glBegin(GL_POLYGON);
	glNormal3fv(normal);
	glVertex3fv(vertices[a]);
	glNormal3fv(normal);
	glVertex3fv(vertices[b]);
	glNormal3fv(normal);
	glVertex3fv(vertices[c]);
	glNormal3fv(normal);
	glVertex3fv(vertices[d]);
	glEnd();
}

void cube(void)
{
	cube_face(0, 3, 2, 1);
	cube_face(3, 7, 6, 2);
	cube_face(0, 4, 7, 3);
	cube_face(1, 2, 6, 5);
	cube_face(4, 5, 6, 7);
	cube_face(0, 1, 5, 4);
}

void change_color(int x)
{
	switch (x)
	{
	case 0:
		current_color = skin;
		break;
	case 1:
		current_color = red;
		break;
	case 2:
		current_color = brown;
		break;
	case 3:
		current_color = green;
		break;
	case 4:
		current_color = blue;
		break;
	case 5:
		current_color = gray;
		break;
	case 6:
		current_color = white;
		break;
	case 7:
		current_color = orange;
		break;
	case 8:
		current_color = black;
		break;
	case 9:
		current_color = grass;
		break;
	default:
		current_color = skin;
		break;
	}
}

void toggle_bounds()
{
	is_bounding_enabled = !is_bounding_enabled;
}

int bounds_enabled()
{
	return is_bounding_enabled;
}