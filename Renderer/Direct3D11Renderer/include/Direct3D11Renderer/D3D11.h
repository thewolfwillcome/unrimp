/*********************************************************\
 * Copyright (c) 2012-2013 Christian Ofenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the �Software�), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


/*
  We don't use the Direct3D headers from the DirectX SDK because there are several issues:
  - Licensing: It's not allowed to redistribute the Direct3D headers, meaning everyone would
    have to get them somehow before compiling this project
  - The Direct3D headers are somewhat chaotic and include tons of other headers.
    This slows down compilation and the more headers are included, the higher the risk of
    naming or redefinition conflicts.
  - Starting with Windows 8, Direct3D is part of the Windows SDK. When using VisualStudio 2012
    and the Direct3D headers from "Microsoft DirectX SDK (June 2010)" you will get a lot of
    "
      <path>\external\directx\include\dxgitype.h(12): warning C4005: 'DXGI_STATUS_OCCLUDED' : macro redefinition
      c:\program files (x86)\windows kits\8.0\include\shared\winerror.h(49449) : see previous definition of 'DXGI_STATUS_OCCLUDED'
    "
    warnings.

    Do not include this header within headers which are usually used by users as well, do only
    use it inside cpp-files. It must still be possible that users of this renderer interface
    can use the Direct3D headers for features not covered by this renderer interface.
*/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once
#ifndef __DIRECT3D11RENDERER_D3D11_H__
#define __DIRECT3D11RENDERER_D3D11_H__


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Renderer/WindowsHeader.h>

__pragma(warning(push))
	__pragma(warning(disable: 4668))	// warning C4668: '<x>' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <Unknwn.h>	// For "IUnknown"
__pragma(warning(pop))


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct IDXGIOutput;
struct ID3D11Query;
struct IDXGIDevice;
struct IDXGISurface;
struct IDXGIAdapter;
struct ID3D11Buffer;
struct IDXGIAdapter1;
struct IDXGIFactory1;
struct D3D11_FEATURE;
struct ID3D11Counter;
struct ID3D11Resource;
struct IDXGISwapChain;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;
struct ID3D11Predicate;
struct ID3D11BlendState;
struct D3D11_BLEND_DESC;
struct D3D11_QUERY_DESC;
struct ID3D11HullShader;
struct ID3DX11ThreadPump;
struct ID3D11PixelShader;
struct D3D11_BUFFER_DESC;
struct ID3D11InputLayout;
struct ID3D11CommandList;
struct D3D11_COUNTER_INFO;
struct D3D11_COUNTER_TYPE;
struct D3D11_COUNTER_DESC;
struct ID3D11ClassLinkage;
struct ID3D11Asynchronous;
struct ID3D11DomainShader;
struct ID3D11VertexShader;
struct D3D11_SAMPLER_DESC;
struct ID3D11SamplerState;
struct ID3D11DeviceContext;
struct ID3D11ClassInstance;
struct ID3D11ComputeShader;
struct D3D11_TEXTURE1D_DESC;
struct D3D11_TEXTURE2D_DESC;
struct D3D11_TEXTURE3D_DESC;
struct ID3D11GeometryShader;
struct DXGI_FRAME_STATISTICS;
struct D3D11_RASTERIZER_DESC;
struct ID3D11RasterizerState;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct D3D11_DEPTH_STENCIL_DESC;
struct ID3D11ShaderResourceView;
struct D3D11_RESOURCE_DIMENSION;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11UnorderedAccessView;
struct D3D11_SO_DECLARATION_ENTRY;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct D3D11_SHADER_RESOURCE_VIEW_DESC;
struct D3D11_UNORDERED_ACCESS_VIEW_DESC;


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// "Microsoft DirectX SDK (June 2010)" -> "D3Dcompiler.h"
#define D3DCOMPILE_DEBUG				(1 << 0)
#define D3DCOMPILE_ENABLE_STRICTNESS	(1 << 11)

// "Microsoft DirectX SDK (June 2010)" -> "d3d9types.h"
typedef DWORD D3DCOLOR;

// "Microsoft DirectX SDK (June 2010)" -> "d3d9types.h"
#define D3DCOLOR_ARGB(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24) | (((r)&0xff)<<16) | (((g)&0xff)<<8) | ((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a, r, g, b)

// "Microsoft DirectX SDK (June 2010)" -> "D3X11.h"
#define D3D11_SDK_VERSION							(7)
#define	D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX	(15)
#define	D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT		(8)

// "Microsoft DirectX SDK (June 2010)" -> "D3D10.h"
#define	D3D10_VIEWPORT_AND_SCISSORRECT_MAX_INDEX	(15)
#define	D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT		(8)

// "Microsoft DirectX SDK (June 2010)" -> "D3X11.h"
#define	D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT	(32)

