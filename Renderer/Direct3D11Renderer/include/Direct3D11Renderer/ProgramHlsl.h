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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once
#ifndef __DIRECT3D11RENDERER_PROGRAM_HLSL_H__
#define __DIRECT3D11RENDERER_PROGRAM_HLSL_H__


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Direct3D11Renderer/Program.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Direct3D11Renderer
{


	//[-------------------------------------------------------]
	//[ Forward declarations                                  ]
	//[-------------------------------------------------------]
	class VertexShaderHlsl;
	class GeometryShaderHlsl;
	class FragmentShaderHlsl;
	class TessellationControlShaderHlsl;
	class TessellationEvaluationShaderHlsl;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL program class
	*/
	class ProgramHlsl : public Program
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D11Renderer
		*    Owner Direct3D 11 renderer instance
		*  @param[in] vertexShaderHlsl
		*    Vertex shader the program is using, can be a null pointer
		*  @param[in] tessellationControlShaderHlsl
		*    Tessellation control shader the program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderHlsl
		*    Tessellation evaluation shader the program is using, can be a null pointer
		*  @param[in] geometryShaderHlsl
		*    Geometry shader the program is using, can be a null pointer
		*  @param[in] fragmentShaderHlsl
		*    Fragment shader the program is using, can be a null pointer
		*
		*  @note
		*    - The program keeps a reference to the provided shaders and releases it when no longer required
		*/
		ProgramHlsl(Direct3D11Renderer &direct3D11Renderer, VertexShaderHlsl *vertexShaderHlsl, TessellationControlShaderHlsl *tessellationControlShaderHlsl, TessellationEvaluationShaderHlsl *tessellationEvaluationShaderHlsl, GeometryShaderHlsl *geometryShaderHlsl, FragmentShaderHlsl *fragmentShaderHlsl);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ProgramHlsl();

		/**
		*  @brief
		*    Return the HLSL vertex shader the program is using
		*
		*  @return
		*    The HLSL vertex shader the program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		inline VertexShaderHlsl *getVertexShaderHlsl() const;

		/**
		*  @brief
		*    Return the HLSL tessellation control shader the program is using
		*
		*  @return
		*    The HLSL tessellation control shader the program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		inline TessellationControlShaderHlsl *getTessellationControlShaderHlsl() const;

		/**
		*  @brief
		*    Return the HLSL tessellation evaluation shader the program is using
		*
		*  @return
		*    The HLSL tessellation evaluation shader the program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		inline TessellationEvaluationShaderHlsl *getTessellationEvaluationShaderHlsl() const;

		/**
		*  @brief
		*    Return the HLSL geometry shader the program is using
		*
		*  @return
		*    The HLSL geometry shader the program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		inline GeometryShaderHlsl *getGeometryShaderHlsl() const;

		/**
		*  @brief
		*    Return the HLSL fragment shader the program is using
		*
		*  @return
		*    The HLSL fragment shader the program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		inline FragmentShaderHlsl *getFragmentShaderHlsl() const;


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IProgram methods             ]
	//[-------------------------------------------------------]
	public:
		virtual Renderer::IVertexArray *createVertexArray(unsigned int numberOfAttributes, const Renderer::VertexArrayAttribute *attributes, Renderer::IIndexBuffer *indexBuffer = nullptr) override;
		virtual int getAttributeLocation(const char *attributeName) override;
		virtual unsigned int getUniformBlockIndex(const char *uniformBlockName, unsigned int defaultIndex) override;
		virtual handle getUniformHandle(const char *uniformName) override;
		virtual void setUniform1f(handle uniformHandle, float value) override;
		virtual void setUniform2fv(handle uniformHandle, const float *value) override;
		virtual void setUniform3fv(handle uniformHandle, const float *value) override;
		virtual void setUniform4fv(handle uniformHandle, const float *value) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		VertexShaderHlsl				 *mVertexShaderHlsl;					/**< Vertex shader the program is using (we keep a reference to it), can be a null pointer */
		TessellationControlShaderHlsl	 *mTessellationControlShaderHlsl;		/**< Tessellation control shader the program is using (we keep a reference to it), can be a null pointer */
		TessellationEvaluationShaderHlsl *mTessellationEvaluationShaderHlsl;	/**< Tessellation evaluation shader the program is using (we keep a reference to it), can be a null pointer */
		GeometryShaderHlsl				 *mGeometryShaderHlsl;					/**< Geometry shader the program is using (we keep a reference to it), can be a null pointer */
		FragmentShaderHlsl				 *mFragmentShaderHlsl;					/**< Fragment shader the program is using (we keep a reference to it), can be a null pointer */


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Direct3D11Renderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "Direct3D11Renderer/ProgramHlsl.inl"


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#endif // __DIRECT3D11RENDERER_PROGRAM_HLSL_H__
