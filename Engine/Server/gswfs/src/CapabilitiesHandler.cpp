#include "CapabilitiesHandler.h"
#include "CapabilitiesRequest.h"
#include "CapabilitiesResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	WFSCapabilitiesHandler::WFSCapabilitiesHandler()
	{

	}

	WFSCapabilitiesHandler::~WFSCapabilitiesHandler()
	{

	}

	const char*	WFSCapabilitiesHandler::GetName()
	{
		return "GetCapabilities";
	}

	const char*	WFSCapabilitiesHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	WFSCapabilitiesHandler::ParseRequest(rude::CGI& cgi)
	{
		WFSCapabilitiesRequest* pRequest = new WFSCapabilitiesRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
			return NULL;
		}
		pRequest->SetHost(getenv("HTTP_HOST"));
		pRequest->SetRequestMethod(getenv("REQUEST_METHOD"));
		return pRequest;
	}

	WebRequest*	WFSCapabilitiesHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		WFSCapabilitiesRequest* pRequest = new WFSCapabilitiesRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->SetHost(getenv("HTTP_HOST"));
		pRequest->SetRequestMethod(getenv("REQUEST_METHOD"));
		pRequest->SetMapName(mapName,true);
		return pRequest;
	}

	//WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	CapabilitiesRequest* pRequest = new CapabilitiesRequest();
	//	if(!pRequest->Create(cgi))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//		return NULL;
	//	}
	//	pRequest->SetHost(getenv("HTTP_HOST"));
	//	pRequest->SetRequestMethod(getenv("REQUEST_METHOD"));
	//	return pRequest;
	//}

	WebRequest*	WFSCapabilitiesHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		WFSCapabilitiesRequest* pRequest = new WFSCapabilitiesRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* WFSCapabilitiesHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		WFSCapabilitiesRequest* pRequest = static_cast<WFSCapabilitiesRequest*>(pWebRequest);
		
		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.1.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* WFSCapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		WFSCapabilitiesRequest* pRequest = static_cast<WFSCapabilitiesRequest*>(pWebRequest);

		if(pRequest->IsValidSource())
		{
			pWebResponse = ExecuteBySource(pRequest, pWebContext, pUser);
		}
		else
		{
			pWebResponse = ExecuteByMap(pRequest, pWebContext, pUser);
		}

		return pWebResponse;
	}



	//WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	WebResponse* pWebResponse = NULL;
	//	CapabilitiesRequest* pRequest = static_cast<CapabilitiesRequest*>(pWebRequest);

	//	const char* version = pRequest->GetVersion();
	//	if(strcmp(version,"1.0.0")==0)
	//	{
	//		pWebResponse = WriteCapabilities_1_0_0(pRequest, pWebContext, pMap);
	//	}
	//	else if(strcmp(version,"1.1.0")==0)
	//	{
	//		pWebResponse = WriteCapabilities_1_1_0(pRequest, pWebContext, pMap);
	//	}
	//	else
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		g_sprintf(msg, "WMS does not support %s",version);
	//		pExpResponse->SetMessage(msg);
	//		pWebResponse = pExpResponse;
	//	}

	//	return pWebResponse;
	//}
	
	//////////////////////////////////////////////////////////////////////////
	// ExecuteByMap
	//////////////////////////////////////////////////////////////////////////
	WebResponse* WFSCapabilitiesHandler::ExecuteByMap(WFSCapabilitiesRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		const char* mapName = pWebRequest->GetMapName();
		if(mapName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No Map is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* version = pWebRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteCapabilities_1_0_0(pWebRequest, pWebContext, pMap);
		}
		else if(strcmp(version,"1.1.0")==0)
		{
			pWebResponse = WriteCapabilities_1_1_0(pWebRequest, pWebContext, pMap);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}
		pMap->Release();
		return pWebResponse;
	}

	// 1.1.0 in fact
	WFSCapabilitiesResponse* WFSCapabilitiesHandler::WriteCapabilities_1_0_0(WFSCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap) 
	{
		return WriteCapabilities_1_1_0(pRequest, pWebContext, pMap);
	}

	//CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_1_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	const char* cache_path = pWebContext->GetCacheProtocolPath();
	//	const char* ns = pWebContext->GetService() ? pWebContext->GetService() : pMap->GetName();

	//	char wfs_xlink[AUGE_MSG_MAX];
	//	g_sprintf(wfs_xlink, "http://%s/%s/%s/%s/ims?service=wfs",	pRequest->GetHost(),
	//																AUGE_VIRTUAL_NAME,
	//																pRequest->GetUser(),
	//																ns); 
	//	 
	//	char str[AUGE_MSG_MAX];
	//	char capa_path[AUGE_PATH_MAX];
	//	auge_make_path(capa_path, NULL, cache_path, "wfs_capabilities_1_1_0","xml");

	//	XElement  *pxNode = NULL; 
	//	XElement  *pxRoot = NULL;
	//	XDocument *pxDoc = new XDocument();
	//	pxRoot = pxDoc->CreateRootNode("WFS_Capabilities", NULL, NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
	//	pxRoot->SetAttribute("version", "1.1.0", NULL);

	//	// WFS_Capabilities-->ServiceIdentification
	//	XElement *pxService = pxRoot->AddChild("ServiceIdentification", "ows");
	//	pxNode = pxNode = pxService->AddChild("Name", "ows");
	//	pxNode->SetChildText("WFS");
	//	pxNode = pxNode = pxService->AddChild("Title", "ows");
	//	pxNode->SetChildText("Auge Web Map Service");
	//	pxNode = pxNode = pxService->AddChild("Abstract", "ows");
	//	// WFS_Capabilities-->ServiceIdentification-->KeywordList
	//	XElement *pxKeywords = pxService->AddChild("KeywordList", "ows");
	//	pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
	//	pxNode->SetChildText("WFS");
	//	pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
	//	pxNode->SetChildText("AugeGIS");
	//	// WFS_Capabilities-->ServiceIdentification-->ServiceType
	//	pxNode = pxService->AddChild("ServiceType", "ows");
	//	pxNode->SetChildText("WFS");
	//	pxNode = pxService->AddChild("ServiceVersion", "ows");
	//	pxNode->SetChildText("1.1.0");
	//	// WFS_Capabilities-->ServiceIdentification-->Fees
	//	XElement *pxFees = pxService->AddChild("Fees", "ows");
	//	pxFees->SetChildText("NONE");
	//	// WFS_Capabilities-->ServiceIdentification-->AccessConstraints
	//	XElement *pxConstraints = pxService->AddChild("AccessConstraints", "ows");
	//	pxConstraints->SetChildText("NONE");

	//	// WFS_Capabilities-->ServiceProvider
	//	XElement* pxProvider = pxRoot->AddChild("ServiceProvider", "ows");
	//	XElement* pxProviderName = pxProvider->AddChild("ProviderName","ows");
	//	pxConstraints->SetChildText("Auge GIS");
	//	XElement* pxServiceContact = pxProvider->AddChild("ServiceContact","ows");
	//	pxNode = pxServiceContact->AddChild("IndividualName","ows");
	//	pxNode = pxServiceContact->AddChild("PositionName","ows");
	//	XElement* pxContactInfo = pxServiceContact->AddChild("PositionName","ows");
	//	XElement* pxPhone = pxContactInfo->AddChild("Phone","ows");
	//	pxNode = pxPhone->AddChild("Voice","ows");
	//	pxNode = pxPhone->AddChild("Facsimile","ows");
	//	XElement* pxAddress = pxContactInfo->AddChild("Address","ows");
	//	pxNode = pxAddress->AddChild("City","ows");
	//	pxNode = pxAddress->AddChild("AdministrativeArea","ows");
	//	pxNode = pxAddress->AddChild("Country","ows");

	//	// WFS_Capabilities-->OperationsMetadata
	//	XElement* pxOperationsMetadata = pxRoot->AddChild("OperationsMetadata", "ows");

	//	// Service Handlers
	//	WFeatureEngine* pFeatureEngine = (WFeatureEngine*)augeGetWebEngineInstance();
	//	std::vector<WebHandler*>& handlers = pFeatureEngine->m_handlers;
	//	std::vector<WebHandler*>::iterator iter;
	//	for(iter=handlers.begin(); iter!=handlers.end(); iter++)
	//	{
	//		WebHandler* handler = *iter;

	//		// WFS_Capabilities-->OperationsMetadata-->Operation (GetCapabilities)
	//		XElement* pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
	//		pxOperation->SetAttribute("name", handler->GetName(),NULL);
	//		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
	//		XElement* pxDCP = pxOperation->AddChild("DCP", "ows");
	//		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
	//		XElement* pxHTTP = pxDCP->AddChild("HTTP", "ows");
	//		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
	//		pxNode = pxHTTP->AddChild("Get", "ows");
	//		pxNode->SetAttribute("href",wfs_xlink,"xlink");
	//		pxNode = pxHTTP->AddChild("Post", "ows");
	//		pxNode->SetAttribute("href",wfs_xlink,"xlink");
	//		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(AcceptVersion)
	//		XElement* pxParameter = pxOperation->AddChild("Parameter", "ows");
	//		pxNode = pxParameter->AddChild("Value", "ows");
	//		pxNode->AddChildText("1.0.0");
	//		pxNode = pxParameter->AddChild("Value", "ows");
	//		pxNode->AddChildText("1.1.0");
	//		pxParameter = pxOperation->AddChild("AcceptFormats", "ows");
	//		pxNode = pxParameter->AddChild("Value", "ows");
	//		pxNode->AddChildText("text/xml");
	//	}

	//	// WFS_Capabilities-->FeatureTypeList 
	//	XElement* pxTypeList = pxRoot->AddChild("FeatureTypeList", "ows");
	//	XElement* pxOperations = pxTypeList->AddChild("Operations", "ows");
	//	XElement* pxOperation = pxOperations->AddChild("Operation", "ows");
	//	pxOperation->AddChildText("Query");
	//	pxOperation = pxOperations->AddChild("Operation", "ows");
	//	pxOperation->AddChildText("Insert");
	//	pxOperation = pxOperations->AddChild("Operation", "ows");
	//	pxOperation->AddChildText("Update");
	//	pxOperation = pxOperations->AddChild("Operation", "ows");
	//	pxOperation->AddChildText("Delete");
	//	pxOperation = pxOperations->AddChild("Operation", "ows");
	//	pxOperation->AddChildText("Lock");

	//	// WFS_Capabilities-->FeatureTypeList->FeatureType
	//	Layer* pLayer = NULL;
	//	g_uint lc = pMap->GetLayerCount();
	//	for(g_uint i=0; i<lc; i++)
	//	{
	//		pLayer = pMap->GetLayer(i);
	//		if(pLayer!=NULL)
	//		{
	//			if(pLayer->GetType()==augeLayerFeature)
	//			{
	//				FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
	//				FeatureClass* pFeatureClass = pFLayer->GetFeatureClass();
	//				if(pFeatureClass!=NULL)
	//				{
	//					XElement* pxType = pxTypeList->AddChild("FeatureType", NULL);
	//					pxNode = pxType->AddChild("Name",NULL);
	//					pxNode->SetChildText(pFLayer->GetName());
	//					pxNode = pxType->AddChild("Tile",NULL);
	//					pxNode->SetChildText(pFeatureClass->GetName());
	//					pxNode = pxType->AddChild("Abstract",NULL);

	//					//Keywords
	//					pxKeywords = pxType->AddChild("KeywordList", "ows");
	//					pxNode = pxKeywords->AddChild("Keyword", "ows");
	//					pxNode->SetChildText(pFeatureClass->GetName());

	//					//DefaultSRS
	//					g_snprintf(str, AUGE_MSG_MAX, "EPSG:%d", pFeatureClass->GetSRID());
	//					pxNode = pxType->AddChild("DefaultSRS", "ows");
	//					pxNode->SetChildText(str);

	//					//WGS84BoundingBox
	//					GEnvelope& extent = pFeatureClass->GetExtent();
	//					XElement* px84Bounding = pxType->AddChild("WGS84BoundingBox", "ows");
	//					pxNode = px84Bounding->AddChild("LowerCorner","ows");
	//					if(extent.IsValid())
	//					{
	//						g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmin, extent.m_ymin);
	//						pxNode->AddChildText(str);
	//					}
	//					pxNode = px84Bounding->AddChild("UpperCorner","ows");
	//					if(extent.IsValid())
	//					{
	//						g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmax, extent.m_ymax);
	//						pxNode->AddChildText(str);
	//					}
	//				}
	//			}
	//		} 
	//	}

	//	// WFS_Capabilities-->OGC_FilterCapabilities
	//	XElement* pxFilter = pxRoot->AddChild("OGC_FilterCapabilities","ogc");

	//	pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
	//	pxDoc->Release();

	//	CapabilitiesResponse* pResponse = new CapabilitiesResponse(pRequest);
	//	pResponse->SetPath(capa_path);

	//	return pResponse;
	//}
	

	WFSCapabilitiesResponse* WFSCapabilitiesHandler::WriteCapabilities_1_1_0(WFSCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap)
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		const char* ns = pWebContext->GetService() ? pWebContext->GetService() : pMap->GetName();

		char wfs_xlink[AUGE_MSG_MAX];
		g_sprintf(wfs_xlink, "http://%s/%s/%s/%s/ims?service=wfs",	pRequest->GetHost(),
			AUGE_VIRTUAL_NAME,
			pRequest->GetUser(),
			ns); 

		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wfs_capabilities_1_1_0","xml");

		XElement  *pxNode = NULL; 
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WFS_Capabilities", NULL, NULL);
		SetRooteNode_1_1_0(pxRoot, "1.1.0");

		// WFS_Capabilities-->ServiceIdentification
		AddServiceIdentificationNode_1_1_0(pxRoot);

		// WFS_Capabilities-->ServiceProvider
		AddServiceProviderNode_1_1_0(pxRoot);

		// WFS_Capabilities-->OperationsMetadata
		AddOperationsMetadataNode_1_1_0(pxRoot, wfs_xlink);

		// WFS_Capabilities-->FeatureTypeList 
		XElement* pxTypeList = AddFeatureTypeListNode_1_1_0(pxRoot); 
		

		// WFS_Capabilities-->FeatureTypeList->FeatureType
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				if(pLayer->GetType()==augeLayerFeature)
				{ 
					FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);					
					FeatureClass* pFeatureClass = pFLayer->GetFeatureClass();
					if(pFeatureClass!=NULL)
					{
						AddFeatureTypeNode_1_1_0(pxTypeList, pFLayer->GetName(), pFeatureClass);
					}
				}
			} 
		}

		// WFS_Capabilities-->OGC_FilterCapabilities
		XElement* pxFilter = pxRoot->AddChild("OGC_FilterCapabilities","ogc");

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		WFSCapabilitiesResponse* pResponse = new WFSCapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}

	void WFSCapabilitiesHandler::SetRooteNode_1_1_0(XElement* pxRoot, const char* version)
	{
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", version, NULL);
	}

	void WFSCapabilitiesHandler::AddServiceIdentificationNode_1_1_0(XElement* pxParent)
	{	
		XElement *pxService = pxParent->AddChild("ServiceIdentification", "ows");
		XElement *pxNode = pxNode = pxService->AddChild("Name", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxNode = pxService->AddChild("Title", "ows");
		pxNode->SetChildText("Auge Web Map Service");
		pxNode = pxNode = pxService->AddChild("Abstract", "ows");
		// WFS_Capabilities-->ServiceIdentification-->KeywordList
		XElement *pxKeywords = pxService->AddChild("KeywordList", "ows");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("AugeGIS");
		// WFS_Capabilities-->ServiceIdentification-->ServiceType
		pxNode = pxService->AddChild("ServiceType", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxService->AddChild("ServiceVersion", "ows");
		pxNode->SetChildText("1.1.0");
		// WFS_Capabilities-->ServiceIdentification-->Fees
		XElement *pxFees = pxService->AddChild("Fees", "ows");
		pxFees->SetChildText("NONE");
		// WFS_Capabilities-->ServiceIdentification-->AccessConstraints
		XElement *pxConstraints = pxService->AddChild("AccessConstraints", "ows");
		pxConstraints->SetChildText("NONE");
	}

	void WFSCapabilitiesHandler::AddServiceProviderNode_1_1_0(XElement* pxParent)
	{
		XElement* pxProvider = pxParent->AddChild("ServiceProvider", "ows");
		XElement* pxProviderName = pxProvider->AddChild("ProviderName","ows");
		//pxConstraints->SetChildText("Auge GIS");
		XElement* pxServiceContact = pxProvider->AddChild("ServiceContact","ows");
		XElement* pxNode = pxServiceContact->AddChild("IndividualName","ows");
		pxNode = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxContactInfo = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxPhone = pxContactInfo->AddChild("Phone","ows");
		pxNode = pxPhone->AddChild("Voice","ows");
		pxNode = pxPhone->AddChild("Facsimile","ows");
		XElement* pxAddress = pxContactInfo->AddChild("Address","ows");
		pxNode = pxAddress->AddChild("City","ows");
		pxNode = pxAddress->AddChild("AdministrativeArea","ows");
		pxNode = pxAddress->AddChild("Country","ows");
	}

	void WFSCapabilitiesHandler::AddOperationsMetadataNode_1_1_0(XElement* pxParent, const char* wfs_xlink)
	{
		XElement* pxOperationsMetadata = pxParent->AddChild("OperationsMetadata", "ows");

		// Service Handlers
		WFeatureEngine* pFeatureEngine = (WFeatureEngine*)augeGetWebEngineInstance();
		std::vector<WebHandler*>& handlers = pFeatureEngine->m_handlers;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			WebHandler* handler = *iter;

			// WFS_Capabilities-->OperationsMetadata-->Operation (GetCapabilities)
			XElement* pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
			pxOperation->SetAttribute("name", handler->GetName(),NULL);
			// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
			XElement* pxDCP = pxOperation->AddChild("DCP", "ows");
			// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
			XElement* pxHTTP = pxDCP->AddChild("HTTP", "ows");
			// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
			XElement* pxNode = pxHTTP->AddChild("Get", "ows");
			pxNode->SetAttribute("href",wfs_xlink,"xlink");
			pxNode = pxHTTP->AddChild("Post", "ows");
			pxNode->SetAttribute("href",wfs_xlink,"xlink");
			// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(AcceptVersion)
			XElement* pxParameter = pxOperation->AddChild("Parameter", "ows");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("1.0.0");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("1.1.0");
			pxParameter = pxOperation->AddChild("AcceptFormats", "ows");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("text/xml");
		}
	}

	XElement* WFSCapabilitiesHandler::AddFeatureTypeListNode_1_1_0(XElement* pxParent)
	{
		XElement* pxTypeList = pxParent->AddChild("FeatureTypeList", "ows");
		XElement* pxOperations = pxTypeList->AddChild("Operations", "ows");
		XElement* pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Query");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Insert");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Update");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Delete");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Lock");
		return pxTypeList;
	}

	void WFSCapabilitiesHandler::AddFeatureTypeNode_1_1_0(XElement* pxParent, const char* typeName, FeatureClass* pFeatureClass)
	{
		char str[AUGE_NAME_MAX] = {0};
		XElement* pxType = pxParent->AddChild("FeatureType", NULL);
		XElement* pxNode = pxType->AddChild("Name",NULL);
		pxNode->SetChildText(typeName, true);
		pxNode = pxType->AddChild("Tile",NULL);
		pxNode->SetChildText(pFeatureClass->GetName(), true);
		pxNode = pxType->AddChild("Abstract",NULL);

		//Keywords
		XElement* pxKeywords = pxType->AddChild("KeywordList", "ows");
		pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText(pFeatureClass->GetName(), true);

		//DefaultSRS
		g_snprintf(str, AUGE_MSG_MAX, "EPSG:%d", pFeatureClass->GetSRID());
		pxNode = pxType->AddChild("DefaultSRS", "ows");
		pxNode->SetChildText(str);

		//WGS84BoundingBox			
		GEnvelope& extent = pFeatureClass->GetExtent();
		XElement* px84Bounding = pxType->AddChild("WGS84BoundingBox", "ows");
		pxNode = px84Bounding->AddChild("LowerCorner","ows");
		if(extent.IsValid())
		{
			g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmin, extent.m_ymin);
			pxNode->AddChildText(str);
		}
		pxNode = px84Bounding->AddChild("UpperCorner","ows");
		if(extent.IsValid())
		{
			g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmax, extent.m_ymax);
			pxNode->AddChildText(str);
		}
	}

	void WFSCapabilitiesHandler::AddAttributeDatasetsNode_1_1_0(XElement* pxParent, const char* name, AttributeDataSet* pattrDataset)
	{
		char str[AUGE_NAME_MAX] = {0};
		XElement* pxType = pxParent->AddChild("FeatureType", NULL);
		XElement* pxNode = pxType->AddChild("Name",NULL);
		pxNode->SetChildText(name);
		pxNode = pxType->AddChild("Tile",NULL);
		pxNode->SetChildText(pattrDataset->GetName());
		pxNode = pxType->AddChild("Abstract",NULL);

		//Keywords
		XElement* pxKeywords = pxType->AddChild("KeywordList", "ows");
		pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText(pattrDataset->GetName());		
	}
	
	WebResponse* WFSCapabilitiesHandler::ExecuteBySource(WFSCapabilitiesRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		const char* sourceName = pWebRequest->GetSourceName();
		if(sourceName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No Source is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		Workspace* pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* version = pWebRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteCapabilities_1_0_0(pWebRequest, pWebContext, pWorkspace);
		}
		else if(strcmp(version,"1.1.0")==0)
		{
			pWebResponse = WriteCapabilities_1_1_0(pWebRequest, pWebContext, pWorkspace);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}
		return pWebResponse;
	}

	WFSCapabilitiesResponse* WFSCapabilitiesHandler::WriteCapabilities_1_0_0(WFSCapabilitiesRequest* pRequest, WebContext* pWebContext, Workspace* pWorkspace)
	{
		return WriteCapabilities_1_1_0(pRequest, pWebContext, pWorkspace);
	}

	WFSCapabilitiesResponse* WFSCapabilitiesHandler::WriteCapabilities_1_1_0(WFSCapabilitiesRequest* pRequest, WebContext* pWebContext, Workspace* pWorkspace)
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		const char* ns = pWorkspace->GetName();

		char wfs_xlink[AUGE_MSG_MAX];
		g_sprintf(wfs_xlink, "http://%s/%s/%s/%s/ims?service=wfs",	pRequest->GetHost(),
			AUGE_VIRTUAL_NAME,
			pRequest->GetUser(),
			NULL); 

		char capa_name[AUGE_PATH_MAX];
		char capa_path[AUGE_PATH_MAX];
		g_sprintf(capa_name, "dbs_%s_capabilities_1_1_0", pWorkspace->GetName());
		auge_make_path(capa_path, NULL, cache_path, capa_name,"xml");

		XElement  *pxNode = NULL; 
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WFS_Capabilities", NULL, NULL);
		SetRooteNode_1_1_0(pxRoot, "1.1.0");

		// WFS_Capabilities-->ServiceIdentification
		AddServiceIdentificationNode_1_1_0(pxRoot);

		// WFS_Capabilities-->ServiceProvider
		AddServiceProviderNode_1_1_0(pxRoot);

		// WFS_Capabilities-->OperationsMetadata
		AddOperationsMetadataNode_1_1_0(pxRoot, wfs_xlink);

		// WFS_Capabilities-->FeatureTypeList 
		XElement* pxTypeList = AddFeatureTypeListNode_1_1_0(pxRoot); 

		// WFS_Capabilities-->FeatureTypeList->FeatureType
		DataSet* pDataSet = NULL;
		const char* datasetName = NULL;
		augeDataSetType type = augeDataSetAttribute;
		FeatureWorkspace* pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
		//EnumDataSet* pDataSets = pFeatureWorkspace->GetFeatureClasses();
		EnumDataSet* pDataSets = pFeatureWorkspace->GetDataSets();
		if(pDataSets!=NULL)
		{
			pDataSets->Reset();
			while((pDataSet=pDataSets->Next())!=NULL)
			{
				datasetName = pDataSet->GetName();
				type = pDataSet->GetType();

				switch(type)
				{
				case augeDataSetFeature:
					{
						FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataSet);
						AddFeatureTypeNode_1_1_0(pxTypeList, pFeatureClass->GetName(), pFeatureClass);
					}
					break;
				case augeDataSetAttribute:
					{
						AttributeDataSet* pattrDataset = static_cast<AttributeDataSet*>(pDataSet);
						AddAttributeDatasetsNode_1_1_0(pxTypeList, pattrDataset->GetName(), pattrDataset);
					}
					break;
				case augeDataSetRaster:
					break;
				}
			}

			pDataSets->Release();
		}

		// WFS_Capabilities-->OGC_FilterCapabilities
		XElement* pxFilter = pxRoot->AddChild("OGC_FilterCapabilities","ogc");

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		WFSCapabilitiesResponse* pResponse = new WFSCapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}
}