// "Microsoft DirectX SDK (June 2010)" -> "D3DX11.h"
#define D3DX11_DEFAULT	(static_cast<UINT>(-1))

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
#define DXGI_MWA_NO_ALT_ENTER	(1 << 1)

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
#define DXGI_USAGE_RENDER_TARGET_OUTPUT	(1L << (1 + 4))

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
enum D3D_DRIVER_TYPE
{
	D3D_DRIVER_TYPE_UNKNOWN		= 0,
	D3D_DRIVER_TYPE_HARDWARE	= (D3D_DRIVER_TYPE_UNKNOWN + 1),
	D3D_DRIVER_TYPE_REFERENCE	= (D3D_DRIVER_TYPE_HARDWARE + 1),
	D3D_DRIVER_TYPE_NULL		= (D3D_DRIVER_TYPE_REFERENCE + 1),
	D3D_DRIVER_TYPE_SOFTWARE	= (D3D_DRIVER_TYPE_NULL + 1),
	D3D_DRIVER_TYPE_WARP		= (D3D_DRIVER_TYPE_SOFTWARE + 1)
};

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
enum D3D_FEATURE_LEVEL
{
	D3D_FEATURE_LEVEL_9_1	= 0x9100,
	D3D_FEATURE_LEVEL_9_2	= 0x9200,
	D3D_FEATURE_LEVEL_9_3	= 0x9300,
	D3D_FEATURE_LEVEL_10_0	= 0xa000,
	D3D_FEATURE_LEVEL_10_1	= 0xa100,
	D3D_FEATURE_LEVEL_11_0	= 0xb000
};

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
enum D3D_SRV_DIMENSION
{
	D3D_SRV_DIMENSION_UNKNOWN				= 0,
	D3D_SRV_DIMENSION_BUFFER				= 1,
	D3D_SRV_DIMENSION_TEXTURE1D				= 2,
	D3D_SRV_DIMENSION_TEXTURE1DARRAY		= 3,
	D3D_SRV_DIMENSION_TEXTURE2D				= 4,
	D3D_SRV_DIMENSION_TEXTURE2DARRAY		= 5,
	D3D_SRV_DIMENSION_TEXTURE2DMS			= 6,
	D3D_SRV_DIMENSION_TEXTURE2DMSARRAY		= 7,
	D3D_SRV_DIMENSION_TEXTURE3D				= 8,
	D3D_SRV_DIMENSION_TEXTURECUBE			= 9,
	D3D_SRV_DIMENSION_TEXTURECUBEARRAY		= 10,
	D3D_SRV_DIMENSION_BUFFEREX				= 11,
	D3D11_SRV_DIMENSION_UNKNOWN				= D3D_SRV_DIMENSION_UNKNOWN,
	D3D11_SRV_DIMENSION_BUFFER				= D3D_SRV_DIMENSION_BUFFER,
	D3D11_SRV_DIMENSION_TEXTURE1D			= D3D_SRV_DIMENSION_TEXTURE1D,
	D3D11_SRV_DIMENSION_TEXTURE1DARRAY		= D3D_SRV_DIMENSION_TEXTURE1DARRAY,
	D3D11_SRV_DIMENSION_TEXTURE2D			= D3D_SRV_DIMENSION_TEXTURE2D,
	D3D11_SRV_DIMENSION_TEXTURE2DARRAY		= D3D_SRV_DIMENSION_TEXTURE2DARRAY,
	D3D11_SRV_DIMENSION_TEXTURE2DMS			= D3D_SRV_DIMENSION_TEXTURE2DMS,
	D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY	= D3D_SRV_DIMENSION_TEXTURE2DMSARRAY,
	D3D11_SRV_DIMENSION_TEXTURE3D			= D3D_SRV_DIMENSION_TEXTURE3D,
	D3D11_SRV_DIMENSION_TEXTURECUBE			= D3D_SRV_DIMENSION_TEXTURECUBE,
	D3D11_SRV_DIMENSION_TEXTURECUBEARRAY	= D3D_SRV_DIMENSION_TEXTURECUBEARRAY,
	D3D11_SRV_DIMENSION_BUFFEREX			= D3D_SRV_DIMENSION_BUFFEREX
};
typedef D3D_SRV_DIMENSION D3D11_SRV_DIMENSION;

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
enum D3D11_PRIMITIVE_TOPOLOGY
{
	D3D_PRIMITIVE_TOPOLOGY_UNDEFINED					= 0,
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST					= 1,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST						= 2,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP					= 3,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST					= 4,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP				= 5,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ					= 10,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ				= 11,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ				= 12,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ			= 13,
	D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	= 33,
	D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	= 34,
	D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	= 35,
	D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	= 36,
	D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	= 37,
	D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	= 38,
	D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	= 39,
	D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	= 40,
	D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	= 41,
	D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	= 42,
	D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	= 43,
	D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	= 44,
	D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	= 45,
	D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	= 46,
	D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	= 47,
	D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	= 48,
	D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	= 49,
	D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	= 50,
	D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	= 51,
	D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	= 52,
	D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	= 53,
	D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	= 54,
	D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	= 55,
	D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	= 56,
	D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	= 57,
	D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	= 58,
	D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	= 59,
	D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	= 60,
	D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	= 61,
	D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	= 62,
	D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	= 63,
	D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	= 64,
	D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED					= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D10_PRIMITIVE_TOPOLOGY_POINTLIST					= D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D10_PRIMITIVE_TOPOLOGY_LINELIST					= D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP					= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST				= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP				= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ				= D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ				= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ			= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ			= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED					= D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST					= D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST					= D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP					= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST				= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP				= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ				= D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ				= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ			= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ			= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
	D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
	D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	= D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST
};

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
struct D3D_SHADER_MACRO
{
	LPCSTR Name;
	LPCSTR Definition;
};
enum D3D_INCLUDE_TYPE
{
	D3D_INCLUDE_LOCAL		= 0,
	D3D_INCLUDE_SYSTEM		= (D3D_INCLUDE_LOCAL + 1),
	D3D10_INCLUDE_LOCAL		= D3D_INCLUDE_LOCAL,
	D3D10_INCLUDE_SYSTEM	= D3D_INCLUDE_SYSTEM,
	D3D_INCLUDE_FORCE_DWORD	= 0x7fffffff
};
DECLARE_INTERFACE(ID3DInclude)
{
	STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) PURE;
	STDMETHOD(Close)(THIS_ LPCVOID pData) PURE;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D10shader.h"
typedef D3D_SHADER_MACRO D3D10_SHADER_MACRO;
typedef __interface ID3DInclude *LPD3D10INCLUDE;

