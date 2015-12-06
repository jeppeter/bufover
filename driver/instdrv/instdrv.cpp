
#include "instdrv.h"



InstDrv::InstDrv(const char* pSvcName,const char* pSysFile)
{
	this->m_hSC = NULL;
	strncpy(this->m_svcname,pSvcName,sizeof(this->m_svcname));
	strncpy(this->m_sysfile,pSysFile,sizeof(this->m_sysfile));
}

InstDrv::~InstDrv()
{
	this->__CloseScManager();
	memset(this->m_svcname,0,sizeof(this->m_svcname));
	memset(this->m_sysfile,0,sizeof(this->m_sysfile));
}

void InstDrv::__CloseScManager()
{
	if (this->m_hSC != NULL)
	{
		BOOL bret;
		bret = CloseServiceHandle(this->m_hSC);
		if (!bret)
		{
			ERROR_INFO("can not close service handle (%d)\n",GETERRNO());
		}
		this->m_hSC = NULL;
	}
	return ;
}

void InstDrv::__OpenScManager()
{
	int ret=0;
	this->__CloseScManager();
	this->m_hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (this->m_hSC != NULL)
	{
		ret = GETERRNO();
	}
	return ret;
}
