
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/framework.cpp"
// #include "../gradient.cpp"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))


GLuint theProgram;

void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "standard.vert"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "standard.frag"));

	theProgram = Framework::CreateProgram(shaderList);
}

const float vertexPositions[] = {
	0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,			
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
	1.25f, 0.25f, 0.0f, 1.0f,
	1.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f

};

const uint indices [] = {
	0, 1, 2,
	3, 4, 5
};

GLuint positionBufferObject;
GLuint vao;
GLuint ebo;

void InitializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();
	InitializeVertexBuffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void ComputePositionOffsets(float &fXOffset, float &fYOffset)
{
	const float fLoopDuration = 2.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
	fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

int getPosition(std::vector<float> &offsetdata, std::map<int, std::vector<std::string>> data)
{

	const float fLoopDuration = data.size();

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	float pos = fmodf(fElapsedTime*fLoopDuration/10, fLoopDuration);


	for (int i = 0; i < data.at(0).size(); i++){
		offsetdata.push_back(std::stof(data.at(pos).at(i)));
	}
	return pos;
}

// void AdjustVertexData(float fXOffset, float fYOffset)
// {
// 	std::vector<float> fNewData(ARRAY_COUNT(vertexPositions));
// 	memcpy(&fNewData[0], vertexPositions, sizeof(vertexPositions));
// 	// std::cout<<ARRAY_COUNT(vertexPositions)<<std::endl;
// 	for(int iVertex = 0; iVertex < ARRAY_COUNT(vertexPositions); iVertex += 4)
// 	{
// 		fNewData[iVertex] += 1;
// 		fNewData[iVertex + 1] += fYOffset;
// 	}
// 	// std::cout<<fNewData.at(2)<<std::endl;
// 	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
// 	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), &fNewData[0]);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// }

void AdjustVertexData(std::vector<float> offsetData)
{
	std::vector<float> fNewData(ARRAY_COUNT(vertexPositions));
	memcpy(&fNewData[0], vertexPositions, sizeof(vertexPositions));
	// std::cout<<ARRAY_COUNT(vertexPositions)<<std::endl;

	
	int num = offsetData.size()/2;
	std::cout<<num<<std::endl;
	for (int i = 0; i < num ; i ++ ){
		fNewData[i*4+0] = offsetData.at(i*2+0);
		fNewData[i*4+1] = offsetData.at(i*2+1);
	}

	std::cout<<fNewData[15]<<std::endl;
	
	// std::cout<<fNewData.at(2)<<std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), &fNewData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
	float fXOffset = 0.0f, fYOffset = 0.0f;
	float fX1 = 0.0f;
	float fX2 = 0.0f;
	float fX3 = 0.0f;
	float fY1 = 0.0f;
	float fY2 = 0.0f;
	float fY3 = 0.0f;
	std::vector<float> offsetdata;
	int pos = 0;
	ComputePositionOffsets(fXOffset, fYOffset);
	pos = getPosition(offsetdata, csv_contents);
	// std::cout<<pos<<std::endl;
	// std::cout<<offsetdata.at(0)<<std::endl;
	// std::cout<<fXOffset<<std::endl;
	// std::cout<<csv_contents.at(0).at(0)<<std::endl;
	AdjustVertexData(offsetdata);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	// glDrawArrays(GL_LINE_LOOP, 0, 3);
	// glDrawArrays(GL_LINE_LOOP, 3, 3);
	// glDrawArrays(GL_LINE_LOOP, 6, 3);
	// glDrawArrays(GL_LINE_LOOP, 9, 3);
	// glDrawArrays(GL_LINE_LOOP, 12, 3);
	// glDrawArrays(GL_LINE_LOOP, 15, 3);
	// glDrawArrays(GL_LINE_LOOP, 18, 3);
	for(int i = 0 ; i < 22 ; i++){
		glDrawArrays(GL_LINE_LOOP, i*3, 3);
	}

	// glDrawArrays(GL_TRIANGLES, 4, 3);
	// glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	  case 27:
		  glutLeaveMainLoop();
		  return;
	}
}


unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}

