#include "WebContextImpl.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeData.h"

namespace auge
{
	WebContext*	augeGetWebContextInstance()
	{
		static WebContextImpl g_webContext;
		return &g_webContext;
	}

	WebContextImpl::WebContextImpl()
	{
		m_name = "world";
		m_cache_path = "G:\\temp\\map\\";
		m_pConnection = NULL;
		m_response_encoding = "GBK";
		m_uri = "http://www.radi.ac.cn";
		m_isRecordRequest = true;
	}

	WebContextImpl::~WebContextImpl()
	{
		Unload();
	}
	
	RESULTCODE WebContextImpl::Initialize(const char* config_path) 
	{
		RESULTCODE rc = AG_FAILURE;
		rc = LoadConfig(config_path);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		InitializeCache();

		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::LoadConfig(const char* config_path)
	{
		if(config_path!=NULL)
			m_path = config_path;
		else
			MakeConfigPath();

		XParser parser;
		XDocument* pxDoc = NULL;
		pxDoc = parser.Parse(m_path.c_str());
		if(pxDoc==NULL)
		{
			return AG_FAILURE;
		}

		XNode *pxRoot = NULL;
		pxRoot = pxDoc->GetRootNode();
		if(pxRoot==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();
			return AG_FAILURE;
		}

		LoadStore(pxRoot);
		LoadLog(pxRoot);
		
		pxDoc->Close();
		pxDoc->Release();
		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::LoadStore(XNode* pxRoot)
	{
		XNode *pxStorage = NULL;
		pxStorage = pxRoot->GetFirstChild("Store");
		if(pxStorage==NULL)
		{
			return AG_FAILURE;
		}

		const char* nodeName = NULL;
		XNode *pxNode = NULL;
		XNodeSet* pxNodeSet = NULL;
		pxNodeSet = pxStorage->GetChildren();
		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			nodeName = pxNode->GetName();
			if(g_stricmp(nodeName, "Engine")==0)
			{
				if(pxNode->GetContent()!=NULL)
				{
					m_data_engine = pxNode->GetContent();
				}
			}
			else if(g_stricmp(nodeName, "URI")==0)
			{
				if(pxNode->GetContent()!=NULL)
				{
					m_data_uri = pxNode->GetContent();
				}
			}
		}

		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::LoadLog(XNode* pxRoot)
	{
		XNode *pxLog = NULL;
		pxLog = pxRoot->GetFirstChild("Log");
		if(pxLog==NULL)
		{
			return AG_FAILURE;
		}

		const char* value = pxLog->GetContent();
		if(value==NULL)
		{
			m_isRecordRequest = false;
		}
		if(strlen(value)==0)
		{
			m_isRecordRequest = false;
		}
		if(value[0]=='T'||value[0]=='t')
		{
			m_isRecordRequest = true;
		}
		else
		{
			m_isRecordRequest = false;
		}
		
		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::Unload()
	{
		if(m_pConnection!=NULL)
		{
			m_pConnection->Close();
			m_pConnection->Release();
			m_pConnection = NULL;
		}
		return AG_SUCCESS;
	}

	void WebContextImpl::MakeConfigPath()
	{
		char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(path, NULL, cdir, "conf\\service", "xml");
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(path, NULL, pdir, "conf/service", "xml");
#endif
		m_path = path;
	}

	const char*	WebContextImpl::GetService()
	{
		return m_name.c_str();
	}

	void WebContextImpl::SetService(const char* named) 
	{
		if(named==NULL)
		{
			m_name.clear();
		}
		m_name = named;
	}

	void WebContextImpl::SetURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_uri.clear();
		}
		else
		{
			m_uri = uri;
		}
	}

	const char* WebContextImpl::GetURI()
	{
		return m_uri.c_str();
	}

	const char*	WebContextImpl::GetCachePath()
	{
		return m_cache_path.c_str();
	}

	const char* WebContextImpl::GetCacheMapPath()
	{
		return m_cache_map_path.c_str();
	}

	const char*	WebContextImpl::GetCacheProtocolPath()
	{
		return m_cache_protocol_path.c_str();
	}

	const char*	WebContextImpl::GetThumbnailPath()
	{
		return m_cache_thumbnail_path.c_str();
	}

	const char* WebContextImpl::GetUploadPath()
	{
		return m_upload_path.c_str();
	}

	const char* WebContextImpl::GetUserRoot()
	{
		return m_user_root.c_str();
	}

	RESULTCODE WebContextImpl::GetUserPath(const char* user, char* user_path, g_uint size)
	{
		if(user_path==NULL)
		{
			return AG_FAILURE;
		}
		auge_make_path(user_path, NULL, m_user_root.c_str(), user, NULL);
		if(g_access(user_path, 4))
		{
			auge_mkdir(user_path); 
		}
		return true;
	}

	GConnection* WebContextImpl::GetConnection()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		if(m_pConnection==NULL)
		{
			DataEngine* pEngine = NULL;
			DataEngineManager* pDataEngineManager = NULL;
			pDataEngineManager = augeGetDataEngineManagerInstance();
			pEngine = pDataEngineManager->GetEngine(m_data_engine.c_str());
			if(pEngine==NULL)
			{
				pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			}
			else
			{
				m_pConnection = pEngine->CreateConnection();
			}
		}
		if(m_pConnection==NULL)
		{
			return NULL;
		}
		if(!m_pConnection->IsOpen())
		{
			m_pConnection->SetConnectionString(m_data_uri.c_str());
			m_pConnection->Open();
		}
		return m_pConnection;
	}

