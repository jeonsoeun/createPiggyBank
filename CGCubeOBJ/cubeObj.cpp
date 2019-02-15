#pragma once
#include"OBJ.h"

Object cube, piggy;

GLfloat g_screenW, g_screenH;
GLuint  g_matLoc;
GLuint g_programID;
GLuint g_colorID;

GLfloat transMat[16];
glm::mat4 rot;
bool rotateMode;
bool projectMode;
int shapeMode;//1:큐브 , 2: 돼지.

float angle;
float angle2;
float camCenterZ;
float camCenterX;
float camCenterY;
float camPan;
float camTilt;

bool shadowMode = false;

int groud = 0;

GLfloat lightPos[3] = { 7.0f, 6.0f, 7.0f };

int ikey = 0;
void cubeTextureMapping(Object& object);

GLuint planeVAO;
GLuint planeVBO[3];
GLfloat planeVtx[24] = {
	10.0f, -3.9f, 10.0f, 0.5f,1.0f,0.5f,
	10.0f, -3.9f, -10.0f, 0.5f, 1.0f, 0.5f,
	-10.0f, -3.9f, -10.0f, 0.5f, 1.0f, 0.5f,
	-10.0f, -3.9f, 10.0f, 0.5f, 1.0f, 0.5f };
//GLfloat planeVtx[36] = {
//	10.0f, -3.9f, 10.0f, 0.5f, 1.0f, 0.5f,
//	10.0f, -3.9f, -10.0f, 0.5f, 1.0f, 0.5f,
//	-10.0f, -3.9f, -10.0f, 0.5f, 1.0f, 0.5f,
//	10.0f, -3.9f, 10.0f, 1.0f, 1.0f, 0.5f,
//	-10.0f, -3.9f, -10.0f, 0.5f, 1.0f, 0.5f,
//	-10.0f, -3.9f, 10.0f, 0.5f, 1.0f, 0.5f };
GLuint planeIndex[6] = { 0, 1, 2, 0, 2, 3 };

GLfloat planeVtx2[36] = {
	10.0f, 10.0f, -0.5f, 1.0f, 0.0f, 0.0f,
	-10.0f, 10.0f, -0.5f, 1.0f, 0.0f, 0.0f,
	-10.0f, -10.0f, - 0.5f, 1.0f, 0.0f, 0.0f,
	10.0f, 10.0f,  - 0.5f, 1.0f, 0.0f, 0.0f,
	-10.0f, -10.0f, - 0.5f, 1.0f, 0.0f, 0.0f,
	10.0f, -10.0f, -0.5f, 1.0f, 0.0f, 0.0f };
GLuint plane2Buff;
GLuint planeArr;

GLfloat TriangeVtx[18] = {
	10.0f, 3.0f, -10.0f, 1.0f, 0.0f, 0.0f,
	-10.0f, 3.0f, 10.0f, 1.0f, 1.0f, 0.0f,
	10.0f, 3.0f, 10.0f, 1.0f, 0.0f, 1.0f };
