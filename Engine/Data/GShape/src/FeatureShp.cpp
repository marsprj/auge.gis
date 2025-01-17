#include "FeatureShp.h"
#include "FeatureClassShp.h"
#include "ShpUtil.h"

namespace auge
{
	FeatureShp::FeatureShp():
	m_pFeatureClass(NULL),
	m_pGeometry(NULL),
	m_pFields(NULL),
	m_pSHPHandle(NULL),
	m_pDBFHandle(NULL)
	{

	}

	FeatureShp::~FeatureShp()
	{
		AUGE_SAFE_RELEASE(m_pFeatureClass);
		AUGE_SAFE_RELEASE(m_pGeometry);
	}

	FeatureClass* FeatureShp::GetFeatureClass() const
	{
		return m_pFeatureClass;
	}

	g_int FeatureShp::GetFID()
	{
		return m_fid;
	}

	Geometry* FeatureShp::GetGeometry()
	{
		if(m_pGeometry==NULL)
		{
			m_pGeometry = CreateGeometry(m_fid, m_pFeatureClass->m_pshpHandle);
		}
		return m_pGeometry;
	}

	void FeatureShp::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool FeatureShp::Create(g_int lFID, FeatureClassShp* pFeatureClassShp)
	{
		m_fid = lFID;
		m_pFeatureClass = pFeatureClassShp;
		m_pFeatureClass->AddRef();
		m_pFields = m_pFeatureClass->GetFields();
		m_pSHPHandle = m_pFeatureClass->m_pshpHandle;
		m_pDBFHandle = m_pFeatureClass->m_pdbfHandle;

		return true;
	}

	Geometry* FeatureShp::CreateGeometry(long index, SHPHandle pSHPHandle)
	{
		if(pSHPHandle==NULL)
		{
			return NULL;
		}

		int	  iWKBLen = 0;
		g_uchar* pWKB = NULL;
		Geometry*	pGeometry  = NULL;
		SHPObject*	pSHPObject = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		pSHPObject = ::SHPReadObject(pSHPHandle, index);
		if(pSHPObject==NULL)
		{
			return NULL;
		}

		iWKBLen = ShpUtil::GetWKBLength(pSHPObject);
		if(iWKBLen==0)
		{
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}

		pWKB = (g_uchar*)auge_malloc(iWKBLen,sizeof(g_uchar));
		if(pWKB==NULL)
		{
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}
		memset(pWKB, 0, iWKBLen);
		if(ShpUtil::SHPObject_2_WKB(pSHPObject, pWKB, iWKBLen)==0)
		{
			delete[] pWKB;
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}

		pGeometry = pGeometryFactory->CreateGeometryFromWKB(pWKB, true);

		if(pGeometry==NULL)
		{
			delete[] pWKB;
		}

		::SHPDestroyObject(pSHPObject);

		return pGeometry;
	}
	
	GValue*	FeatureShp::GetValue(g_uint i) const
	{
		GValue* pValue = NULL;
		SHPHandle pSHPHandle = m_pFeatureClass->m_pshpHandle;
		DBFHandle pDBFHandle = m_pFeatureClass->m_pdbfHandle;
		//if(index == m_lFIDIndex)
		//{
		//	pValue = new GValue(m_fid);
		//}		
		//else
		{
			if(pDBFHandle==NULL)
				return NULL;
			//int innerIndex = GetFieldInnerIndex(index);		
			GField* pField = (GField*)m_pFeatureClass->GetFields()->GetField(i);
			if(pField==NULL)
				return NULL;

			const char* szFieldName = pField->GetName();
			int innerIndex = ::DBFGetFieldIndex(pDBFHandle, szFieldName);

			switch(pField->GetType())
			{
			case augeFieldTypeString:
				{				
					const char* szTemp = DBFReadStringAttribute (pDBFHandle,  m_fid, innerIndex);
					if(NULL == szTemp)
					{
						return NULL;
					}
					pValue = new GValue(szTemp);
				}			
				break;
			case augeFieldTypeInt:
				{
					int val;
					val = ::DBFReadIntegerAttribute(pDBFHandle, m_fid, innerIndex);
					pValue = new GValue(val);
				}
				break;
			case augeFieldTypeDouble:
				{
					double val;
					val = ::DBFReadDoubleAttribute(pDBFHandle, m_fid, innerIndex);
					pValue = new GValue(val);
				}
				break;
			case augeFieldTypeBool:
				{
					const char* val;
					val = ::DBFReadLogicalAttribute(pDBFHandle, m_fid, innerIndex);
					if(g_stricmp(val, "N")==0)
						pValue = new GValue(false);
					else
						pValue = new GValue(true);

				}
				break;
			case augeFieldTypeTime:
				{
					//const char* val;
					//val = ::DBFReadDateAttribute(pDBFHandle, m_fid, innerIndex);
					//TIME_STRU timStru;
					//Parse(timStru, val);
					//pValue = new GValue(&timStru, true);
				}
				break;

			case augeFieldTypeNone:
				{

				}
				break;
			case augeFieldTypeGeometry:
				{
					if(m_pGeometry==NULL)
					{
						FeatureShp* pThis = (FeatureShp*)this;
						pThis->m_pGeometry = pThis->CreateGeometry(m_fid, m_pFeatureClass->m_pshpHandle);
					}
					if(m_pGeometry!=NULL)
					{
						pValue = new GValue(m_pGeometry);
						m_pGeometry->AddRef();
					}
					
				}
				break;
			}
		}

		return pValue;
	}

