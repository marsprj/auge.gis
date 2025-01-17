#ifndef __AUGE_CANVAS_IMPL_H__
#define __AUGE_CANVAS_IMPL_H__

#include "AugeCarto.h"
#include "TransformImpl.h"
#include "Maplex.h"
#include <vector>

namespace auge
{
	class GLabel;

	class CanvasImpl : public Canvas
	{
	public:
		CanvasImpl(g_uint width, g_uint height);
		virtual ~CanvasImpl();
	public:
		virtual void			SetViewer(GEnvelope& viewer);
		virtual void			DrawBackground(GColor& color);

		virtual void			Draw(Map*pMap);
		virtual void			DrawLayer(Layer* pLayer);
		virtual void			DrawLayer(FeatureLayer* pLayer);
		virtual void			DrawLayer(RasterLayer* pLayer);
		virtual void			DrawLayer(Layer* pLayer, Style* pStyle);
		virtual void			DrawLayer(FeatureLayer* pLayer, FeatureStyle* pStyle);
		virtual void			Label();

		virtual void			DrawSymbol(Geometry* pGeometry, Symbol* pSymbol);

		virtual Transformation*	GetTransform();
		virtual Renderer*		GetRenderer();

		virtual RESULTCODE		Save(const char* path);
		virtual RESULTCODE		Save(g_uint x, g_uint y, g_uint width, g_uint height, const char* path);

	private:
		void		DrawLayer(FeatureLayer* pLayer, Rule* pRule);
		void		DrawLayer(FeatureLayer* pLayer, Symbolizer* pSymbolizer, GFilter* pFilter, g_uint limit=0);

		void		LabelLayer(FeatureLayer* pLayer, TextSymbolizer* pSymbolizer, GFilter* pFilter, g_uint limit=0);
		void		LabelGeometry(g_uchar* wkb, TextSymbolizer* pSymbolizer, const char* text);
		void		LabelPoint(WKBPoint* pWKBPoint, TextSymbolizer* pSymbolizer,const char* text);

		void		LabelLayer(LabelSet* pLabelSet, FeatureLayer* pLayer, TextSymbolizer* pSymbolizer, GFilter* pFilter);
		GLabel*		CreateLabel(augeGeometryType type);

		//GFilter*	CreateViewFilter(Rule* pRule,FeatureClass* pFeatureClass);
		GFilter*	CreateViewFilter(GFilter* pSubFilter,FeatureClass* pFeatureClass);

		void		DrawFeatures(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor);
		void		DrawFeatures(LineSymbolizer* pSymbolizer, FeatureCursor* pCursor);
		void		DrawFeatures(PolygonSymbolizer* pSymbolizer, FeatureCursor* pCursor);
		void		DrawGraphics(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor);
		void		DrawPoints(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor);

	private:
		void		Cleanup();

	private:
		g_uint			m_width;
		g_uint			m_height;
		GEnvelope		m_viewer;
		Renderer		*m_pRenderer;
		TransformationImpl	m_transform;

		Maplex			m_maplex;
	};	
}

#endif //__AUGE_CANVAS_IMPL_H__