GLfloat TriIndex[3] = {0,1,2};
GLuint TriBuff[2], TriArr;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> ProgramErrorMessage(InfoLogLength);
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void renderScene(void)
{ 
	//Clear all pixels
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//Let's draw something here
	glm::mat4 projMat;
	if (projectMode)
		projMat = glm::perspective(45.0f, 70.0f / 48.0f, 0.1f, 100.0f);
	else
		projMat = glm::ortho(1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 100.0f);

	glm::mat4 viewMat = glm::mat4(1.0f);
	viewMat[3] = glm::vec4(-camCenterX, -camCenterY, -camCenterZ, 1.0f);
	glm::mat4 panMat = glm::rotate(-camPan, glm::vec3(0, 1, 0));
	glm::mat4 tiltMat = glm::rotate(-camTilt, glm::vec3(0, 0, 1));
	viewMat = tiltMat*panMat*viewMat;

	GLuint viewLoc = glGetUniformLocation(g_programID, "viewMat");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMat[0][0]);
	GLuint projLoc = glGetUniformLocation(g_programID, "projMat");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMat[0][0]);
	glm::mat4 worldMat = glm::mat4(0.1f);
	glm::mat4 rotMat = glm::rotate(angle, glm::vec3(0, 1, 0));
	glm::mat4 rotMat2 = glm::rotate(angle2, glm::vec3(1, 0, 0));
	glm::mat4 total_rotMat = rotMat*rotMat2;
	worldMat = total_rotMat*worldMat;

	GLuint matLoc = glGetUniformLocation(g_programID, "tranMat");
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &worldMat[0][0]);

	//노말 행렬을 정한다.
	GLuint normMatLoc = glGetUniformLocation(g_programID, "normMat");
	glm::mat4 normMat = viewMat*total_rotMat;
	normMat = glm::inverse(normMat);
	normMat = glm::transpose(normMat);
	glUniformMatrix4fv(normMatLoc, 1, GL_FALSE, &normMat[0][0]);

	//광원 위치랑 색을 넘겨준다. 
	GLuint lightPosLoc = glGetUniformLocation(g_programID, "lightPosW");
	GLuint lightColorLoc = glGetUniformLocation(g_programID, "lightCol");
	GLuint shinyFactorLoc = glGetUniformLocation(g_programID, "shinyN");
	GLuint ambientColorLoc = glGetUniformLocation(g_programID, "ambientColor");
	glUniform3f(lightPosLoc, lightPos[0], lightPos[1], lightPos[2]);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform1f(shinyFactorLoc, 30.0f);

	GLuint modeLoc = glGetUniformLocation(g_programID, "mode");
	glUniform1i(modeLoc, shapeMode);

	GLuint isTextureLoc = glGetUniformLocation(g_programID, "isTex");

	GLuint isShadowVLoc = glGetUniformLocation(g_programID, "isShadowV");
	glUniform1i(isShadowVLoc, 0);

	GLuint isGraudLoc = glGetUniformLocation(g_programID, "isGraud");
	glUniform1i(isGraudLoc, groud);

	//바닥
	//glBindVertexArray(planeVAO);
	//glUniform1i(isTextureLoc, 0);
	//glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
	//glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
	////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	if(shapeMode == 1)
	{
		//큐브의 텍스쳐를 내가 따로 만들거다.
		//cubeTextureMapping(cube);
		//스텐실만들기.
		
		glStencilFunc(GL_ALWAYS, 0, 255);
		//바닥그리고
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glEnableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//원래 색으로 그리고
		glBindVertexArray(cube.VAO[1]);
		glDepthMask(GL_FALSE);//depth writting안하는거
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);//스텐실0,뎁스0일때만 빼야지.
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
		glDrawElements(GL_TRIANGLES, cube.shadowV_index.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));
		
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glStencilFunc(GL_EQUAL, 0, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		GLuint myTexLoc = glGetUniformLocation(g_programID, "myTex");
		glUniform1i(myTexLoc, 1);
		glBindVertexArray(cube.VAO[0]);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.0, 0.5, 0.9);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform1i(isTextureLoc, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cube.TEX[0]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glEnableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//그림자 그리기
		glStencilFunc(GL_NOTEQUAL, 0, 255);
		glUniform1i(isTextureLoc, 0);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		glBindVertexArray(cube.VAO[0]);
		glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//면
		glBindVertexArray(planeArr);
		glUniform1i(isTextureLoc, 0);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 1.0, 1.0, 0.0);
		//glDrawArrays(GL_TRIANGLES, 0, 6); 

		if (shadowMode){
			glStencilFunc(GL_ALWAYS, 0, 0xff);
			glBindVertexArray(cube.VAO[1]);
			glVertexAttrib3f(g_colorID, 0.5, 0.5, 0.9);
			glUniform3f(ambientColorLoc, 0.3f, 0.3f, 0.3f);
			glUniform1i(isShadowVLoc, 1);
			 
			//선으로 모양보여주기
			glPolygonMode(GL_FRONT, GL_LINE);
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			//조금씩그려보기.
			glDrawElements(GL_TRIANGLES, ikey, GL_UNSIGNED_INT, ((GLvoid*)(0)));
			//glPolygonMode(GL_FRONT, GL_LINE);
			//glDrawElements(GL_TRIANGLES, cube.shadowV_index.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));
		}
		//glDrawElements(GL_TRIANGLES, ikey, GL_UNSIGNED_INT, ((GLvoid*)(0)));
	}
	else if (shapeMode == 2)
	{
		////스텐실만들기.
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0, 255);
		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glEnableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//쉐도우볼륨 스탠실
		glBindVertexArray(piggy.VAO[1]);
		glDepthMask(GL_FALSE);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
		glDrawElements(GL_TRIANGLES, piggy.shadowV_index.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));
		//glDrawElements(GL_TRIANGLES, 99, GL_UNSIGNED_INT, (GLuint*)sizeof(GLuint)*51);//몇개만 그려보자

		//원래 부분
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 0, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		////돼지그리기
		GLuint myTexLoc = glGetUniformLocation(g_programID, "myTex");
		glUniform1i(myTexLoc, 0);
		glUniform1i(isTextureLoc, 1);
		glBindVertexArray(piggy.VAO[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, piggy.TEX[0]);
		//glDrawElements(GL_TRIANGLES, ikey, GL_UNSIGNED_INT, ((GLvoid*)(0)));
		glDrawElements(GL_TRIANGLES, piggy.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥그리기
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glEnableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//그림자 부분
		glStencilFunc(GL_NOTEQUAL, 0, 255);
		glUniform1i(isTextureLoc, 0);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		glBindVertexArray(piggy.VAO[0]);
		glDrawElements(GL_TRIANGLES, piggy.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));

		if (shadowMode){
			glStencilFunc(GL_ALWAYS, 0, 0xff);
			glUniform1i(isTextureLoc, 0);
			glBindVertexArray(piggy.VAO[1]);
			glVertexAttrib3f(g_colorID, 0.0, 0.5, 0.9);
			glUniform3f(ambientColorLoc, 0.3f, 0.3f, 0.3f);
			glUniform1i(isShadowVLoc, 1);
			glPolygonMode(GL_FRONT, GL_LINE);
			//glDrawElements(GL_TRIANGLES, piggy.shadowV_index.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));
			glDrawElements(GL_TRIANGLES, ikey, GL_UNSIGNED_INT, ((GLvoid*)(0)));	
		}
	}
	else if (shapeMode == 0){
		glBindVertexArray(TriArr);
		glStencilFunc(GL_ALWAYS, 0, 255);
		glStencilOpSeparate(GL_FRONT, GL_INCR_WRAP, GL_INCR_WRAP, GL_INCR_WRAP);
		glStencilOpSeparate(GL_BACK, GL_DECR_WRAP, GL_DECR_WRAP, GL_DECR_WRAP);
		glDepthFunc(GL_NEVER);
		///glDrawElements(GL_TRIANGLES, cube.shadowV_index.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		glDepthFunc(GL_LESS);
		glStencilFunc(GL_EQUAL, 0, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		GLuint myTexLoc = glGetUniformLocation(g_programID, "myTex");
		glUniform1i(myTexLoc, 1);
		glBindVertexArray(cube.VAO[0]);
		glVertexAttrib3f(g_colorID, 0.0, 0.5, 0.9);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform1i(isTextureLoc, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cube.TEX[0]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glVertexAttrib3f(g_colorID, 0.5, 0.9, 0.5);
		glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//이거때문에 안그려지는거였어 ㅠㅠㅠㅠ근대 왱애얼애ㅓ래너란ㅇ 까맣게 칠햇는데 왜.. 없어짐..
		glStencilFunc(GL_NOTEQUAL, 0, 255);
		glUniform1i(isTextureLoc, 0);
		glDisableVertexAttribArray(g_colorID);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		glBindVertexArray(cube.VAO[0]);
		glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, ((GLvoid*)(0)));

		//바닥
		glBindVertexArray(planeVAO);
		glUniform1i(isTextureLoc, 0);
		glVertexAttrib3f(g_colorID, 0.0, 0.0, 0.0);
		//glUniform3f(ambientColorLoc, 1.0f, 1.0f, 1.0f);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*)(0)));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////면
		//glBindVertexArray(planeArr);
		//glUniform1i(isTextureLoc, 0);
		////glDisableVertexAttribArray(g_colorID);
		////glVertexAttrib3f(g_colorID, 1.0, 0.0, 0.0);
		//glDrawArrays(GL_TRIANGLES, 0, 6); 

		//glDrawElements(GL_TRIANGLES, ikey, GL_UNSIGNED_INT, ((GLvoid*)(0)));
	}
	
	glutSwapBuffers();
}
//초기화. 
void init()
{
	//initilize the glew and check the errors.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
	}

	//select the background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	angle = 0.0;
	angle2 = 0.0;
	projectMode = true;
	camCenterZ = 10;
	camPan = 0.0;
	camTilt = 0.0;

	shapeMode = 1;

	//FreeImage_Initialise(); //freeImage 사용을 위한 초기화.
}
void myMouse(int button, int state, int x, int y)
{
	float fx, fy;
	fx = (x / (float)480) * 2  ;
	fy = (y / (float)480) * 2  ;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		shadowMode = !shadowMode;
	}
	glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		angle += 0.1;
	}
	else if (key == 's')
		angle2 += 0.1;
	else if (key == 'q')
		camCenterZ += 0.1;
	else if (key == 'w')
		camCenterZ -= 0.1;
	else if (key == 'e')
		camCenterX += 0.1;
	else if (key == 'r')
		camCenterX -= 0.1;
	else if (key == 't')
		camCenterY += 0.1;
	else if (key == 'y')
		camCenterY -= 0.1;

	else if (key == 'z')
		camPan += 0.1;
	else if (key == 'x')
		camPan -= 0.1;

	else if (key == 'c')
		camTilt += 0.1;
	else if (key == 'v')
		camTilt -= 0.1;

	else if (key == 'm')
		projectMode != projectMode;
	else if (key == '1')
		shapeMode = 1;
	else if (key == '2')
		shapeMode = 2;

	else if (key == '3'){
		if (groud == 0)
			groud = 1;
		else
			groud = 0;
	}

	else if (key == 'i'){
		if (ikey <piggy.indices.size())
		ikey+=3;
	}

	glutPostRedisplay();
} 

