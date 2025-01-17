#ifndef __AUGE_WORKSPACE_SHP_H__
#define __AUGE_WORKSPACE_SHP_H__

#include "AugeFeature.h"
#include "shapefil.h"
#include <string>

namespace auge
{
	class WorkspaceShp : public FeatureWorkspace
	{
		friend class FeatureClassShp;

	public:
		WorkspaceShp();
		virtual ~WorkspaceShp();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);
		virtual	DataEngine*		GetEngine();

	public:
		virtual void			SetUser(g_uint user);
		virtual g_int			GetUser();

		/* DATABASE=d:\\data\\world */
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();
		virtual GConnection*	GetConnection();

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		virtual EnumDataSet*	GetFeatureClasses();
		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		virtual FeatureClass*	CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);

	private:
		g_uint			GetShpType(augeGeometryType type);
		DBFFieldType	GetDbfType(augeFieldType type);

	private:
		std::string	m_connstring;
		std::string	m_path;
		std::string m_name;

		g_int	m_user;
	};
}

#endif //__AUGE_WORKSPACE_SHP_H__
