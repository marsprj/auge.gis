#include "CapabilitiesResponse.h"
#include "CapabilitiesRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "WProcessEngine.h"

namespace auge
{
	CapabilitiesResponse::CapabilitiesResponse(CapabilitiesRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	CapabilitiesResponse::~CapabilitiesResponse()
	{

	}

	RESULTCODE CapabilitiesResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(m_path.c_str(), __FILE__, __LINE__);

		FILE* fp = fopen(m_path.c_str(), "rb");
		if(fp==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead(m_pRequest->GetMimeType());

		g_uint nBytes = 0;
		g_uchar buffer[AUGE_BUFFER_MAX];		
		memset(buffer, 0, AUGE_BUFFER_MAX);
		while((nBytes=fread(buffer, sizeof(g_uchar),AUGE_BUFFER_MAX, fp))>0)
		{
			pWriter->Write(buffer, nBytes);
		}

		pWriter->WriteTail();

		fclose(fp);

		return AG_SUCCESS;
	}

	void CapabilitiesResponse::SetPath(const char* path)
	{
		m_path = path;
	}

}