#include "pch.h"
#include "PostProcess.h"
#include "RenderManager.h"
#include "GameClock.h"
//#include "RenderHelper.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

using namespace Fiea::GameEngine;

void PostProcess::Init()
{
	// create a framebuffer for post processing
	glGenFramebuffers(1, &FBO);

	// bind the framebuffer as the active framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create a framebuffer for post processing
	//glGenFramebuffers(1, &RFBO);

	// bind the read framebuffer as the active framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, RFBO);

	/*Unfortunately, we can't use our framebuffer yet because it is not complete. For a framebuffer to be complete the following requirements have to be satisfied:
		- We have to attach at least one buffer (color, depth or stencil buffer).
		- There should be at least one color attachment.
		- All attachments should be complete as well (reserved memory).
		- Each buffer should have the same number of samples.
	*/

	// An attachment is a memory location that can act as a buffer for the framebuffer, think of it as an image.
	// When creating an attachment we have two options to take: textures or renderbuffer objects.

	// render buffer object, only used here if we need depth and stencil testing
	glGenRenderbuffers(1, &RBO);
	/*glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, , 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);*/

	// check if the framebuffer is complete and if it's not, we print an error message.
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << fboStatus << std::endl;	// ERROR HERE
	}

	// unbind render buffer for good practice
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// bind current frame buffer to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create a texture for the framebuffer
	glGenTextures(1, &TCB);
	glBindTexture(GL_TEXTURE_2D, TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_COLOR_ATTACHMENT0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

	// check if the framebuffer is complete and if it's not, we print an error message.
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << fboStatus << std::endl;
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	ppImg = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Screen Overlay/cuphead_screen_fx_sheet.png", ppText, true);

	// attach it to currently bound framebuffer object
	/*The glFrameBufferTexture2D function has the following parameters:
		target: the framebuffer type we're targeting (draw, read or both).
		attachment: the type of attachment we're going to attach. Right now we're attaching a color attachment.
		textarget: the type of the texture you want to attach.
		texture: the actual texture to attach.
		level: the mipmap level. We keep this at 0.
	*/
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);

	// check if the framebuffer is complete and if it's not, we print an error message.
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frambuffer error: " << fboStatus << std::endl;
	}
	
	// initialize the VAO and VBO
	InitVAOVBO();

	// unbind the framebuffer to make sure we're not accidentally rendering to the wrong framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*to draw the scene to a single texture we'll have to take the following steps:
		- Render the scene as usual with the new framebuffer bound as the active framebuffer.
		- Bind to the default framebuffer.
		- Draw a quad that spans the entire screen with the new framebuffer's color buffer as its texture.
	*/

	//When we're done with all framebuffer operations, do not forget to delete the framebuffer object:
	//glDeleteFramebuffers(1, &fbo);

	// Load shaders
	PostProcessShader = new Shader("..\\FieaGameEngine\\FrameBuffer.vs", "..\\FieaGameEngine\\PostProcess.fs");

	// activate shader
	PostProcessShader->use();

	GLint postTexture = glGetUniformLocation(PostProcessShader->ID, "texture1");
	glUniform1i(postTexture, 0);

	GLint sceneTexture = glGetUniformLocation(PostProcessShader->ID, "sceneTexture");
	glUniform1i(sceneTexture, 1);
}

void PostProcess::InitVAOVBO()
{
	// Load and cache VAO, all sprites are quads
	float vertices[] = {
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f , 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PostProcess::BeginRender()
{
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// specify the color of the background 
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// make sure we clear the framebuffer's content
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcess::Render()
{
	// use our shader
	PostProcessShader->use();

	// bind our post process texture to text0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ppText);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, TCB);

	GLint frameLocation = glGetUniformLocation(PostProcessShader->ID, "frameIndex");
	GLint widthLocation = glGetUniformLocation(PostProcessShader->ID, "nRows");
	GLint heightLocation = glGetUniformLocation(PostProcessShader->ID, "nCols");
	
	// set frame index to 0
	glUniform1i(glGetUniformLocation(PostProcessShader->ID, "frameIndex"), int(currFrame));

	// number of rows is 12
	glUniform1i(glGetUniformLocation(PostProcessShader->ID, "nRows"), 12);
	// number of columns is 11
	glUniform1i(glGetUniformLocation(PostProcessShader->ID, "nCols"), 11);


	// binds our VAO to the vertex Array
	glBindVertexArray(VAO);

	// Draw 2 triangles on screen
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcess::EndRender()
{
	// this is where we would render the post process effect that should blend the scene texture created in above loop with the
	// post process texture
	
	// we need to get the current scene as a texture and send that as sceneTexture then multiply it
	// with post process texture "texture1"

	// we bind the read frame buffer and the draw frame buffer
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, RFBO);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

	// Copies the contents of RFBO to FBO
	//glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

/// @brief Update the post process' sprite sheet current frame in the animation
/// @param time 
void Fiea::GameEngine::PostProcess::update(const GameTime& time)
{
	currFrame += 24 * time.Frame() * 0.001f;

	while (currFrame >= 127)
	{
		currFrame -= (127 - 1);
	}
}
