/*********************************************************\
 * Copyright (c) 2012-2013 Christian Ofenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "VertexBuffer/VertexBuffer.h"
#include "Framework/Color4.h"

#include <string.h>


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VertexBuffer::VertexBuffer(const char *rendererName) :
	IApplicationRenderer(rendererName)
{
	// Nothing to do in here
}

/**
*  @brief
*    Destructor
*/
VertexBuffer::~VertexBuffer()
{
	// The resources are released within "onDeinitialization()"
	// Nothing to do in here
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void VertexBuffer::onInitialization()
{
	// Call the base implementation
	IApplicationRenderer::onInitialization();

	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			{ // Create the program
				// Get the shader source code (outsourced to keep an overview)
				const char *vertexShaderSourceCode = nullptr;
				const char *fragmentShaderSourceCode = nullptr;
				#include "VertexBuffer_Cg.h"
				#include "VertexBuffer_GLSL_110.h"
				#include "VertexBuffer_GLSL_ES2.h"
				#include "VertexBuffer_HLSL_D3D9_D3D10_D3D11.h"
				#include "VertexBuffer_Null.h"

				// Create the program
				mProgram = shaderLanguage->createProgram(
					shaderLanguage->createVertexShader(vertexShaderSourceCode),
					shaderLanguage->createFragmentShader(fragmentShaderSourceCode));
			}

			// Is there a valid program?
			if (nullptr != mProgram)
			{
				// Vertex array object (VAO)
				// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
				// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
				// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
				//    reference of the used vertex buffer objects (VBO). If the reference counter of a
				//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.

				{ // Create vertex array object (VAO) using one vertex buffer object (VBO)
					// Create the vertex buffer object (VBO) holding position and color data
					// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
					// -> Traditional normalized RGB vertex colors
					static const float VERTEX_POSITION_COLOR[] =
					{	 // Position     Color				// Vertex ID	Triangle on screen
						 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,	// 0				0
						 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// 1			   .   .
						-0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// 2			  2.......1
					};
					Renderer::IVertexBufferPtr vertexBufferPositionColor(renderer->createVertexBuffer(sizeof(VERTEX_POSITION_COLOR), VERTEX_POSITION_COLOR, Renderer::BufferUsage::STATIC_DRAW));

					// Create vertex array object (VAO)
					const Renderer::VertexArrayAttribute vertexArray[] =
					{
						{ // Attribute 0
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_2,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Position",								// name[64] (char)
							"POSITION",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferPositionColor,				// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float) * (2 + 3),				// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						},
						{ // Attribute 1
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_3,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Color",								// name[64] (char)
							"COLOR",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferPositionColor,				// vertexBuffer (Renderer::IVertexBuffer *)
							sizeof(float) * 2,						// offset (unsigned int)
							sizeof(float) * (2 + 3),				// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						}
					};
					mVertexArrayVBO = mProgram->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
				}

				{ // Create vertex array object (VAO) using multiple vertex buffer object (VBO)
					// Create the vertex buffer object (VBO) holding position data
					// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
					static const float VERTEX_POSITION[] =
					{					// Vertex ID	Triangle on screen
						-0.5f,  0.0f,	// 0			  0.......1
						 1.0f,  0.0f,	// 1			   .   .
						 0.0f, -1.0f	// 2			  	2
					};
					Renderer::IVertexBufferPtr vertexBufferPosition(renderer->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, Renderer::BufferUsage::STATIC_DRAW));

					// Create the vertex buffer object (VBO) holding color data
					// -> Traditional normalized RGB vertex colors
					static const float VERTEX_COLOR[] =
					{					// Vertex ID	Triangle on screen
						1.0f, 0.0f, 0.0f,	// 0			  0.......1
						0.0f, 1.0f, 0.0f,	// 1			   .   .
						0.0f, 0.0f, 1.0f	// 2			  	2
					};
					Renderer::IVertexBufferPtr vertexBufferColor(renderer->createVertexBuffer(sizeof(VERTEX_COLOR), VERTEX_COLOR, Renderer::BufferUsage::STATIC_DRAW));

					// Create vertex array object (VAO)
					const Renderer::VertexArrayAttribute vertexArray[] =
					{
						{ // Attribute 0
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_2,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Position",								// name[64] (char)
							"POSITION",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferPosition,					// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float) * 2,						// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						},
						{ // Attribute 1
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_3,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Color",								// name[64] (char)
							"COLOR",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferColor,						// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float) * 3,						// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						}
					};
					mVertexArrayVBOs = mProgram->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
				}
			}
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}

void VertexBuffer::onDeinitialization()
{
	// Begin debug event
	RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(getRenderer())

	// Release the used resources
	mVertexArrayVBOs = nullptr;
	mVertexArrayVBO = nullptr;
	mProgram = nullptr;

	// End debug event
	RENDERER_END_DEBUG_EVENT(getRenderer())

	// Call the base implementation
	IApplicationRenderer::onDeinitialization();
}

void VertexBuffer::onDraw()
{
	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer && nullptr != mProgram)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Clear the color buffer of the current render target with gray, do also clear the depth buffer
		renderer->clear(Renderer::ClearFlag::COLOR_DEPTH, Color4::GRAY, 1.0f, 0);

		// Begin scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		if (renderer->beginScene())
		{
			// Set the used program
			renderer->setProgram(mProgram);

			{ // First lower triangle using one vertex buffer object (VBO)
				// Begin debug event
				RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Draw using one VBO")

				{ // Setup input assembly (IA)
					// Set the used vertex array
					renderer->iaSetVertexArray(mVertexArrayVBO);

					// Set the primitive topology used for draw calls
					renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
				}

				// Render the specified geometric primitive, based on an array of vertices
				renderer->draw(0, 3);

				// End debug event
				RENDERER_END_DEBUG_EVENT(renderer)
			}

			{ // Second upper triangle using multiple vertex buffer object (VBO)
				// Begin debug event
				RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Draw using multiple VBOs")

				{ // Setup input assembly (IA)
					// Set the used vertex array
					renderer->iaSetVertexArray(mVertexArrayVBOs);

					// Set the primitive topology used for draw calls
					renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
				}

				// Render the specified geometric primitive, based on an array of vertices
				renderer->draw(0, 3);

				// End debug event
				RENDERER_END_DEBUG_EVENT(renderer)
			}

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
