#ifndef __AUGE_IMS_GET_COLORMAP_REQUEST_H__
#define __AUGE_IMS_GET_COLORMAP_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetColorMapRequest : public WebRequest
	{
	public:
		GetColorMapRequest();
		virtual ~GetColorMapRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual g_uint			GetCount();
		virtual g_int			GetID();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

	public:
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		void		SetID(const char* id);
		void		SetCount(const char* count);

		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;
		g_int m_id;
		g_uint m_count;

		std::string m_name;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_IMS_GET_COLORMAP_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetColorMap
http://127.0.0.1:8088/ows/admin/mgr?service=ims&version=1.0.0&request=GetColorMap
http://192.168.111.160:8088/ows/admin/mgr?service=ims&version=1.0.0&request=GetColorMap
http://123.57.207.198/ows/admin/mgr?service=ims&version=1.0.0&request=GetColorMap

************************************************************************/
