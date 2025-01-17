#ifndef __AUGE_FEATURE_IMPORT_HANDLER_H__
#define __AUGE_FEATURE_IMPORT_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "GProcessHandler.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class FeatureImportRequest;
	class FeatureClass;

	class FeatureImportHandler : public GProcessHandler
	{
	public:
		FeatureImportHandler();
		virtual ~FeatureImportHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		WebResponse*	ImportShapeFile(const char* shp_path, const char* shp_name, const char* source_name, const char* type_name, User* pUser);
		WebResponse*	ImportZipedShapeFile(const char* zip_path, const char* zip_name, const char* source_name, const char* type_name, User* pUser);
	};
}

#endif //__AUGE_FEATURE_IMPORT_HANDLER_H__
