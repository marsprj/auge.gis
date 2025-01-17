#include "GetTileHandler.h"
#include "GetTileRequest.h"
#include "GetTileResponse.h"
#include "WTileEngine.h"
#include "AugeService.h"
#include "AugeTile.h"
#include "AugeUser.h"

namespace auge
{
	GetTileHandler::GetTileHandler()
	{

	}

	GetTileHandler::~GetTileHandler()
	{

	}

	const char*	GetTileHandler::GetName()
	{
		return "GetTile";
	}

	const char*	GetTileHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetTileHandler::ParseRequest(rude::CGI& cgi)
	{
		GetTileRequest* pRequest = new GetTileRequest();
		if(!pRequest->Create(cgi))
		{ 
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->SetHost(getenv("HTTP_HOST"));
		pRequest->SetRequestMethod(getenv("REQUEST_METHOD"));
		return pRequest;
	}

	WebRequest*	GetTileHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		WebRequest* pWebRequest = ParseRequest(cgi);
		if(pWebRequest!=NULL)
		{
			GetTileRequest* pWRequest = static_cast<GetTileRequest*>(pWebRequest);
			pWRequest->SetMapName(mapName);
		}
		return pWebRequest;
	}

	WebRequest*	GetTileHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetTileRequest* pRequest = new GetTileRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetTileHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;

		return pWebResponse;
	}

	WebResponse* GetTileHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GetTileRequest* pRequest = static_cast<GetTileRequest*>(pWebRequest);
		GLogger* pLogger = augeGetLoggerInstance();

		const char* sourceName = pRequest->GetMapName();
		if(sourceName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No Map is attached");
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg,"[SourceName]:%s",sourceName);
		pLogger->Debug(msg,__FILE__,__LINE__);
		const char* storeName = pRequest->GetStoreName();
		if(storeName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No Layer is defined");
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		
		Tile* pTile = NULL;
		TileStore* pTileStore = NULL;
		TileWorkspace* pTileWorkspace = NULL;
		ConnectionManager *pConnManager = augeGetConnectionManagerInstance();
		
		pTileWorkspace = dynamic_cast<TileWorkspace*>(pConnManager->GetWorkspace(pUser->GetID(), sourceName));
		if(pTileWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "Cannot Connect to DataSource [%s]",sourceName);
			pLogger->Error(msg,__FILE__,__LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();			
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		long ts = auge_get_time();
		pTileStore = pTileWorkspace->OpenTileStore(storeName);
		if(pTileStore==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "Cannot Load to Layer [%s]",storeName);
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();			
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		long te = auge_get_time();
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_snprintf(msg, AUGE_MSG_MAX, "[Open Tile Store time]:%d\tms",(te-ts)/1000);
			pLogger->Info(msg, __FILE__, __LINE__);
		}
		
		ts = auge_get_time();
		g_int level = pRequest->GetLevel();
		g_int row = pRequest->GetRow();
		g_int col = pRequest->GetCol();
		pTile = pTileStore->GetTile(level, row, col);
		if(pTile==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Tile [%d-%d-%d] not found in Matrix [%s]",level, row, col,storeName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Debug(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();			
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		pTileStore->Release();
		te = auge_get_time();
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_snprintf(msg, AUGE_MSG_MAX, "[Get Tile Store time]:%d\tms",(te-ts)/1000);
			pLogger->Info(msg, __FILE__, __LINE__);
		}

		GetTileResponse* pResponse = new GetTileResponse(pRequest);
		pResponse->SetTile(pTile);

		return pResponse;
	}

}