	bool FeatureShp::GetBool(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return false;
		}
		if(pField->GetType()!=augeFieldTypeBool)
		{
			return false;
		}
		const char* val =::DBFReadLogicalAttribute(m_pDBFHandle, m_fid, i);
		return g_stricmp(val, "F");
	}

	char FeatureShp::GetChar(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return false;
		}
		if(pField->GetType()!=augeFieldTypeChar)
		{
			return false;
		}
		//const char* val =::DBFReadLogicalAttribute(m_pDBFHandle, m_fid, i);
		//return g_stricmp(val, "F");
		return 'a';
	}

	short FeatureShp::GetShort(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeShort)
		{
			return -1;
		}
		return ::DBFReadIntegerAttribute(m_pDBFHandle, m_fid, i);
	}

	int	FeatureShp::GetInt(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeInt)
		{
			return -1;
		}
		return ::DBFReadIntegerAttribute(m_pDBFHandle, m_fid, i);
	}

	long FeatureShp::GetLong(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeLong)
		{
			return -1;
		}
		return ::DBFReadIntegerAttribute(m_pDBFHandle, m_fid, i);
	}

	float FeatureShp::GetFloat(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeFloat)
		{
			return -1;
		}
		return ::DBFReadFloatAttribute(m_pDBFHandle, m_fid, i);
	}

	double FeatureShp::GetDouble(g_uint i) const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeDouble)
		{
			return -1;
		}
		return ::DBFReadDoubleAttribute(m_pDBFHandle, m_fid, i);
	}

	int64 FeatureShp::GetInt64(g_uint i)	const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return -1;
		}
		if(pField->GetType()!=augeFieldTypeInt64)
		{
			return -1;
		}
		return ::DBFReadDoubleAttribute(m_pDBFHandle, m_fid, i);
	}

	const char*	FeatureShp::GetString(g_uint i)	const
	{
		GField* pField = m_pFields->GetField(i);
		if(pField==NULL)
		{
			return NULL;
		}
		if(pField->GetType()!=augeFieldTypeString)
		{
			return NULL;
		}
		return ::DBFReadStringAttribute(m_pDBFHandle, m_fid, i);
	}

	TIME_STRU* FeatureShp::GetTime(g_uint i)		const
	{
		return NULL;
	}

	GValue*	FeatureShp::GetValue(const char* name) const
	{
		return GetValue(m_pFields->FindField(name));
	}

	bool FeatureShp::GetBool(const char* name) const
	{
		return GetBool(m_pFields->FindField(name));
	}

	char FeatureShp::GetChar(const char* name) const
	{
		return GetChar(m_pFields->FindField(name));
	}

	short FeatureShp::GetShort(const char* name) const
	{
		return GetShort(m_pFields->FindField(name));
	}

	int	FeatureShp::GetInt(const char* name) const
	{
		return GetInt(m_pFields->FindField(name));
	}

	long FeatureShp::GetLong(const char* name) const
	{
		return GetLong(m_pFields->FindField(name));
	}

	float FeatureShp::GetFloat(const char* name) const
	{
		return GetFloat(m_pFields->FindField(name));
	}

	double FeatureShp::GetDouble(const char* name) const
	{
		return GetDouble(m_pFields->FindField(name));
	}

	int64 FeatureShp::GetInt64(const char* name)	const
	{
		return GetInt64(m_pFields->FindField(name));
	}

	const char*	FeatureShp::GetString(const char* name)	const
	{
		return GetString(m_pFields->FindField(name));
	}

	RESULTCODE FeatureShp::SetValue(const char* name, GValue *pValue)
	{
		return AG_FAILURE;
	}
}