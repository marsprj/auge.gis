#ifndef __AUGE_WMS_GETMAP_HANDLER_H__
#define __AUGE_WMS_GETMAP_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class WebContext;
	class GetMapRequest;
	class GetMapResponse;
	class Canvas;
	class Layer; 
	class Style;

	class GetMapHandler : public WebHandler
	{
	public:
		GetMapHandler();
		virtual ~GetMapHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	private:
		WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		//virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		void	DrawLayer(Canvas* pCanvas, Layer* pLayer, Style *pStyle);
		void	DrawNamedLayer(Canvas* pCanvas, Layer* pLayer, const char* style_name);
	};
}

#endif //__AUGE_WMS_GETMAP_HANDLER_H__

