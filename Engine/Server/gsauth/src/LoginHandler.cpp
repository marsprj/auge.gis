#include "LoginHandler.h"
#include "LoginRequest.h"
//#include "LoginResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	LoginHandler::LoginHandler()
	{

	}

	LoginHandler::~LoginHandler()
	{

	}

	const char*	LoginHandler::GetName()
	{
		return "Login";
	}

	const char*	LoginHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	LoginHandler::ParseRequest(rude::CGI& cgi)
	{
		LoginRequest* pRequest = new LoginRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	LoginHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	LoginHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//LoginRequest* pRequest = new LoginRequest();
		////if(!pRequest->Create(pxDoc, pMap))
		//if(!pRequest->Create(pxDoc))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	return NULL;
		//}
		//return pRequest;
		return NULL;
	}

	WebResponse* LoginHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GLogger* pLogger = augeGetLoggerInstance();
		LoginRequest* pRequest = static_cast<LoginRequest*>(pWebRequest);
		const char* name = pRequest->GetName();
		const char* pswd = pRequest->GetPassword();
		const char* remote = pRequest->GetRemoteAddress();

		if(name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}
		if(pswd==name)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		User* pdbUser = NULL;
		UserManager* pUserManager = augeGetUserManagerInstance();
		pdbUser = pUserManager->GetUser(name);
		if(pdbUser==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "�û�[%s]������", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		if(pdbUser->GetPassword()==NULL)
		{	
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "�û�[%s]�������", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			pdbUser->Release();
			return pExpResponse;
		}
		pdbUser->Release();
		
		//if(strcmp(pdbUser->GetPassword(), pswd))
		//{
		//	const char* msg = "�������";
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	pdbUser->Release();
		//	return pExpResponse;
		//}
		RESULTCODE rc = pUserManager->Login(name, pswd, remote);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* LoginHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		LoginRequest* pRequest = static_cast<LoginRequest*>(pWebRequest);
		const char* name = pRequest->GetName();
		const char* pswd = pRequest->GetPassword();
		const char* remote = pRequest->GetRemoteAddress();

		if(name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}
		if(pswd==name)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		User* pdbUser = NULL;
		UserManager* pUserManager = augeGetUserManagerInstance();
		pdbUser = pUserManager->GetUser(name);
		if(pdbUser==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "�û�[%s]������", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		if(pdbUser->GetPassword()==NULL)
		{	
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "�û�[%s]�������", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			pdbUser->Release();
			return pExpResponse;
		}
		pdbUser->Release();

		//if(strcmp(pdbUser->GetPassword(), pswd))
		//{
		//	const char* msg = "�������";
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	pdbUser->Release();
		//	return pExpResponse;
		//}
		RESULTCODE rc = pUserManager->Login(name, pswd, remote);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return pExpResponse;
		}

		char user_root[AUGE_PATH_MAX];
		memset(user_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_root, NULL, pWebContext->GetUserRoot(), name, NULL);
		if(g_access(user_root, 4))
		{
			auge_mkdir(user_root);
		}

		char user_file_root[AUGE_PATH_MAX];
		memset(user_file_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_file_root, NULL, user_root, "upload", NULL);
		if(g_access(user_file_root, 4))
		{
			auge_mkdir(user_file_root);
		}

		char user_raster_root[AUGE_PATH_MAX];
		memset(user_raster_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_raster_root, NULL, user_root, "rds", NULL);
		if(g_access(user_raster_root, 4))
		{
			auge_mkdir(user_raster_root);
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}