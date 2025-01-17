#include "FeatureClassByd.h"
#include "FeatureCursorByd.h"
#include "FeatureInsertCommandByd.h"
#include "FeatureObj.h"
#include "WorkspaceByd.h"
#include "SQLBuilder.h"
#include "AugeField.h"

namespace auge
{
	FeatureClassByd::FeatureClassByd():
	m_srid(AUGE_DEFAULT_SRID),
	m_pFields(NULL),
	m_dimension(2),
	m_geom_type(augeGTNull)
	{

	}

	FeatureClassByd::~FeatureClassByd()
	{
		if(m_pFields!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFields);
		}
	}

	const char*	FeatureClassByd::GetName()
	{
		return m_name.c_str();
	}

	const char*	FeatureClassByd::GetAlias()
	{
		return m_name.c_str();
	}

	augeDataSetType FeatureClassByd::GetType()
	{
		return augeDataSetFeature;
	}

	g_uint FeatureClassByd::GetSRID()
	{
		return 0;
	}

	GEnvelope& FeatureClassByd::GetExtent()
	{
		m_extent.Set(-180.0, -90.0,180.0,90.0);
		return m_extent;
	}

	const char*	FeatureClassByd::GetUUID()
	{
		if(m_uuid.empty())
		{
			char uuid[AUGE_PATH_MAX];
			memset(uuid, 0, AUGE_PATH_MAX);
			auge_generate_uuid(uuid, AUGE_PATH_MAX);
			m_uuid = uuid;
		}
		return m_uuid.c_str();
	}

	GFields* FeatureClassByd::GetFields()
	{
		if(m_pFields==NULL)
		{
			m_pFields = CreateFields();
		}

		return m_pFields;
	}

	GField*	FeatureClassByd::GetField(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		GFields* pFields = GetFields();
		if(pFields==NULL)
		{
			return NULL;
		}
		return pFields->GetField(name);
	}

	RESULTCODE FeatureClassByd::AddField(const char* name, augeFieldType type, g_uint width/*=32*/)
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureClassByd::RemoveField(const char* name)
	{
		return AG_SUCCESS;
	}

	g_uint FeatureClassByd::GetCount()
	{
		std::string sql;
		SQLBuilder::BuildCount(sql, this);
		
		CPPIRow* row = NULL;
		CPPIDataItem* item = NULL;
		CPPIResultSet* rst = NULL;
		CPPIStatement* stmt = NULL;
		CPPIConnection* conn = m_pWorkspace->m_pbydConnction;
		CPPIEnvironment* env = m_pWorkspace->m_pbydEnvironment;

		stmt = m_pWorkspace->m_pbydConnction->CreateStatement();
		rst = stmt->ExecuteQuery(CATEXT(sql));
		if(rst==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();

			CPPIString msg = env->GetErrorMessage()->GetErrorString();
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			conn->CloseStatement(stmt);

			return 0;
		}

		CPPIStatus status = rst->Next();
		if(status!=CS_OK)
		{
			stmt->CloseResultSet(rst);
			conn->CloseStatement(stmt);

			return 0;
		}
		row = rst->GetRow();
		if(row==NULL)
		{
			stmt->CloseResultSet(rst);
			conn->CloseStatement(stmt);
			return 0;
		}

		CPPIUInt2 ii = 0;
		CPPIDataType type = row->GetField(ii)->GetType();
		g_uint count = row->GetItem(0)->GetAsInt8();
		stmt->CloseResultSet(rst);
		conn->CloseStatement(stmt);

		return count;
	}

	g_uint FeatureClassByd::GetCount(GEnvelope& extent)
	{
		//std::string sql;
		//SQLBuilder::BuildCount(sql, extent, this);
		//PGresult* pgResult =  m_pWorkspace->m_pgConnection_r.PgExecute(sql.c_str());
		//g_uint count = atoi(PQgetvalue(pgResult,0,0));
		//PQclear(pgResult);

		//return count;
		return 0;
	}

	g_uint FeatureClassByd::GetCount(GFilter* pFilter)
	{
		return 0;
	}

	FeatureCursor* FeatureClassByd::Query(augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursorByd* pCursor = new FeatureCursorByd();
		pCursor->Create(this);
		this->AddRef();

		return pCursor;
	}

	FeatureCursor* FeatureClassByd::Query(GEnvelope& extent, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursorByd* pCursor = new FeatureCursorByd();
		pCursor->Create(this, extent);
		this->AddRef();
		return pCursor;
	}

	FeatureCursor* FeatureClassByd::Query(GFilter* pFilter, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursorByd* pCursor = new FeatureCursorByd();
		pCursor->Create(this, pFilter);
		this->AddRef();
		return pCursor;
	}

	FeatureCursor* FeatureClassByd::Query(GQuery* pQuery, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursorByd* pCursor = new FeatureCursorByd();
		pCursor->Create(this, pQuery);
		return pCursor;
	}

	EnumValue* FeatureClassByd::GetUniqueValue(const char* field, augeOrderType order/*=augeOrderAsc*/)
	{
		return NULL;
	}

	EnumValue* FeatureClassByd::GetMinMaxValue(const char* field)
	{
		return NULL;
	}

	RESULTCODE FeatureClassByd::UpdateFeature(EnumString* pFieldNames, EnumValue* pValues, GFilter* pFilter)
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureClassByd::RemoveFeature(GFilter* pFilter)
	{
		return AG_SUCCESS;
	}

	FeatureInsertCommand* FeatureClassByd::CreateInsertCommand()
	{
		return (new FeatureInsertCommandByd(this));
	}

	Feature* FeatureClassByd::NewFeature()
	{
		return (new FeatureObj(this));
	}

	RESULTCODE FeatureClassByd::Refresh()
	{
		return AG_SUCCESS;
	}

	bool FeatureClassByd::Create(const char* name, WorkspaceByd* pWorkspace)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;

		GetGeometryMeta();
		//if(!GetGeometryMeta())
		//{
		//	return false;
		//}

		if(m_pFields==NULL)
		{
			m_pFields = CreateFields();
			if(m_pFields==NULL)
			{
				return false;
			}
		}

		return true;
	}

	bool FeatureClassByd::Create(const char* name, WorkspaceByd* pWorkspace, CPPIResultSet* pResultSet)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;

		GetGeometryMeta();
		//if(!GetGeometryMeta())
		//{
		//	return false;
		//}

		if(m_pFields==NULL)
		{
			m_pFields = CreateFields();
			if(m_pFields==NULL)
			{
				return false;
			}
		}

		return true;
	}

	bool FeatureClassByd::GetGeometryMeta()
	{
		const char* format = "select column_name,srs_name,srs_id,geometry_type,coord_dimension,xmin,ymin,xmax,ymax from ST_GEOMETRY_COLUMNS where table_name='%s'";
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, format, m_name.c_str());

		CPPIRow* row = NULL;
		CPPIDataItem* item = NULL;
		CPPIResultSet* rst = NULL;
		CPPIStatement* stmt = NULL;
		CPPIConnection* conn = m_pWorkspace->m_pbydConnction;
		CPPIEnvironment* env = m_pWorkspace->m_pbydEnvironment;

		stmt = m_pWorkspace->m_pbydConnction->CreateStatement();
		if(stmt==NULL)
		{
			return false;
		}
		rst = stmt->ExecuteQuery(CATEXT(sql));
		if(rst==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();

			CPPIString msg = env->GetErrorMessage()->GetErrorString();
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			conn->CloseStatement(stmt);

			return false;
		}

		CPPIStatus status = rst->Next();
		if(status!=CS_OK)
		{
			stmt->CloseResultSet(rst);
			conn->CloseStatement(stmt);

			return false;
		}
		row = rst->GetRow();
		if(row!=NULL)
		{
			// column_name,
			m_geom_field = row->GetItem(0)->GetAsString().c_str();
			// srs_name,
			m_srs_name = row->GetItem(1)->GetAsString().c_str();
			// srs_id,
			m_srid = row->GetItem(2)->GetAsInt4();
			// geometry_type,
			m_geom_type = ParseGeometryType(row->GetItem(3)->GetAsInt4());
			// coord_dimension,
			m_dimension = row->GetItem(4)->GetAsInt2();

			// xmin,ymin,xmax,ymax
			m_extent.m_xmin = row->GetItem(5)->GetAsFloat8();
			m_extent.m_ymin = row->GetItem(6)->GetAsFloat8();
			m_extent.m_xmax = row->GetItem(7)->GetAsFloat8();
			m_extent.m_ymax = row->GetItem(8)->GetAsFloat8();
		}

		stmt->CloseResultSet(rst);
		conn->CloseStatement(stmt);

		return true;
	}

	GFields* FeatureClassByd::CreateFields()
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select first 1 * from %s", m_name.c_str());

		CPPIRow* row = NULL;
		CPPIResultSet* rst = NULL;
		CPPIStatement* stmt = NULL;
		CPPIConnection* conn = m_pWorkspace->m_pbydConnction;
		CPPIEnvironment* env = m_pWorkspace->m_pbydEnvironment;

		stmt = m_pWorkspace->m_pbydConnction->CreateStatement();
		rst = stmt->ExecuteQuery(CATEXT(sql));
		if(rst==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();

			CPPIString msg = env->GetErrorMessage()->GetErrorString();
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			conn->CloseStatement(stmt);

			return NULL;
		}

		GField* pField = NULL;
		GFields* pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();

		CPPIField  *pbydField  = NULL;
		CPPIFields *pbydFields = rst->GetFields();
		if(pbydFields!=NULL)
		{
			CPPIUInt2 count = pbydFields->GetCount();
			for(CPPIUInt2 i=0; i<count; i++)
			{
				pbydField = pbydFields->GetField(i);
				pField = CreateField(pbydField);
				if(pField!=NULL)
				{
					pFields->Add(pField);
				}
			}
		}

		stmt->CloseResultSet(rst);
		conn->CloseStatement(stmt);

		return pFields;
	}

	GFields* FeatureClassByd::CreateFields(CPPIResultSet* pResultSet)
	{
		GField* pField = NULL;
		GFields* pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();

		CPPIField  *pbydField  = NULL;
		CPPIFields *pbydFields = pResultSet->GetFields();
		if(pbydFields!=NULL)
		{
			CPPIUInt2 count = pbydFields->GetCount();
			for(CPPIUInt2 i=0; i<count; i++)
			{
				pbydField = pbydFields->GetField(i);
				pField = CreateField(pbydField);
				if(pField!=NULL)
				{
					pFields->Add(pField);
				}
			}
		}
		return pFields;
	}

	GField*	FeatureClassByd::CreateField(CPPIField* pbydField)
	{
		GField* pField = NULL;
		GField_2* pField_2 = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();

		const char* fname = pbydField->GetName();
		CPPIDataType type = pbydField->GetType();
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(pbydField->GetName());
		pField_2->SetAlias(pbydField->GetName());
		pField_2->SetNullable(pbydField->GetNullable());
		pField_2->SetPrecision(pbydField->GetPrecision());
		pField_2->SetScale(pbydField->GetScale());

		if(stricmp(fname, m_geom_field.c_str())==0)
		{
			pField_2->SetType(augeFieldTypeGeometry);
			GeometryDef* pGeometryDef = pField_2->GetGeometryDef();
			GeometryDef_2* pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();			
			pGeometryDef_2->SetGeometryType(m_geom_type);
			pGeometryDef_2->SetDefault(true);
			pGeometryDef_2->SetSRID(m_srid);
			pGeometryDef_2->SetExtent(m_extent);
			pGeometryDef_2->SetDimension(m_dimension);
		}
		else
		{
			switch(type)
			{
			case CPPI_BOOL_TYPE:
				pField_2->SetType(augeFieldTypeBool);
				break;
			case CPPI_CHA_TYPE:
				pField_2->SetType(augeFieldTypeChar);
				break;
			case CPPI_VARCHAR_TYPE:
				pField_2->SetType(augeFieldTypeString);
				pField_2->SetLength(pbydField->GetLength());
				break;
			case  CPPI_INTEGER_TYPE:
				pField_2->SetType(augeFieldTypeInt);
				break;
			case CPPI_FLOAT_TYPE:
				pField_2->SetType(augeFieldTypeDouble);
				break;
			case CPPI_LBYTE_TYPE:
				//pField_2->SetType(augeFieldTypeBLOB);
				//break;
			case CPPI_GEOMETRY_TYPE:
				pField_2->SetType(augeFieldTypeGeometry);
				m_geom_field = fname;
				break;
			}
		}		

		return pField;
	}

	augeGeometryType FeatureClassByd::ParseGeometryType(int type)
	{
		switch(type)
		{
		case 1:
			return augeGTPoint;
		case 2:
			return augeGTLineString;
		case 3:
			return augeGTPolygon;
		case 4:
			return augeGTMultiPoint;
		case 5:
			return augeGTMultiLineString;
		case 6:
			return augeGTMultiPolygon;
		case 7:
			return augeGTGeometryCollection;
		default:
			break;
		}
		return augeGTNull;
	}

	RESULTCODE FeatureClassByd::BuildSpatialIndex()
	{
		GEnvelope& extent = GetExtent();
		GFields* pFields = GetFields();
		GField* pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			return AG_FAILURE;
		}

		std::string sql;
		SQLBuilder::BuildCreateSpatialIndex(sql, GetName(), pField->GetName(), extent);

		return m_pWorkspace->ExecuteSQL(sql.c_str());
	}

	const char*	FeatureClassByd::GetSize()
	{
		return "0 MB";
	}
}
