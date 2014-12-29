#ifndef __AUGE_CARTO__H__
#define __AUGE_CARTO__H__

#include "AugeObject.h"
#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeStyle.h"
#include "AugeFeature.h"

#ifdef WIN32
#	ifdef AUGE_CARTO_EXPORTS
#		define AUGE_CARTO_API __declspec(dllexport)
#	else
#		define AUGE_CARTO_API __declspec(dllimport)
#	endif
#else
#	define AUGE_CARTO_API
#endif

namespace auge
{
	typedef enum LayerType
	{
		augeLayerNone		=	0,
		augeLayerFeature		 ,
		augeLayerRaster			 ,
		auerLayerTile			 ,
	}augeLayerType;

	class Layer;
	class FeatureLayer;
	class RasterLayer;
	class Style;
	class Renderer;
	class Transformation;
	class Canvas;

	class Map : public GObject
	{
	protected:
		Map(){}
		virtual ~Map(){}
	public:
		virtual const char*	GetName() = 0;
		virtual RESULTCODE	SetName(const char* szName) = 0;
		virtual GEnvelope&	GetExtent() = 0;
		virtual g_int		GetSRID() = 0;

		virtual g_uint		GetLayerCount() = 0;		
		virtual Layer*		GetLayer(g_uint i) = 0;
		virtual Layer*		GetLayer(const char* szName) = 0;

		virtual RESULTCODE	AddLayer(Layer* pLayer) = 0;


		
	};

	class Layer : public GObject
	{
	protected:
		Layer(){}
		virtual ~Layer(){}

	public:
		virtual const char*		GetName() = 0;
		virtual void			SetName(const char* szName) = 0;
		virtual augeLayerType	GetType() = 0;

		virtual g_int			GetSRID() = 0;
		virtual GEnvelope&		GetExtent() = 0;

		virtual bool			IsQueryable() = 0;
		virtual void			SetQueryable(bool flag) = 0;

		virtual bool			IsVisiable() = 0;
		virtual void			SetVisiable(bool flag) = 0;

		virtual RESULTCODE		SetStyle(Style* pStyle) = 0;
		virtual Style*			GetStyle() = 0;
	};

	class FeatureLayer : public Layer
	{
	protected:
		FeatureLayer(){}
		virtual ~FeatureLayer(){}
	public:
		virtual augeLayerType	GetType() = 0;
		virtual RESULTCODE		SetFeatureClass(FeatureClass* pFeatureClass) = 0;
		virtual FeatureClass*	GetFeatureClass() = 0;
	};

	class RasterLayer : public Layer
	{
	protected:
		RasterLayer(){}
		virtual ~RasterLayer(){}

	public:
		virtual augeLayerType	GetType() = 0;

	};

	class Canvas : public GObject
	{
	protected:
		Canvas(){}
		virtual ~Canvas(){}
	public:
		virtual void			SetViewer(GEnvelope& viewer) = 0;
		virtual void			DrawBackground(GColor& color) = 0;

		virtual void			Draw(Map*pMap) = 0;
		virtual void			DrawLayer(Layer* pLayer) = 0;
		virtual void			DrawLayer(FeatureLayer* pLayer) = 0;
		virtual void			DrawLayer(RasterLayer* pLayer) = 0;
		virtual void			DrawLayer(Layer* pLayer, Style* pStyle) = 0;

		virtual Transformation*	GetTransform() = 0;
		virtual Renderer*		GetRenderer() = 0;

		virtual RESULTCODE		Save(const char* path) = 0;
	};

