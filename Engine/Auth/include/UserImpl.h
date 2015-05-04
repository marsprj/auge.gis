#ifndef __AUGE_USER_IMPL_H__
#define __AUGE_USER_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class UserImpl : public User
	{
	public:
		UserImpl();
		virtual ~UserImpl();
	public:
		//! @brief	��ȡ�ֶ���
		//! @return	�ֶ���
		virtual const char*		GetName();
		//! @brief	��ȡ�ֶα���
		//! @return	�ֶα���
		virtual const char*		GetAlias();
		virtual const char*		GetPassword();
		virtual const char*		GetEmail();
		virtual Role*			GetRole();
		virtual void			Release();

	public:
		bool	Create(int gid, const char* name, const char* alias, const char* passwd, const char* email);
		void	SetName(const char* name);
		void	SetAlias(const char* alias);
		void	SetPasswd(const char* passwd);
		void	SetEmail(const char* email);
		void	SetRole(Role* pRole);

	private:
		std::string m_name;
		std::string m_alias;
		std::string m_passwd;
		std::string m_email;
		Role*		m_pRole;
	};
}

#endif //__AUGE_USER_IMPL_H__