#include "ListRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	ListRequest::ListRequest()
	{
		m_version = "1.0.0"; 
		m_encoding = "GBK";
	}

	ListRequest::~ListRequest()
	{

	}

	bool ListRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetPath(cgi["Path"]);

		return true;
	}

	const char*	ListRequest::GetEngine()
	{
		return "rds";
	}

	const char*	ListRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	ListRequest::GetRequest()
	{
		return "List";
	}

	void ListRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	ListRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void ListRequest::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path = "/";
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			m_path = pWebContext->ParameterEncoding(path);
		}
	}
	
	const char* ListRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	void ListRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	const char* ListRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	const char*	ListRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}