	class Renderer : public GObject
	{
	protected:
		Renderer(){}
		virtual ~Renderer(){}
	public:
		virtual void		Draw(g_uchar* wkb, Symbolizer* pSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawPoint(WKBPoint			*pWKBPoint,				PointSymbolizer* pPointSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawPoint(WKBMultiPoint		*pWKBMultiPoint,		PointSymbolizer* pPointSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawLine(WKBLineString		*pWKBLineString,		Stroke* pStroke, Transformation* pTransformation) = 0;
		virtual void		DrawLine(WKBMultiLineString	*pWKBMultiLineString,	Stroke* pStroke, Transformation* pTransformation) = 0;
		virtual void		DrawLine(WKBLineString		*pWKBLineString,		LineSymbolizer* pSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawLine(WKBMultiLineString	*pWKBMultiLineString,	LineSymbolizer* pSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawPolygon(WKBPolygon		*pWKBPolygon,			PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation) = 0;
		virtual void		DrawPolygon(WKBMultiPolygon	*pWKBMultiPolygon,		PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation) = 0;

		virtual void		Label(const char* text, WKBPoint *pWKBPoint, TextSymbolizer* pSymbolizer, Transformation* pTransformation) = 0;

	public:
		virtual void		FillRectangle(g_uint width, g_uint height, GColor& color) = 0;
		virtual	void		DrawRectangle(g_uint width, g_uint height, GColor& color, double border) = 0;
		virtual void		FillRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color) = 0;
		virtual	void		DrawRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color, double border) = 0;

		

		virtual bool		SaveAsImage(const char* path) = 0;
	};

	class Transformation
	{
	protected:
		Transformation(){}
		virtual ~Transformation(){}

	public:
		virtual void			ToScreenPoint(double mapX, double mapY, int& screenX, int& screenY) = 0;
		virtual void			ToMapPoint(int screenX, int screenY, double& mapX, double mapY) = 0;

		virtual void			SetCanvas(int width, int height) = 0;
		virtual void			SetViewer(GEnvelope& viewer) = 0;

		virtual GEnvelope&		GetViewer() = 0;
	};

	class CartoManager
	{
	protected:
		CartoManager(){}
		virtual ~CartoManager(){}
	public:
		virtual g_uint			GetCount() = 0;
		virtual	Map*			GetMap(g_uint mid) = 0;
		virtual	Map*			GetMap(const char* name) = 0;
		virtual Map*			LoadMap(g_uint mid) = 0;
		virtual Map*			LoadMap(const char* name) = 0;

		virtual Map*			CreateMap(const char* name) = 0;
		virtual Map*			RemoveMap(const char* name) = 0;

		virtual Layer*			CreateLayer(const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_i, g_uint style_id) = 0;

		virtual g_uint			GetStyleCount() = 0;
		virtual Style*			GetStyle(g_uint id) = 0;
		virtual Style*			GetStyle(const char* name) = 0;
		virtual g_int			CreateStyle(const char* name, Style* pStyle) = 0;
		virtual bool			HasStyle(g_uint id) = 0;
		virtual bool			HasStyle(const char* name) = 0;
		virtual RESULTCODE		UpdateStyle(const char* name, Style* pStyle) = 0;
		virtual RESULTCODE		RemoveStyle(const char* name) = 0;

	public:
		virtual	RESULTCODE		Initialize(GConnection* pConnection) = 0;
		virtual void			Cleanup() = 0;
	};

	class CartoFactory
	{
	protected:
		CartoFactory(){}
		virtual ~CartoFactory(){}
	public:
		virtual Map*			CreateMap() = 0;
		virtual FeatureLayer*	CreateFeatureLayer() = 0;
		virtual Canvas*			CreateCanvas2D(g_uint width, g_uint height) = 0;
	};

	extern "C"
	{
		AUGE_CARTO_API CartoFactory*	augeGetCartoFactoryInstance();
		AUGE_CARTO_API CartoManager*	augeGetCartoManagerInstance();
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// �������趯̬��
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_CARTO_EXPORTS
#		define AUGE_CARTO_NAME "GCarto"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, AUGE_CARTO_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " AUGE_CARTO_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef AUGE_CARTO_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================

#endif// __AUGE_CARTO__H__