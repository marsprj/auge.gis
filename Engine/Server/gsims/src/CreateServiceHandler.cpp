#include "CreateServiceHandler.h"
#include "CreateServiceRequest.h"
#include "CreateServiceResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	CreateServiceHandler::CreateServiceHandler()
	{

	}

	CreateServiceHandler::~CreateServiceHandler()
	{

	}

	const char*	CreateServiceHandler::GetName()
	{
		return "CreateService";
	}

	const char*	CreateServiceHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	CreateServiceHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateServiceRequest* pRequest = new CreateServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CreateServiceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* CreateServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		ServiceManager* pServiceManager = NULL;
		Service* pService = NULL;
		CreateServiceRequest* pRequest = static_cast<CreateServiceRequest*>(pWebRequest);

		g_uint user_id = pUser->GetID();
		const char* name = pRequest->GetName();
		const char* mapName = pRequest->GetMapName();
		const char* uri	 = pRequest->GetURI();
		pServiceManager = augeGetServiceManagerInstance();
		if(pServiceManager->Has(user_id, name))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s already exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pServiceManager->Register(user_id,name,mapName,uri);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("CreateService");
		return pSusResponse;
	}

	WebResponse* CreateServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}