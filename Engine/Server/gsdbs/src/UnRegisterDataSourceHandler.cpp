#include "UnRegisterDataSourceHandler.h"
#include "UnRegisterDataSourceRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	UnRegisterDataSourceHandler::UnRegisterDataSourceHandler()
	{

	}

	UnRegisterDataSourceHandler::~UnRegisterDataSourceHandler()
	{

	}

	const char*	UnRegisterDataSourceHandler::GetName()
	{
		return "UnRegisterDataSource";
	}

	const char*	UnRegisterDataSourceHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	UnRegisterDataSourceHandler::ParseRequest(rude::CGI& cgi)
	{
		UnRegisterDataSourceRequest* pRequest = new UnRegisterDataSourceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	UnRegisterDataSourceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UnRegisterDataSourceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* UnRegisterDataSourceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		const char* name = NULL;
		WebResponse* pWebResponse = NULL;
		UnRegisterDataSourceRequest* pRequest = static_cast<UnRegisterDataSourceRequest*>(pWebRequest);

		name = pRequest->GetName();
		if(name==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("DataSource Name not Defined");
			return pExpResponse;
		}

		ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
		RESULTCODE rc = pConnManager->Unregister(pUser->GetID(), name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* UnRegisterDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}