int makeIndexForFaceInfo(vector<string>faceIndex, vector<BST>index_tree, vector<GLuint>& p_index, int faceVtxNum)
{
	int num = faceVtxNum;

	for (int i = 0; i < faceIndex.size() / num; i++)
	{
		vector<GLuint>temp_index;
		int key = 0;
		for (int j = 0; j < num; j++)
		{
			if (searchTree(index_tree, faceIndex[i*num+j], key))
			{
				temp_index.push_back(key);
			}
		}
		for (int j = 1; j <= num - 2; j++)
		{
			p_index.push_back(temp_index[0]);
			p_index.push_back(temp_index[j]);
			p_index.push_back(temp_index[j + 1]);
		}
	}
	return faceVtxNum;
}
//mtl 파일 내용을 받는 함수. 아직 안씀.
int useMtl(string filename, Object object)
{
	ifstream mtlOpen(filename);
	while (!mtlOpen.eof())
	{
		char line[MAXNUM];
		mtlOpen.getline(line, MAXNUM);
		char* context;
		char* first = strtok_s(line, " ", &context);
		int groupNum = 0;
		if (first == NULL)
		{
			continue;
		}
		if (!strncmp(first, "newmtl", 6))
		{
			for (int i = 0; i < object.group.size(); i++)
			{
				if (!strncmp(context, object.group[i].matarial.data(), MAXNUM))
				{
					groupNum = i;
				}
			}
		}
		if (!strncmp(first, "Ka", 2))
		{
			//엠비언트 색.
			int n=0;
			for (;;)
			{
				char* temp = strtok_s(context, " ", &context);
				if (temp != NULL)
				{
					float a = atof(temp);
					object.group[groupNum].ambient[n++] = a;
				}
				else
					break;
			}
		}
		else if (!strncmp(first, "Kd", 2))
		{
			//디퓨즈색
			int n = 0;
			for (;;)
			{
				char* temp = strtok_s(context, " ", &context);
				if (temp != NULL)
				{
					float a = atof(temp);
					object.group[groupNum].diffuse[n++] = a;
				}
				else
					break;
			}
		}
		else if (!strncmp(first, "Ks", 2))
		{
			//스펙큘러색
			int n = 0;
			for (;;)
			{
				char* temp = strtok_s(context, " ", &context);
				if (temp != NULL)
				{
					float a = atof(temp);
					object.group[groupNum].specular[n++] = a;
				}
				else
					break;
			}
		}
		else if (!strncmp(first, "Ns", 2))
		{
			float a = atof(context);
			object.group[groupNum].Ns = a;
		}
	}
	return 0;
}

