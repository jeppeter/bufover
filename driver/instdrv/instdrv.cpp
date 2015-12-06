
#include "instdrv.h"



InstDrv::InstDrv(const char* pSvcName,const char* pSysFile)
{
	this->m_hSC = NULL;
	this->m_hSvc = NULL;
	strncpy(this->m_svcname,pSvcName,sizeof(this->m_svcname));
	strncpy(this->m_sysfile,pSysFile,sizeof(this->m_sysfile));
	memset(this->m_svcansi,0,sizeof(this->m_svcansi));
	memset(this->m_sysansi,0,sizeof(this->m_sysansi));
	memset(this->m_svcdesc,0,sizeof(this->m_svcdesc));
}

InstDrv::~InstDrv()
{
	this->__CloseSvc();
	this->__CloseScManager();
	memset(this->m_svcname,0,sizeof(this->m_svcname));
	memset(this->m_sysfile,0,sizeof(this->m_sysfile));
}

void InstDrv::__CloseSvc()
{
	if (this->m_hSvc != NULL)
	{
		BOOL bret;
		bret = CloseServiceHandle(this->m_hSvc);
		if (!bret)
		{
			ERROR_INFO("can not close service handle (%d)\n",GETERRNO());
		}
		this->m_hSvc = NULL;
	}
	return ;
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
	if (this->m_hSC != NULL)
	{
		return 0;
	}
	
	this->m_hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (this->m_hSC != NULL)
	{
		ret = GETERRNO();
	}
	return ret;
}

int InstDrv::__OpenSvc(DWORD access)
{
	int ret=0;

	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}

	/*we close the service for it will reopen with different access*/
	this->__CloseSvc();

	this->m_hSvc = OpenService(this->m_hSC,this->m_svcname,access);
	if (this->m_hSvc == NULL)
	{
		ret = GETERRNO();
		ERROR_INFO("open (%s) service error (%d)\n",this->m_svcansi,ret);
		goto fail;
	}

	return 0;
fail:
	this->__CloseSvc();
	this->__CloseScManager();
	return ret;
}

int InstDrv::__CreateService()
{
	int ret;
	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}
	this->__CloseSvc();
	this->m_hSvc = CreateService();
}


int InstDrv::RegisterDrv(const char* pDesc)
{
	int ret;

	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}

	ret = 


}