// "Microsoft DirectX SDK (June 2010)" -> "DXGIFormat.h"
enum DXGI_FORMAT
{
	DXGI_FORMAT_UNKNOWN	                    = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
	DXGI_FORMAT_R32G32B32A32_UINT           = 3,
	DXGI_FORMAT_R32G32B32A32_SINT           = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
	DXGI_FORMAT_R32G32B32_FLOAT             = 6,
	DXGI_FORMAT_R32G32B32_UINT              = 7,
	DXGI_FORMAT_R32G32B32_SINT              = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
	DXGI_FORMAT_R16G16B16A16_UINT           = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
	DXGI_FORMAT_R16G16B16A16_SINT           = 14,
	DXGI_FORMAT_R32G32_TYPELESS             = 15,
	DXGI_FORMAT_R32G32_FLOAT                = 16,
	DXGI_FORMAT_R32G32_UINT                 = 17,
	DXGI_FORMAT_R32G32_SINT                 = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
	DXGI_FORMAT_R10G10B10A2_UINT            = 25,
	DXGI_FORMAT_R11G11B10_FLOAT             = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
	DXGI_FORMAT_R8G8B8A8_UINT               = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
	DXGI_FORMAT_R8G8B8A8_SINT               = 32,
	DXGI_FORMAT_R16G16_TYPELESS             = 33,
	DXGI_FORMAT_R16G16_FLOAT                = 34,
	DXGI_FORMAT_R16G16_UNORM                = 35,
	DXGI_FORMAT_R16G16_UINT                 = 36,
	DXGI_FORMAT_R16G16_SNORM                = 37,
	DXGI_FORMAT_R16G16_SINT                 = 38,
	DXGI_FORMAT_R32_TYPELESS                = 39,
	DXGI_FORMAT_D32_FLOAT                   = 40,
	DXGI_FORMAT_R32_FLOAT                   = 41,
	DXGI_FORMAT_R32_UINT                    = 42,
	DXGI_FORMAT_R32_SINT                    = 43,
	DXGI_FORMAT_R24G8_TYPELESS              = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
	DXGI_FORMAT_R8G8_TYPELESS               = 48,
	DXGI_FORMAT_R8G8_UNORM                  = 49,
	DXGI_FORMAT_R8G8_UINT                   = 50,
	DXGI_FORMAT_R8G8_SNORM                  = 51,
	DXGI_FORMAT_R8G8_SINT                   = 52,
	DXGI_FORMAT_R16_TYPELESS                = 53,
	DXGI_FORMAT_R16_FLOAT                   = 54,
	DXGI_FORMAT_D16_UNORM                   = 55,
	DXGI_FORMAT_R16_UNORM                   = 56,
	DXGI_FORMAT_R16_UINT                    = 57,
	DXGI_FORMAT_R16_SNORM                   = 58,
	DXGI_FORMAT_R16_SINT                    = 59,
	DXGI_FORMAT_R8_TYPELESS                 = 60,
	DXGI_FORMAT_R8_UNORM                    = 61,
	DXGI_FORMAT_R8_UINT                     = 62,
	DXGI_FORMAT_R8_SNORM                    = 63,
	DXGI_FORMAT_R8_SINT                     = 64,
	DXGI_FORMAT_A8_UNORM                    = 65,
	DXGI_FORMAT_R1_UNORM                    = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
	DXGI_FORMAT_BC1_TYPELESS                = 70,
	DXGI_FORMAT_BC1_UNORM                   = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
	DXGI_FORMAT_BC2_TYPELESS                = 73,
	DXGI_FORMAT_BC2_UNORM                   = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
	DXGI_FORMAT_BC3_TYPELESS                = 76,
	DXGI_FORMAT_BC3_UNORM                   = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
	DXGI_FORMAT_BC4_TYPELESS                = 79,
	DXGI_FORMAT_BC4_UNORM                   = 80,
	DXGI_FORMAT_BC4_SNORM                   = 81,
	DXGI_FORMAT_BC5_TYPELESS                = 82,
	DXGI_FORMAT_BC5_UNORM                   = 83,
	DXGI_FORMAT_BC5_SNORM                   = 84,
	DXGI_FORMAT_B5G6R5_UNORM                = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
	DXGI_FORMAT_BC6H_TYPELESS               = 94,
	DXGI_FORMAT_BC6H_UF16                   = 95,
	DXGI_FORMAT_BC6H_SF16                   = 96,
	DXGI_FORMAT_BC7_TYPELESS                = 97,
	DXGI_FORMAT_BC7_UNORM                   = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
	DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
inline UINT D3D11CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT MipLevels)
{
	return MipSlice + ArraySlice * MipLevels;
}

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_CREATE_DEVICE_FLAG
{
	D3D11_CREATE_DEVICE_SINGLETHREADED								= 0x1,
	D3D11_CREATE_DEVICE_DEBUG										= 0x2,
	D3D11_CREATE_DEVICE_SWITCH_TO_REF								= 0x4,
	D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS	= 0x8,
	D3D11_CREATE_DEVICE_BGRA_SUPPORT								= 0x20
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_BOX
{
	UINT left;
	UINT top;
	UINT front;
	UINT right;
	UINT bottom;
	UINT back;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_VIEWPORT
{
	FLOAT TopLeftX;
	FLOAT TopLeftY;
	FLOAT Width;
	FLOAT Height;
	FLOAT MinDepth;
	FLOAT MaxDepth;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
typedef RECT D3D11_RECT;

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_RTV_DIMENSION
{
	D3D11_RTV_DIMENSION_UNKNOWN				= 0,
	D3D11_RTV_DIMENSION_BUFFER				= 1,
	D3D11_RTV_DIMENSION_TEXTURE1D			= 2,
	D3D11_RTV_DIMENSION_TEXTURE1DARRAY		= 3,
	D3D11_RTV_DIMENSION_TEXTURE2D			= 4,
	D3D11_RTV_DIMENSION_TEXTURE2DARRAY		= 5,
	D3D11_RTV_DIMENSION_TEXTURE2DMS			= 6,
	D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY	= 7,
	D3D11_RTV_DIMENSION_TEXTURE3D			= 8
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_DSV_DIMENSION
{
	D3D11_DSV_DIMENSION_UNKNOWN				= 0,
	D3D11_DSV_DIMENSION_TEXTURE1D			= 1,
	D3D11_DSV_DIMENSION_TEXTURE1DARRAY		= 2,
	D3D11_DSV_DIMENSION_TEXTURE2D			= 3,
	D3D11_DSV_DIMENSION_TEXTURE2DARRAY		= 4,
	D3D11_DSV_DIMENSION_TEXTURE2DMS			= 5,
	D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY	= 6
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
struct DXGI_SAMPLE_DESC
{
	UINT Count;
	UINT Quality;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct IDXGIObject : public IUnknown
{
	public:
		virtual HRESULT STDMETHODCALLTYPE SetPrivateData(__in REFGUID Name, UINT DataSize, __in_bcount(DataSize) const void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(__in REFGUID Name, __in const IUnknown *pUnknown) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetPrivateData(__in REFGUID Name, __inout UINT *pDataSize, __out_bcount(*pDataSize) void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetParent(__in REFIID riid, __out void **ppParent) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
struct DXGI_RATIONAL
{
	UINT Numerator;
	UINT Denominator;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
enum DXGI_MODE_SCANLINE_ORDER
{
	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED		= 0,
	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE		= 1,
	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST	= 2,
	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST	= 3
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
enum DXGI_MODE_SCALING
{
	DXGI_MODE_SCALING_UNSPECIFIED	= 0,
	DXGI_MODE_SCALING_CENTERED		= 1,
	DXGI_MODE_SCALING_STRETCHED		= 2
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGIType.h"
struct DXGI_MODE_DESC
{
	UINT Width;
	UINT Height;
	DXGI_RATIONAL RefreshRate;
	DXGI_FORMAT Format;
	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
	DXGI_MODE_SCALING Scaling;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
typedef UINT DXGI_USAGE;

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
enum DXGI_SWAP_EFFECT
{
	DXGI_SWAP_EFFECT_DISCARD	= 0,
	DXGI_SWAP_EFFECT_SEQUENTIAL	= 1
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct DXGI_SWAP_CHAIN_DESC
{
	DXGI_MODE_DESC BufferDesc;
	DXGI_SAMPLE_DESC SampleDesc;
	DXGI_USAGE BufferUsage;
	UINT BufferCount;
	HWND OutputWindow;
	BOOL Windowed;
	DXGI_SWAP_EFFECT SwapEffect;
	UINT Flags;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct IDXGIFactory : public IDXGIObject
{
	public:
		virtual HRESULT STDMETHODCALLTYPE EnumAdapters(UINT Adapter, __out IDXGIAdapter **ppAdapter) = 0;
		virtual HRESULT STDMETHODCALLTYPE MakeWindowAssociation(HWND WindowHandle, UINT Flags) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetWindowAssociation(__out HWND *pWindowHandle) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(__in IUnknown *pDevice, __in DXGI_SWAP_CHAIN_DESC *pDesc, __out IDXGISwapChain **ppSwapChain) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(HMODULE Module, __out IDXGIAdapter **ppAdapter) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
MIDL_INTERFACE("770aae78-f26f-4dba-a829-253c83d1b387")
IDXGIFactory1 : public IDXGIFactory
{
	public:
		virtual HRESULT STDMETHODCALLTYPE EnumAdapters1(UINT Adapter, __out IDXGIAdapter1 **ppAdapter) = 0;
		virtual BOOL STDMETHODCALLTYPE IsCurrent(void) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct DXGI_ADAPTER_DESC
{
	WCHAR Description[128];
	UINT VendorId;
	UINT DeviceId;
	UINT SubSysId;
	UINT Revision;
	SIZE_T DedicatedVideoMemory;
	SIZE_T DedicatedSystemMemory;
	SIZE_T SharedSystemMemory;
	LUID AdapterLuid;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct IDXGIAdapter : public IDXGIObject
{
	public:
		virtual HRESULT STDMETHODCALLTYPE EnumOutputs(UINT Output, __out IDXGIOutput **ppOutput) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetDesc(__out DXGI_ADAPTER_DESC *pDesc) = 0;
		virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(__in REFGUID InterfaceName, __out LARGE_INTEGER *pUMDVersion) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct DXGI_SURFACE_DESC
{
	UINT Width;
	UINT Height;
	DXGI_FORMAT Format;
	DXGI_SAMPLE_DESC SampleDesc;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct DXGI_SHARED_RESOURCE
{
	HANDLE Handle;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
enum DXGI_RESIDENCY
{
	DXGI_RESIDENCY_FULLY_RESIDENT				= 1,
	DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY	= 2,
	DXGI_RESIDENCY_EVICTED_TO_DISK				= 3
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
MIDL_INTERFACE("54ec77fa-1377-44e6-8c32-88fd5f44c84c")
IDXGIDevice : public IDXGIObject
{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetAdapter(__out IDXGIAdapter **pAdapter) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateSurface(__in const DXGI_SURFACE_DESC *pDesc, UINT NumSurfaces, DXGI_USAGE Usage, __in_opt const DXGI_SHARED_RESOURCE *pSharedResource, __out IDXGISurface **ppSurface) = 0;
		virtual HRESULT STDMETHODCALLTYPE QueryResourceResidency(__in_ecount(NumResources) IUnknown *const *ppResources, __out_ecount(NumResources) DXGI_RESIDENCY *pResidencyStatus, UINT NumResources) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(INT Priority) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(__out INT *pPriority) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct IDXGIDeviceSubObject : public IDXGIObject
{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetDevice(__in REFIID riid, __out void **ppDevice) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "DXGI.h"
struct IDXGISwapChain : public IDXGIDeviceSubObject
{
	public:
		virtual HRESULT STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetBuffer(UINT Buffer, __in REFIID riid, __out void **ppSurface) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(BOOL Fullscreen, __in_opt IDXGIOutput *pTarget) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(__out BOOL *pFullscreen, __out IDXGIOutput **ppTarget) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetDesc(__out DXGI_SWAP_CHAIN_DESC *pDesc) = 0;
		virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) = 0;
		virtual HRESULT STDMETHODCALLTYPE ResizeTarget(__in const DXGI_MODE_DESC *pNewTargetParameters) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(__out IDXGIOutput **ppOutput) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(__out DXGI_FRAME_STATISTICS *pStats) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(__out UINT *pLastPresentCount) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_DEVICE_CONTEXT_TYPE
{
	D3D11_DEVICE_CONTEXT_IMMEDIATE	= 0,
	D3D11_DEVICE_CONTEXT_DEFERRED	= (D3D11_DEVICE_CONTEXT_IMMEDIATE + 1)
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_CLEAR_FLAG
{
	D3D11_CLEAR_DEPTH	= 0x1L,
	D3D11_CLEAR_STENCIL	= 0x2L
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_USAGE
{
	D3D11_USAGE_DEFAULT		= 0,
	D3D11_USAGE_IMMUTABLE	= 1,
	D3D11_USAGE_DYNAMIC		= 2,
	D3D11_USAGE_STAGING		= 3
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_MAP
{
	D3D11_MAP_READ					= 1,
	D3D11_MAP_WRITE					= 2,
	D3D11_MAP_READ_WRITE			= 3,
	D3D11_MAP_WRITE_DISCARD			= 4,
	D3D11_MAP_WRITE_NO_OVERWRITE	= 5
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_BIND_FLAG
{
	D3D11_BIND_VERTEX_BUFFER	= 0x1L,
	D3D11_BIND_INDEX_BUFFER		= 0x2L,
	D3D11_BIND_CONSTANT_BUFFER	= 0x4L,
	D3D11_BIND_SHADER_RESOURCE	= 0x8L,
	D3D11_BIND_STREAM_OUTPUT	= 0x10L,
	D3D11_BIND_RENDER_TARGET	= 0x20L,
	D3D11_BIND_DEPTH_STENCIL	= 0x40L,
	D3D11_BIND_UNORDERED_ACCESS	= 0x80L
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_CPU_ACCESS_FLAG
{
	D3D11_CPU_ACCESS_WRITE	= 0x10000L,
	D3D11_CPU_ACCESS_READ	= 0x20000L
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_BUFFER_DESC
{
	UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_BUFFER_RTV
{
	union
	{
		UINT FirstElement;
		UINT ElementOffset;
	};
	union
	{
		UINT NumElements;
		UINT ElementWidth;
	};
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_RTV
{
	UINT MipSlice;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_ARRAY_RTV
{
	UINT MipSlice;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_RTV
{
	UINT MipSlice;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_RTV
{
	UINT UnusedField_NothingToDefine;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_ARRAY_RTV
{
	UINT MipSlice;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_ARRAY_RTV
{
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX3D_RTV
{
	UINT MipSlice;
	UINT FirstWSlice;
	UINT WSize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEXTURE2D_DESC
{
	UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	DXGI_FORMAT Format;
	DXGI_SAMPLE_DESC SampleDesc;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_DSV
{
	UINT MipSlice;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_ARRAY_DSV
{
	UINT MipSlice;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_DSV
{
	UINT MipSlice;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_ARRAY_DSV
{
	UINT MipSlice;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_DSV
{
	UINT UnusedField_NothingToDefine;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_ARRAY_DSV
{
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_SUBRESOURCE_DATA
{
	const void *pSysMem;
	UINT SysMemPitch;
	UINT SysMemSlicePitch;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_MAPPED_SUBRESOURCE
{
	void *pData;
	UINT RowPitch;
	UINT DepthPitch;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_BUFFER_SRV
{
	union
	{
		UINT FirstElement;
		UINT ElementOffset;
	};
	union
	{
		UINT NumElements;
		UINT ElementWidth;
	};
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX1D_ARRAY_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2D_ARRAY_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX3D_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEXCUBE_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEXCUBE_ARRAY_SRV
{
	UINT MostDetailedMip;
	UINT MipLevels;
	UINT First2DArrayFace;
	UINT NumCubes;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_SRV
{
	UINT UnusedField_NothingToDefine;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_TEX2DMS_ARRAY_SRV
{
	UINT FirstArraySlice;
	UINT ArraySize;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_BUFFEREX_SRV
{
	UINT FirstElement;
	UINT NumElements;
	UINT Flags;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_SHADER_RESOURCE_VIEW_DESC
{
	DXGI_FORMAT Format;
	D3D11_SRV_DIMENSION ViewDimension;
	union
	{
		D3D11_BUFFER_SRV Buffer;
		D3D11_TEX1D_SRV Texture1D;
		D3D11_TEX1D_ARRAY_SRV Texture1DArray;
		D3D11_TEX2D_SRV Texture2D;
		D3D11_TEX2D_ARRAY_SRV Texture2DArray;
		D3D11_TEX2DMS_SRV Texture2DMS;
		D3D11_TEX2DMS_ARRAY_SRV Texture2DMSArray;
		D3D11_TEX3D_SRV Texture3D;
		D3D11_TEXCUBE_SRV TextureCube;
		D3D11_TEXCUBE_ARRAY_SRV TextureCubeArray;
		D3D11_BUFFEREX_SRV BufferEx;
	};
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_DEPTH_STENCIL_VIEW_DESC
{
	DXGI_FORMAT Format;
	D3D11_DSV_DIMENSION ViewDimension;
	UINT Flags;
	union
	{
		D3D11_TEX1D_DSV Texture1D;
		D3D11_TEX1D_ARRAY_DSV Texture1DArray;
		D3D11_TEX2D_DSV Texture2D;
		D3D11_TEX2D_ARRAY_DSV Texture2DArray;
		D3D11_TEX2DMS_DSV Texture2DMS;
		D3D11_TEX2DMS_ARRAY_DSV Texture2DMSArray;
	};
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_RENDER_TARGET_VIEW_DESC
{
	DXGI_FORMAT Format;
	D3D11_RTV_DIMENSION ViewDimension;
	union
	{
		D3D11_BUFFER_RTV Buffer;
		D3D11_TEX1D_RTV Texture1D;
		D3D11_TEX1D_ARRAY_RTV Texture1DArray;
		D3D11_TEX2D_RTV Texture2D;
		D3D11_TEX2D_ARRAY_RTV Texture2DArray;
		D3D11_TEX2DMS_RTV Texture2DMS;
		D3D11_TEX2DMS_ARRAY_RTV Texture2DMSArray;
		D3D11_TEX3D_RTV Texture3D;
	};
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_INPUT_CLASSIFICATION
{
	D3D11_INPUT_PER_VERTEX_DATA		= 0,
	D3D11_INPUT_PER_INSTANCE_DATA	= 1
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_INPUT_ELEMENT_DESC
{
	LPCSTR SemanticName;
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11Device : public IUnknown
{
	public:
		virtual HRESULT STDMETHODCALLTYPE CreateBuffer(__in const D3D11_BUFFER_DESC *pDesc, __in_opt const D3D11_SUBRESOURCE_DATA *pInitialData, __out_opt ID3D11Buffer **ppBuffer) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(__in const D3D11_TEXTURE1D_DESC *pDesc, __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData, __out_opt ID3D11Texture1D **ppTexture1D) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(__in const D3D11_TEXTURE2D_DESC *pDesc, __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData, __out_opt ID3D11Texture2D **ppTexture2D) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(__in const D3D11_TEXTURE3D_DESC *pDesc, __in_xcount_opt(pDesc->MipLevels) const D3D11_SUBRESOURCE_DATA *pInitialData, __out_opt ID3D11Texture3D **ppTexture3D) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(__in ID3D11Resource *pResource, __in_opt const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, __out_opt ID3D11ShaderResourceView **ppSRView) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(__in ID3D11Resource *pResource, __in_opt const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, __out_opt ID3D11UnorderedAccessView **ppUAView) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(__in ID3D11Resource *pResource, __in_opt const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, __out_opt ID3D11RenderTargetView **ppRTView) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(__in ID3D11Resource *pResource, __in_opt const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, __out_opt ID3D11DepthStencilView **ppDepthStencilView) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(__in_ecount(NumElements) const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements, __in const void *pShaderBytecodeWithInputSignature, __in SIZE_T BytecodeLength, __out_opt ID3D11InputLayout **ppInputLayout) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11VertexShader **ppVertexShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11GeometryShader **ppGeometryShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_ecount_opt(NumEntries) const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, __in_range(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) UINT NumEntries, __in_ecount_opt(NumStrides) const UINT *pBufferStrides, __in_range(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumStrides, __in UINT RasterizedStream, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11GeometryShader **ppGeometryShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11PixelShader **ppPixelShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateHullShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11HullShader **ppHullShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11DomainShader **ppDomainShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(__in const void *pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage *pClassLinkage, __out_opt ID3D11ComputeShader **ppComputeShader) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(__out ID3D11ClassLinkage **ppLinkage) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateBlendState(__in const D3D11_BLEND_DESC *pBlendStateDesc, __out_opt ID3D11BlendState **ppBlendState) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(__in const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, __out_opt ID3D11DepthStencilState **ppDepthStencilState) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(__in const D3D11_RASTERIZER_DESC *pRasterizerDesc, __out_opt ID3D11RasterizerState **ppRasterizerState) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(__in const D3D11_SAMPLER_DESC *pSamplerDesc, __out_opt ID3D11SamplerState **ppSamplerState) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateQuery(__in const D3D11_QUERY_DESC *pQueryDesc, __out_opt ID3D11Query **ppQuery) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreatePredicate(__in const D3D11_QUERY_DESC *pPredicateDesc, __out_opt ID3D11Predicate **ppPredicate) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateCounter(__in const D3D11_COUNTER_DESC *pCounterDesc, __out_opt ID3D11Counter **ppCounter) = 0;
		virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags, __out_opt ID3D11DeviceContext **ppDeferredContext) = 0;
		virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(__in HANDLE hResource, __in REFIID ReturnedInterface, __out_opt void **ppResource) = 0;
		virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(__in DXGI_FORMAT Format, __out UINT *pFormatSupport) = 0;
		virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(__in DXGI_FORMAT Format, __in UINT SampleCount, __out UINT *pNumQualityLevels) = 0;
		virtual void STDMETHODCALLTYPE CheckCounterInfo(__out D3D11_COUNTER_INFO *pCounterInfo) = 0;
		virtual HRESULT STDMETHODCALLTYPE CheckCounter(__in const D3D11_COUNTER_DESC *pDesc, __out D3D11_COUNTER_TYPE *pType, __out UINT *pActiveCounters, __out_ecount_opt(*pNameLength) LPSTR szName, __inout_opt UINT *pNameLength, __out_ecount_opt(*pUnitsLength) LPSTR szUnits, __inout_opt UINT *pUnitsLength, __out_ecount_opt(*pDescriptionLength) LPSTR szDescription, __inout_opt UINT *pDescriptionLength) = 0;
		virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature, __out_bcount(FeatureSupportDataSize) void *pFeatureSupportData, UINT FeatureSupportDataSize) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetPrivateData(__in REFGUID guid, __inout UINT *pDataSize, __out_bcount_opt(*pDataSize) void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetPrivateData(__in REFGUID guid, __in UINT DataSize, __in_bcount_opt(DataSize) const void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(__in REFGUID guid, __in_opt const IUnknown *pData) = 0;
		virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel(void) = 0;
		virtual UINT STDMETHODCALLTYPE GetCreationFlags(void) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason(void) = 0;
		virtual void STDMETHODCALLTYPE GetImmediateContext(__out ID3D11DeviceContext **ppImmediateContext) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags) = 0;
		virtual UINT STDMETHODCALLTYPE GetExceptionMode(void) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11DeviceChild : public IUnknown
{
	public:
		virtual void STDMETHODCALLTYPE GetDevice(__out ID3D11Device **ppDevice) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetPrivateData(__in REFGUID guid, __inout UINT *pDataSize, __out_bcount_opt(*pDataSize) void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetPrivateData(__in REFGUID guid, __in UINT DataSize, __in_bcount_opt(DataSize) const void *pData) = 0;
		virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(__in REFGUID guid, __in_opt const IUnknown *pData) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11DeviceContext : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE VSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE PSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE PSSetShader(__in_opt ID3D11PixelShader *pPixelShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE PSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE VSSetShader(__in_opt ID3D11VertexShader *pVertexShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE DrawIndexed(__in UINT IndexCount, __in UINT StartIndexLocation, __in INT BaseVertexLocation) = 0;
		virtual void STDMETHODCALLTYPE Draw(__in UINT VertexCount, __in UINT StartVertexLocation) = 0;
		virtual HRESULT STDMETHODCALLTYPE Map(__in ID3D11Resource *pResource, __in UINT Subresource, __in D3D11_MAP MapType, __in UINT MapFlags, __out D3D11_MAPPED_SUBRESOURCE *pMappedResource) = 0;
		virtual void STDMETHODCALLTYPE Unmap(__in ID3D11Resource *pResource, __in UINT Subresource) = 0;
		virtual void STDMETHODCALLTYPE PSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE IASetInputLayout(__in_opt ID3D11InputLayout *pInputLayout) = 0;
		virtual void STDMETHODCALLTYPE IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppVertexBuffers, __in_ecount(NumBuffers) const UINT *pStrides, __in_ecount(NumBuffers) const UINT *pOffsets) = 0;
		virtual void STDMETHODCALLTYPE IASetIndexBuffer(__in_opt ID3D11Buffer *pIndexBuffer, __in DXGI_FORMAT Format, __in UINT Offset) = 0;
		virtual void STDMETHODCALLTYPE DrawIndexedInstanced(__in UINT IndexCountPerInstance, __in UINT InstanceCount, __in UINT StartIndexLocation, __in INT BaseVertexLocation, __in UINT StartInstanceLocation) = 0;
		virtual void STDMETHODCALLTYPE DrawInstanced(__in UINT VertexCountPerInstance, __in UINT InstanceCount, __in UINT StartVertexLocation, __in UINT StartInstanceLocation) = 0;
		virtual void STDMETHODCALLTYPE GSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE GSSetShader(__in_opt ID3D11GeometryShader *pShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(__in D3D11_PRIMITIVE_TOPOLOGY Topology) = 0;
		virtual void STDMETHODCALLTYPE VSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE VSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE Begin(__in ID3D11Asynchronous *pAsync) = 0;
		virtual void STDMETHODCALLTYPE End(__in ID3D11Asynchronous *pAsync) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetData(__in ID3D11Asynchronous *pAsync, __out_bcount_opt(DataSize) void *pData, __in UINT DataSize, __in UINT GetDataFlags) = 0;
		virtual void STDMETHODCALLTYPE SetPredication(__in_opt ID3D11Predicate *pPredicate, __in BOOL PredicateValue) = 0;
		virtual void STDMETHODCALLTYPE GSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE GSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE OMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews, __in_ecount_opt(NumViews) ID3D11RenderTargetView *const *ppRenderTargetViews, __in_opt ID3D11DepthStencilView *pDepthStencilView) = 0;
		virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(__in UINT NumRTVs, __in_ecount_opt(NumRTVs) ID3D11RenderTargetView *const *ppRenderTargetViews, __in_opt ID3D11DepthStencilView *pDepthStencilView, __in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot, __in UINT NumUAVs, __in_ecount_opt(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, __in_ecount_opt(NumUAVs) const UINT *pUAVInitialCounts) = 0;
		virtual void STDMETHODCALLTYPE OMSetBlendState(__in_opt ID3D11BlendState *pBlendState, __in_opt const FLOAT BlendFactor[4], __in UINT SampleMask) = 0;
		virtual void STDMETHODCALLTYPE OMSetDepthStencilState(__in_opt ID3D11DepthStencilState *pDepthStencilState, __in UINT StencilRef) = 0;
		virtual void STDMETHODCALLTYPE SOSetTargets(__in_range(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers, __in_ecount_opt(NumBuffers) ID3D11Buffer *const *ppSOTargets, __in_ecount_opt(NumBuffers) const UINT *pOffsets) = 0;
		virtual void STDMETHODCALLTYPE DrawAuto(void) = 0;
		virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(__in ID3D11Buffer *pBufferForArgs, __in UINT AlignedByteOffsetForArgs) = 0;
		virtual void STDMETHODCALLTYPE DrawInstancedIndirect(__in ID3D11Buffer *pBufferForArgs, __in UINT AlignedByteOffsetForArgs) = 0;
		virtual void STDMETHODCALLTYPE Dispatch(__in UINT ThreadGroupCountX, __in UINT ThreadGroupCountY, __in UINT ThreadGroupCountZ) = 0;
		virtual void STDMETHODCALLTYPE DispatchIndirect(__in ID3D11Buffer *pBufferForArgs, __in UINT AlignedByteOffsetForArgs) = 0;
		virtual void STDMETHODCALLTYPE RSSetState(__in_opt ID3D11RasterizerState *pRasterizerState) = 0;
		virtual void STDMETHODCALLTYPE RSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumViewports, __in_ecount_opt(NumViewports) const D3D11_VIEWPORT *pViewports) = 0;
		virtual void STDMETHODCALLTYPE RSSetScissorRects(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumRects, __in_ecount_opt(NumRects) const D3D11_RECT *pRects) = 0;
		virtual void STDMETHODCALLTYPE CopySubresourceRegion(__in ID3D11Resource *pDstResource, __in UINT DstSubresource, __in UINT DstX, __in UINT DstY, __in UINT DstZ, __in ID3D11Resource *pSrcResource, __in UINT SrcSubresource, __in_opt const D3D11_BOX *pSrcBox) = 0;
		virtual void STDMETHODCALLTYPE CopyResource(__in ID3D11Resource *pDstResource, __in ID3D11Resource *pSrcResource) = 0;
		virtual void STDMETHODCALLTYPE UpdateSubresource(__in ID3D11Resource *pDstResource, __in UINT DstSubresource, __in_opt const D3D11_BOX *pDstBox, __in const void *pSrcData, __in UINT SrcRowPitch, __in UINT SrcDepthPitch) = 0;
		virtual void STDMETHODCALLTYPE CopyStructureCount(__in ID3D11Buffer *pDstBuffer, __in UINT DstAlignedByteOffset, __in ID3D11UnorderedAccessView *pSrcView) = 0;
		virtual void STDMETHODCALLTYPE ClearRenderTargetView(__in ID3D11RenderTargetView *pRenderTargetView, __in const FLOAT ColorRGBA[4]) = 0;
		virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(__in ID3D11UnorderedAccessView *pUnorderedAccessView, __in const UINT Values[4]) = 0;
		virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(__in ID3D11UnorderedAccessView *pUnorderedAccessView, __in const FLOAT Values[4]) = 0;
		virtual void STDMETHODCALLTYPE ClearDepthStencilView(__in ID3D11DepthStencilView *pDepthStencilView, __in UINT ClearFlags, __in FLOAT Depth, __in UINT8 Stencil) = 0;
		virtual void STDMETHODCALLTYPE GenerateMips(__in ID3D11ShaderResourceView *pShaderResourceView) = 0;
		virtual void STDMETHODCALLTYPE SetResourceMinLOD(__in ID3D11Resource *pResource, FLOAT MinLOD) = 0;
		virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(__in ID3D11Resource *pResource) = 0;
		virtual void STDMETHODCALLTYPE ResolveSubresource(__in ID3D11Resource *pDstResource, __in UINT DstSubresource, __in ID3D11Resource *pSrcResource, __in UINT SrcSubresource, __in DXGI_FORMAT Format) = 0;
		virtual void STDMETHODCALLTYPE ExecuteCommandList(__in ID3D11CommandList *pCommandList, BOOL RestoreContextState) = 0;
		virtual void STDMETHODCALLTYPE HSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE HSSetShader(__in_opt ID3D11HullShader *pHullShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE HSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE HSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE DSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE DSSetShader(__in_opt ID3D11DomainShader *pDomainShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE DSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE DSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE CSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(__in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs, __in_ecount(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, __in_ecount(NumUAVs) const UINT *pUAVInitialCounts) = 0;
		virtual void STDMETHODCALLTYPE CSSetShader(__in_opt ID3D11ComputeShader *pComputeShader, __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE CSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE CSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE VSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE PSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE PSGetShader(__out ID3D11PixelShader **ppPixelShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE PSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE VSGetShader(__out ID3D11VertexShader **ppVertexShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE PSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE IAGetInputLayout(__out ID3D11InputLayout **ppInputLayout) = 0;
		virtual void STDMETHODCALLTYPE IAGetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount_opt(NumBuffers) ID3D11Buffer **ppVertexBuffers, __out_ecount_opt(NumBuffers) UINT *pStrides, __out_ecount_opt(NumBuffers) UINT *pOffsets) = 0;
		virtual void STDMETHODCALLTYPE IAGetIndexBuffer(__out_opt ID3D11Buffer **pIndexBuffer, __out_opt DXGI_FORMAT *Format, __out_opt UINT *Offset) = 0;
		virtual void STDMETHODCALLTYPE GSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE GSGetShader(__out ID3D11GeometryShader **ppGeometryShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(__out D3D11_PRIMITIVE_TOPOLOGY *pTopology) = 0;
		virtual void STDMETHODCALLTYPE VSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE VSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE GetPredication(__out_opt ID3D11Predicate **ppPredicate, __out_opt BOOL *pPredicateValue) = 0;
		virtual void STDMETHODCALLTYPE GSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE GSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE OMGetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews, __out_ecount_opt(NumViews) ID3D11RenderTargetView **ppRenderTargetViews, __out_opt ID3D11DepthStencilView **ppDepthStencilView) = 0;
		virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumRTVs, __out_ecount_opt(NumRTVs) ID3D11RenderTargetView **ppRenderTargetViews, __out_opt ID3D11DepthStencilView **ppDepthStencilView, __in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot, __in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot) UINT NumUAVs, __out_ecount_opt(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews) = 0;
		virtual void STDMETHODCALLTYPE OMGetBlendState(__out_opt ID3D11BlendState **ppBlendState, __out_opt FLOAT BlendFactor[4], __out_opt UINT *pSampleMask) = 0;
		virtual void STDMETHODCALLTYPE OMGetDepthStencilState(__out_opt ID3D11DepthStencilState **ppDepthStencilState, __out_opt UINT *pStencilRef) = 0;
		virtual void STDMETHODCALLTYPE SOGetTargets(__in_range(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppSOTargets) = 0;
		virtual void STDMETHODCALLTYPE RSGetState(__out ID3D11RasterizerState **ppRasterizerState) = 0;
		virtual void STDMETHODCALLTYPE RSGetViewports(__inout UINT *pNumViewports, __out_ecount_opt(*pNumViewports) D3D11_VIEWPORT *pViewports) = 0;
		virtual void STDMETHODCALLTYPE RSGetScissorRects(__inout UINT *pNumRects, __out_ecount_opt(*pNumRects) D3D11_RECT *pRects) = 0;
		virtual void STDMETHODCALLTYPE HSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE HSGetShader(__out ID3D11HullShader **ppHullShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE HSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE HSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE DSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE DSGetShader(__out ID3D11DomainShader **ppDomainShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE DSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE DSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE CSGetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews, __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews) = 0;
		virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(__in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs, __out_ecount(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews) = 0;
		virtual void STDMETHODCALLTYPE CSGetShader(__out ID3D11ComputeShader **ppComputeShader, __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances, __inout_opt UINT *pNumClassInstances) = 0;
		virtual void STDMETHODCALLTYPE CSGetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers, __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers) = 0;
		virtual void STDMETHODCALLTYPE CSGetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers, __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers) = 0;
		virtual void STDMETHODCALLTYPE ClearState(void) = 0;
		virtual void STDMETHODCALLTYPE Flush(void) = 0;
		virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType(void) = 0;
		virtual UINT STDMETHODCALLTYPE GetContextFlags(void) = 0;
		virtual HRESULT STDMETHODCALLTYPE FinishCommandList(BOOL RestoreDeferredContextState, __out_opt ID3D11CommandList **ppCommandList) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11InputLayout : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11SamplerState : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_SAMPLER_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11RasterizerState : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_RASTERIZER_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11BlendState : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_BLEND_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11DepthStencilState : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_DEPTH_STENCIL_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11VertexShader : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11HullShader : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11DomainShader : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11GeometryShader : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11PixelShader : public ID3D11DeviceChild
{
	// Nothing in here
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11Resource : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetType(__out D3D11_RESOURCE_DIMENSION *pResourceDimension) = 0;
		virtual void STDMETHODCALLTYPE SetEvictionPriority(__in UINT EvictionPriority) = 0;
		virtual UINT STDMETHODCALLTYPE GetEvictionPriority(void) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11Buffer : public ID3D11Resource
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_BUFFER_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
MIDL_INTERFACE("6f15aaf2-d208-4e89-9ab4-489535d34f9c")
ID3D11Texture2D : public ID3D11Resource
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_TEXTURE2D_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11View : public ID3D11DeviceChild
{
	public:
		virtual void STDMETHODCALLTYPE GetResource(__out ID3D11Resource **ppResource) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11ShaderResourceView : public ID3D11View
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11RenderTargetView : public ID3D11View
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_RENDER_TARGET_VIEW_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11DepthStencilView : public ID3D11View
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3Dcommon.h"
struct ID3D10Blob : public IUnknown
{
	public:
		virtual LPVOID STDMETHODCALLTYPE GetBufferPointer(void) = 0;
		virtual SIZE_T STDMETHODCALLTYPE GetBufferSize(void) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11Asynchronous : public ID3D11DeviceChild
{
	public:
		virtual UINT STDMETHODCALLTYPE GetDataSize(void) = 0;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
enum D3D11_QUERY
{
	D3D11_QUERY_EVENT							= 0,
	D3D11_QUERY_OCCLUSION						= (D3D11_QUERY_EVENT + 1),
	D3D11_QUERY_TIMESTAMP						= (D3D11_QUERY_OCCLUSION + 1),
	D3D11_QUERY_TIMESTAMP_DISJOINT				= (D3D11_QUERY_TIMESTAMP + 1),
	D3D11_QUERY_PIPELINE_STATISTICS				= (D3D11_QUERY_TIMESTAMP_DISJOINT + 1),
	D3D11_QUERY_OCCLUSION_PREDICATE				= (D3D11_QUERY_PIPELINE_STATISTICS + 1),
	D3D11_QUERY_SO_STATISTICS					= (D3D11_QUERY_OCCLUSION_PREDICATE + 1),
	D3D11_QUERY_SO_OVERFLOW_PREDICATE			= (D3D11_QUERY_SO_STATISTICS + 1),
	D3D11_QUERY_SO_STATISTICS_STREAM0			= (D3D11_QUERY_SO_OVERFLOW_PREDICATE + 1),
	D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0	= (D3D11_QUERY_SO_STATISTICS_STREAM0 + 1),
	D3D11_QUERY_SO_STATISTICS_STREAM1			= (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 + 1),
	D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1	= (D3D11_QUERY_SO_STATISTICS_STREAM1 + 1),
	D3D11_QUERY_SO_STATISTICS_STREAM2			= (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1 + 1),
	D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2	= (D3D11_QUERY_SO_STATISTICS_STREAM2 + 1),
	D3D11_QUERY_SO_STATISTICS_STREAM3			= (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2 + 1),
	D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM3	= (D3D11_QUERY_SO_STATISTICS_STREAM3 + 1) 
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct D3D11_QUERY_DESC
{
	D3D11_QUERY Query;
	UINT MiscFlags;
};

// "Microsoft DirectX SDK (June 2010)" -> "D3D11.h"
struct ID3D11Query : public ID3D11Asynchronous
{
	public:
		virtual void STDMETHODCALLTYPE GetDesc(__out D3D11_QUERY_DESC *pDesc) = 0;
};


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#endif // __DIRECT3D11RENDERER_D3D11_H__