//둘다 되는걸 만들자..
int getEveryVtx(string fileName, vector<GLfloat>& p_vtx, vector<GLfloat>& p_vt, vector<GLuint>& p_index, vector<GLfloat>& p_normal, vector<BST>& index_tree, Object& object)
{
	ifstream fileObjOpen(fileName);
	int indexNum = 0;
	vector<string> faceInfo;//'f'뒤에 쓰여진 정보를 그냥 순서대로 저장.
	int faceVtxNumver=0; // 한면의 점의 개수.
	int faceNum = 0;//면의 개수.
	string mtlFileName;

	vector<GLfloat> forBufferVtx;
	vector<GLuint> forBufferInd;
	GLuint total_index=0;

	//쉐도우 볼륨 light position
	object.shadowVolume.push_back(lightPos[0]);
	object.shadowVolume.push_back(lightPos[1]);
	object.shadowVolume.push_back(lightPos[2]);
	//빛-점 가장긴 점의 위치.
	float MaxLightToPos[3];
	float MaxLightToPosLen=0;
	//점 몇개째인지.
	//※쉐도우 볼륨 해결책. 한 사각형 면에서 빛(L)-0-1 | L-1-2 |  L-2-3 | 3-L-0 의 점을 이용해서 삼각형을 만든다.

	while (!fileObjOpen.eof())
	{
		char line[MAXNUM];
		fileObjOpen.getline(line, MAXNUM);
		char* context;
		char* first = strtok_s(line, " ", &context);
		int vtxCount = 0;
		int groupNum = 0;
		if (first == NULL)
		{
			continue;
		}
		//버텍스
		if (!strncmp(first, "vt", 2))
		{
			for (;;)
			{
				char* temp = strtok_s(context, " ", &context);
				if (temp != NULL)
				{
					float vtx = atof(temp);
					p_vt.push_back(vtx);
				}
				else
					break;
			}
		}
		//텍스쳐 point
		else if (!strncmp(first, "v", 1))
		{
			vtxCount++;
			for (;;)
			{
				char* temp = strtok_s(context, " ", &context);
				if (temp != NULL)
				{
					float vtx = atof(temp);
					p_vtx.push_back(vtx);
				}
				else
					break;
			}
		}
		//인덱스 써있는 부분.. 버텍스/텍스쳐/노말.  
		else if (!strncmp(first, "f", 1))
		{
			int number = 0;
			vector<GLuint> temp_index;
			vector<GLfloat> forNormal;
			GLfloat vtx_x = 0.0;
			GLfloat vtx_y = 0.0;
			GLfloat vtx_z = 0.0;
			for (;;)
			{
				char* copy = strtok_s(context, " ", &context);
				if (copy == NULL)
				{
					break;
				}
				else
				{
					faceInfo.push_back(string(copy));
					number++;
					int findIndex = 0; //search 트리로 찾은거 insert가 됬건 안됬건 그걸 넣을것.
					GLuint temp_total_index = total_index;  
					bool insert_result = insertTree(index_tree, string(copy), findIndex, temp_total_index);
					 vtx_x = p_vtx[(index_tree[findIndex].vtx_index - 1) * 3];
					 vtx_y = p_vtx[(index_tree[findIndex].vtx_index - 1) * 3+1];
					 vtx_z = p_vtx[(index_tree[findIndex].vtx_index - 1) * 3+2];

					//노말을 위해 미리 저장해둠.
					if (!object.use_nor)
					{
						forNormal.push_back(vtx_x);
						forNormal.push_back(vtx_y);
						forNormal.push_back(vtx_z);
					}

					if (insert_result)
					{
						forBufferVtx.push_back(vtx_x);
						forBufferVtx.push_back(vtx_y);
						forBufferVtx.push_back(vtx_z);
						//쉐도우 볼륨 vtx
						GLfloat light_to_point[3] = { vtx_x - lightPos[0], vtx_y - lightPos[1], vtx_z - lightPos[2] };
						float lightToPosLenth = pow(vtx_x - lightPos[0], 2) + pow(vtx_y - lightPos[1], 2) + pow(vtx_z - lightPos[2], 2);
						//MaxLightToPosLen = pow(MaxLightToPos[0] - lightPos[0], 2) + pow(MaxLightToPos[1] - lightPos[1], 2) + pow(MaxLightToPos[2] - lightPos[2], 2);
						if (lightToPosLenth>MaxLightToPosLen){
							MaxLightToPosLen = lightToPosLenth;
						}
						object.shadowVolume.push_back(vtx_x);
						object.shadowVolume.push_back(vtx_y);
						object.shadowVolume.push_back(vtx_z);
						if (p_vt.size() != 0)
						{
							forBufferVtx.push_back(p_vt[(index_tree[findIndex].tex_index - 1) * 2]);
							forBufferVtx.push_back(p_vt[(index_tree[findIndex].tex_index - 1) * 2 + 1]);
						}
						else
						{
							forBufferVtx.push_back(-1);
							forBufferVtx.push_back(-1);
						}
						temp_index.push_back(temp_total_index);
						total_index++;
					}
					else
					{
						temp_index.push_back(temp_total_index);
					}
				}
			}
			//obj에 몇각형으로 나와있던 삼각형을 기준으로 하는 최종인덱스를 갖는다.
			//여기서 면 평균 노말을 구해서 더한걸 넣는다. 전체에서 평균계산은 어쩔까? >>계산 할 필요 없이 더해놓고 나중에 노말라이즈.
			faceVtxNumver = number;
			glm::vec3 faceNor;
			for (int i = 0; i < faceVtxNumver-2; i++)
			{
				//면단위의 인덱스를 삼각형 단위로 임시저장.
				int temp_triangle_index[3] = {0,0,0};
				temp_triangle_index[0] = temp_index[0];
				temp_triangle_index[1] = temp_index[i+1];
				temp_triangle_index[2] = temp_index[i+2];
				
				//인덱스를 넣는다. 해당 점이 처음 발견된 순서대로 번호를 매긴것을 인덱스로쓴다. temp_index에는 한줄에 쓰여있는 점 정보의 인덱스 값이다.
				p_index.push_back(temp_triangle_index[0]);
				p_index.push_back(temp_triangle_index[1]);
				p_index.push_back(temp_triangle_index[2]);

				//외적으로 면 노말을 구한다.
				glm::vec3 o = glm::vec3(forNormal[0], forNormal[1], forNormal[2]);
				glm::vec3 a = glm::vec3(forNormal[3 * (i + 1)], forNormal[3 * (i + 1) + 1], forNormal[3 * (i + 1) + 2]);
				glm::vec3 b = glm::vec3(forNormal[3 * (i + 2)], forNormal[3 * (i + 2) + 1], forNormal[3 * (i + 2) + 2]);
				glm::vec3 oa = a - o;
				glm::vec3 ob = b - o;
				glm::vec3 after_cross = glm::cross(oa, ob);
				faceNor = glm::normalize(after_cross);

				for (int k = 0; k < 3; k++)
				{
					//노말을 저장한다.
					if (p_normal.size() / 3 > temp_triangle_index[k])//이미 그 점의 노말이 하나이상 더해졌을때
					{
						p_normal[temp_triangle_index[k] * 3] += faceNor.x;
						p_normal[temp_triangle_index[k] * 3 + 1] += faceNor.y;
						p_normal[temp_triangle_index[k] * 3 + 2] += faceNor.z;
					}
					else//처음 나타나쓸때
					{
						p_normal.push_back(faceNor.x);
						p_normal.push_back(faceNor.y);
						p_normal.push_back(faceNor.z);
					}
				}
			}
			//쉐도우 볼륨 index L-0-1, L-1-2, L-2-3, L-3-0 (L은 빛의 위치)
			//광원->면 벡터와 면의 노말벡터를 비교해서 알맞게 방향 설정.
			//광원->면 벡터랑 면의 노말벡터 내적이 +면 방향이 같은것 : 면은 앞면 | -면 방향이 다른것 : 면은 뒷면. 
			//앞면이면 L-1-0, 뒷면이면 L-0-1 --->지금 해야될것. 
			glm::vec3 vecLight = glm::vec3(lightPos[0],lightPos[1],lightPos[2]);
			glm::vec3 vecPoint = glm::vec3(vtx_x, vtx_y, vtx_z);  
			glm::vec3 LtoP = vecPoint - vecLight;
			if (glm::dot(LtoP, faceNor)>=0){
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(temp_index[3] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[3] + 1);
				object.shadowV_index.push_back(temp_index[0] + 1);

				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[3] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
			}
			else{
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[3] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(0);
				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[3] + 1);

				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[1] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(temp_index[0] + 1);
				object.shadowV_index.push_back(temp_index[2] + 1);
				object.shadowV_index.push_back(temp_index[3] + 1);
			}
			faceNum++;
		}
		//mtl파일이름 얻는것.
		else if (!strncmp(first, "mtllib", 6))

		{
			mtlFileName = string(context);
		}
		//mtl파일읽어서 쓰려했는데...완성못함.
		else if (!strncmp(first, "usemtl", 6) || !strncmp(first, "s", 1)) 
		{
			char* imageName;
			char* result = strtok_s(context, "_", &imageName);
			if (result != NULL)
			{
				//Group group(object.group.size(),imageName,faceNum);
				//object.group.push_back(group);
				//useMtl(mtlFileName, object);
				//object.imageName = imageName;
			}
		}
	}
	//노말라이즈 해서 노말 저장.
	for (int i = 0; i < p_normal.size() / 3; i++){
		glm::vec3 temp = glm::normalize(glm::vec3(p_normal[i * 3], p_normal[i * 3 + 1], p_normal[i * 3 + 2]));
		p_normal[i * 3] = temp.x;
		p_normal[i * 3 + 1] = temp.y;
		p_normal[i * 3 + 2] = temp.z;
	}
	//쉐도우 볼륨의 길이 조정.
	for (int i = 3; i < object.shadowVolume.size(); i += 3){
		//빛-점 사이 길이
		float length = pow(object.shadowVolume[i] - lightPos[0], 2) + pow(object.shadowVolume[i + 1] - lightPos[1], 2) + pow(object.shadowVolume[i + 2] - lightPos[2], 2);
		//제일 긴 길이*3 과 현재 길이 배율.
		float N = MaxLightToPosLen*3 / length;
		//float N = MaxLightToPosLen / length;//잠시 줄임.
		float vecx = object.shadowVolume[i] - lightPos[0];
		float vecy = object.shadowVolume[i+1] - lightPos[1];
		float vecz = object.shadowVolume[i+2] - lightPos[2];
		 vecx = vecx * N + lightPos[0];
		 vecy = vecy * N + lightPos[1];
		 vecz = vecz * N + lightPos[2]; 
		object.shadowVolume[i] = vecx;
		object.shadowVolume[i + 1] = vecy;
		object.shadowVolume[i + 2] = vecz;
	}
	glBindVertexArray(object.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*forBufferVtx.size(), forBufferVtx.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.VEO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*p_index.size(), p_index.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, object.VNO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*p_normal.size(), p_normal.data(), GL_STATIC_DRAW);

	glBindVertexArray(object.VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO_sh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*object.shadowVolume.size(), object.shadowVolume.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.VEO_sh);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*object.shadowV_index.size(), object.shadowV_index.data(), GL_STATIC_DRAW);

	//makeIndexForFaceInfo(faceInfo, index_tree, p_index, faceVtxNumver);//약간 복잡한 단계꺼.
	
	return p_vtx.size() / 3;
}

