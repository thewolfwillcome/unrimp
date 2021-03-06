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
#include "OpenGLRenderer/DepthStencilState.h"
#include "OpenGLRenderer/OpenGLRuntimeLinking.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace OpenGLRenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Constructor
	*/
	DepthStencilState::DepthStencilState(OpenGLRenderer &openGLRenderer, const Renderer::DepthStencilState &depthStencilState) :
		IDepthStencilState(reinterpret_cast<Renderer::IRenderer&>(openGLRenderer)),
		mDepthStencilState(depthStencilState)
	{
		// Nothing to do in here
	}

	/**
	*  @brief
	*    Destructor
	*/
	DepthStencilState::~DepthStencilState()
	{
		// Nothing to do in here
	}

	/**
	*  @brief
	*    Set the OpenGL depth stencil states
	*/
	void DepthStencilState::setOpenGLDepthStencilStates() const
	{
		// Renderer::DepthStencilState::depthEnable
		if (mDepthStencilState.depthEnable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		// Renderer::DepthStencilState::depthWriteMask
		glDepthMask(static_cast<GLboolean>((Renderer::DepthWriteMask::ALL == mDepthStencilState.depthWriteMask) ? GL_TRUE : GL_FALSE));

		// TODO(co) Map the rest of the depth stencil states
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // OpenGLRenderer