	const char* WebContextImpl::GetResponseEncoding()
	{
		return m_response_encoding.c_str();
	}

	const char*	WebContextImpl::GetServer()
	{
		const char* val = getenv("SERVER_NAME");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetPort()
	{
		const char* val = getenv("SERVER_PORT");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetScrpitName()
	{
		const char* val = getenv("SCRIPT_NAME");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetServerSoftware()
	{
		const char* val = getenv("SERVER_SOFTWARE");
		return (val==NULL) ? "" : val;
	} 

	const char*	WebContextImpl::GetHttpUserAgent()
	{
		const char* val = getenv("HTTP_USER_AGENT");
		return (val==NULL) ? "" : val;
	}

	const char* WebContextImpl::GetRequestMethod()
	{
		const char* val = getenv("REQUEST_METHOD");
		return (val==NULL) ? "" : val;
	}

	const char* WebContextImpl::GetQueryString()
	{
		const char* val = getenv("QUERY_STRING");
		return (val==NULL) ? "" : val;
	}

	const char* WebContextImpl::GetHttpClient()
	{
		//const char* val = getenv("HTTP_X_FORWARDED_FOR");
		//return (val==NULL) ? "127.0.0.1" : val;

		const char* remote_addr = getenv("HTTP_X_FORWARDED_FOR");//getenv("REMOTE_ADDR");
		if(remote_addr==NULL)
		{
			m_client_ip = "127.0.0.1" ;
		}
		else
		{
			if(strlen(remote_addr)==0)
			{
				m_client_ip = "127.0.0.1";
			}
			else
			{
				const char *ptr = strstr(remote_addr, ",");
				if(ptr==NULL)
				{
					m_client_ip = remote_addr;
				}
				else
				{
					std::string str = remote_addr;
					m_client_ip = str.substr(0,str.find_first_of(","));
				}

			}
		}
		return m_client_ip.c_str();
	}

	void WebContextImpl::InitializeCache()
	{
		//char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		char cache_home[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(cache_home, NULL, cdir, "cache", NULL);
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(cache_home, NULL, pdir, "cache", NULL);
#endif
		m_cache_path = cache_home;
		if(g_access(cache_home, 4))
		{
			auge_mkdir(cache_home);
		}

		// protocol cache path
		char cache_protocol[AUGE_PATH_MAX];
		auge_make_path(cache_protocol, NULL, cache_home, "protocol", NULL);
		if(g_access(cache_protocol, 4))
		{
			auge_mkdir(cache_protocol);
		}
		m_cache_protocol_path = cache_protocol;

		// map cache path
		char cache_map[AUGE_PATH_MAX];
		auge_make_path(cache_map, NULL, cache_home, "map", NULL);
		if(g_access(cache_map, 4))
		{
			auge_mkdir(cache_map);
		}
		m_cache_map_path = cache_map;

		// thumbnail cache
		char cache_thumbnail[AUGE_PATH_MAX];
		auge_make_path(cache_thumbnail, NULL, cache_home, "thumbnail", NULL);
		if(g_access(cache_thumbnail, 4))
		{
			auge_mkdir(cache_thumbnail);
		}
		m_cache_thumbnail_path = cache_thumbnail;

		// Upload Path
		char upload_path[AUGE_PATH_MAX];
#ifdef WIN32
		auge_make_path(upload_path, NULL, cdir, "upload", NULL);
#else
		auge_make_path(upload_path, NULL, pdir, "upload", NULL);
#endif 
		if(g_access(upload_path, 4))
		{
			auge_mkdir(upload_path); 
		}
		m_upload_path = upload_path;		

		// User Path
		char user_path[AUGE_PATH_MAX];
#ifdef WIN32
		auge_make_path(user_path, NULL, cdir, "user", NULL);
#else
		auge_make_path(user_path, NULL, pdir, "user", NULL);
#endif 
		if(g_access(user_path, 4))
		{
			auge_mkdir(user_path); 
		}
		m_user_root = user_path;		
	}

	 bool WebContextImpl::IsIE()
	 {
		 const char* http_user_agent = getenv("HTTP_USER_AGENT"); 
		 if(http_user_agent==NULL)
		 {

#ifdef WIN32
			 return true;
#else
			 return false;
#endif
		 }
		 const char* ptr = strstr(http_user_agent, "MSIE");
		 return (ptr!=NULL);
	 }

	 const char* WebContextImpl::ParameterEncoding(const char* value)
	 {
		 WebContext* pWebContext = augeGetWebContextInstance();
		 if(IsIE())
		 {
			 return value;
		 }

		 return auge_encoding_convert(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, value, strlen(value));
	 }

	 RESULTCODE	WebContextImpl::RecordUserRequest(const char* user_name, const char* user_request, const char* request_service, const char* http_request)
	 {
		 if(!m_isRecordRequest)
		 {
			 return AG_SUCCESS;
		 }
		 const char* http_method = GetRequestMethod();
		 const char* http_client = GetHttpClient();
		 const char* format = "insert into g_user_request (user_name,user_request,request_service, request_time,http_method,http_client,http_request) values('%s','%s','%s',now(),'%s','%s','%s')";

		 char sql[AUGE_SQL_MAX];
		 memset(sql, 0, AUGE_SQL_MAX);
		 g_snprintf(sql, AUGE_SQL_MAX, format, user_name, user_request, request_service, http_method, http_client, http_request);
		 return m_pConnection->ExecuteSQL(sql);
	 }
}
