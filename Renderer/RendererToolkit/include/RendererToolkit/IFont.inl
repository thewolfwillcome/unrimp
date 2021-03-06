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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RendererToolkit
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Destructor
	*/
	inline IFont::~IFont()
	{
		// Nothing to do in here
	}

	/**
	*  @brief
	*    Return the font size in points (72 points per inch)
	*/
	inline unsigned int IFont::getSize() const
	{
		return mSize;
	}

	/**
	*  @brief
	*    Return the horizontal and vertical resolution in DPI
	*/
	inline unsigned int IFont::getResolution() const
	{
		return mResolution;
	}

	/**
	*  @brief
	*    Return the font size in pixels
	*/
	inline unsigned int IFont::getSizeInPixels() const
	{
		return static_cast<unsigned int>(mSize / 72.0f * mResolution);
	}

	/**
	*  @brief
	*    Return the font height in pixels
	*/
	inline unsigned int IFont::getHeightInPixels() const
	{
		return static_cast<unsigned int>(getHeight() / 72.0f * mResolution);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Default constructor
	*/
	inline IFont::IFont()
	{
		// Nothing to do in here
	}

	/**
	*  @brief
	*    Copy constructor
	*/
	inline IFont::IFont(const IFont &)
	{
		// Not supported
	}

	/**
	*  @brief
	*    Copy operator
	*/
	inline IFont &IFont::operator =(const IFont &)
	{
		// Not supported
		return *this;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RendererToolkit