int main(int argc, char **argv)  
{
	//init GLUT and create Window
	//initialize the GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//These two functions are used to define the position and size of the window. 
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(700, 480);

	g_screenW = 700;
	g_screenH = 480;
	//This is used to define the name of the window.
	glutCreateWindow("Simple OpenGL Window");

	//call initization function
	init();	

	//texture
	prepareTexture(piggy);

	GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);
	g_programID = programID;

	GLint posID = glGetAttribLocation(g_programID, "pos");
	g_colorID = glGetAttribLocation(g_programID, "col");
	GLint texID = glGetAttribLocation(g_programID, "tex");
	GLint norID = glGetAttribLocation(g_programID, "nor");

	//바닥.
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(3, planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*36, planeVtx, GL_STATIC_DRAW);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(g_colorID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(g_colorID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 6, planeIndex, GL_STATIC_DRAW);
	GLfloat planeNor[12] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f };
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, planeNor, GL_STATIC_DRAW);
	glVertexAttribPointer(norID, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(norID);

	//삼각형
	glGenVertexArrays(1, &TriArr);
	glBindVertexArray(TriArr);
	glGenBuffers(2, TriBuff);
	glBindBuffer(GL_ARRAY_BUFFER, TriBuff[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, TriangeVtx, GL_STATIC_DRAW);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(g_colorID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(g_colorID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriBuff[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 3, TriIndex, GL_STATIC_DRAW);

	//앞에 보일 면 만들기.
	glGenVertexArrays(1, &planeArr);
	glGenBuffers(1, &plane2Buff);
	glBindVertexArray(planeArr);
	glBindBuffer(GL_ARRAY_BUFFER, plane2Buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, &planeVtx2, GL_STATIC_DRAW);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(g_colorID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(g_colorID);

	//gen cube
	glGenVertexArrays(2, cube.VAO);
	glGenBuffers(2, cube.VBO);
	glGenBuffers(2, cube.VEO);
	glGenBuffers(2, cube.VNO);
	//bind cube
	glBindVertexArray(cube.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO[0]);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);
	//glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(texID);
	glBindBuffer(GL_ARRAY_BUFFER, cube.VNO[0]);
	glVertexAttribPointer(norID, 3, GL_FLOAT, GL_FALSE,0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(norID);
	//bind cubeshadow
	glBindVertexArray(cube.VAO[1]);
	glGenBuffers(1, &cube.VBO_sh);
	glGenBuffers(1, &cube.VEO_sh);
	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO_sh);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);


	//gen pig
	glGenVertexArrays(2, piggy.VAO);
	glGenBuffers(1, piggy.VBO);
	glGenBuffers(1, piggy.VEO);
	glGenBuffers(1, piggy.VNO);
	//bind pig	
	glBindVertexArray(piggy.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, piggy.VBO[0]);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(texID);
	glBindBuffer(GL_ARRAY_BUFFER, piggy.VNO[0]);
	glVertexAttribPointer(norID, 3, GL_FLOAT, GL_FALSE,0 , (GLvoid*)(0));
	glEnableVertexAttribArray(norID);
	//bine pig shadow
	glBindVertexArray(piggy.VAO[1]);
	glGenBuffers(1, &piggy.VBO_sh);
	glGenBuffers(1, &piggy.VEO_sh);
	glBindBuffer(GL_ARRAY_BUFFER, piggy.VBO_sh);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, ((GLvoid*)(0)));
	glEnableVertexAttribArray(posID);

//큐브 좌표값 받기
	getEveryVtx(string("cube.obj"), cube.vertices, cube.texture_vtx, cube.indices, cube.normal ,cube.index_tree,cube);
	////돼지저금통 좌표값받기
	getEveryVtx(string("PiggyBank.obj"), piggy.vertices, piggy.texture_vtx, piggy.indices, piggy.normal,piggy.index_tree,piggy);

	

	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(renderScene);

	//enter GLUT event processing cycle
	glutMainLoop();
	return 1;
}

void cubeTextureMapping(Object& object){
	GLfloat cube_tex_vtx[16] =
	{
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		3.0, 0.0,
		2.0,0.0,
		3.1, 1.0,
		2.1,1.0
	};
	glBindVertexArray(object.VAO[0]);
	glBindBuffer(GL_VERTEX_ARRAY, object.TEX[0]);
	//glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	GLint texID = glGetAttribLocation(g_programID, "tex");
	glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16, cube_tex_vtx, GL_STATIC_DRAW);
	glEnableVertexAttribArray(texID);
	checkerboard(object);
}