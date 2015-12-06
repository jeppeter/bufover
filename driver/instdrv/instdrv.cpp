
#include "instdrv.h"



InstDrv::InstDrv(const char* pSvcName,const char* pSysFile)
{
	this->m_hSC = NULL;
	this->m_hSvc = NULL;



	strncpy(this->m_svcansi,pSvcName,sizeof(this->m_svcname));
	strncpy(this->m_sysansi,pSysFile,sizeof(this->m_sysfile));

#ifdef _UNICODE
	wchar_t* pRetWide=NULL;
	int wsize=0;
	int ret;

	ret = AnsiToUnicode(pSvcName,&pRetWide,&wsize);
	ASSERT_IF(ret >= 0);
	wscncpy(this->m_svcname,pRetWide,sizeof(this->m_svcname)/sizeof(this->m_svcname[0]));

	ret = AnsiToUnicode(pSysFile,&pRetWide,&wsize);
	ASSERT_IF(ret >= 0);
	wscncpy(this->m_sysfile,pRetWide,sizeof(this->m_sysfile)/sizeof(this->m_sysfile[0]));
	/*reset for null*/
	AnsiToUnicode(NULL,&pRetWide,&wsize);
#else
	strncpy(this->m_svcansi,pSvcName,sizeof(this->m_svcname));
	strncpy(this->m_sysansi,pSysFile,sizeof(this->m_sysfile));
#endif

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
	this->m_hSvc = CreateService(this->m_hSC,
		this->m_svcname,this->m_svcdesc,0,SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,this->m_sysfile,NULL,NULL,NULL,NULL,NULL);
	if (this->m_hSvc == NULL)
	{
		ret = GETERRNO();
		ERROR_INFO("can not create (%s) error(%d)\n",this->m_svcansi,ret);
		goto fail;
	}

	/*to close service*/
	this->__CloseSvc();
	return 0;
fail:
	this->__CloseSvc();
	this->__CloseScManager();
	return ret;
}


int InstDrv::RegisterDrv(const char* pDesc)
{
	int ret;
#ifdef _UNICODE
	int wsize=0;
	wchar_t* pRetWide=NULL;
	if (pDesc != NULL)
	{
	ret = AnsiToUnicode(pDesc,&pRetWide,&wsize);
	if (ret < 0)
	{
		return ret;
	}
	wscncpy(this->m_svcdesc,pRetWide,sizeof(this->m_svcdesc)/sizeOf(this->m_svcdesc[0]));
	AnsiToUnicode(NULL,&pRetWide,&wsize);
	}
	else
	{
		memset(this->m_svcdesc,0,sizeof(this->m_svcdesc));
	}
#else
	if (pDesc != NULL)
	{
	strncpy(this->m_svcdesc,pDesc,sizeof(this->m_svcdesc)/sizeOf(this->m_svcdesc[0]));
	}
	else
	{
		memset(this->m_svcdesc,0,sizeof(this->m_svcdesc));
	}
#endif

	ret=  this->__CreateService();
	if (ret < 0)
	{
		return ret;
	}
	/*return 0*/
	return 0;
}

int InstDrv::RunDrv()
{
	int ret;
	BOOL bret;
	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}

	ret = this->__OpenSvc(SERVICE_START);
	if (ret < 0)
	{		
		goto fail;
	}

	bret = StartService(this->m_hSvc,0,NULL);
	if (!bret)
	{
		ret = GETERRNO();
		ERROR_INFO("can not start service(%s) error(%d)\n",this->m_svcansi,ret);
		goto fail;
	}

	return 0;
fail:
	this->__CloseSvc();
	this->__CloseScManager();
	return ret;
}

int InstDrv::StopDrv()
{
	int ret;
	BOOL bret;
	SERVICE_STATUS sts;
	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}

	ret = this->__OpenSvc(SERVICE_STOP);
	if (ret < 0)
	{		
		goto fail;
	}

	bret = ControlService(this->m_hSvc, SERVICE_CONTROL_STOP, &sts);
	if (!bret)
	{
		ret = GETERRNO();
		ERROR_INFO("can not start service(%s) error(%d)\n",this->m_svcansi,ret);
		goto fail;
	}
	DEBUG_INFO("stop service (%s) status (%d)\n",this->m_svcansi,sts);
	return 0;
fail:
	this->__CloseSvc();
	this->__CloseScManager();
	return ret;	
}

int InstDrv::UnregisterDrv()
{
	int ret;
	BOOL bret;
	ret = this->__OpenScManager();
	if (ret < 0)
	{
		return ret;
	}

	ret = this->__OpenSvc(DELETE);
	if (ret < 0)
	{		
		goto fail;
	}

	bret = DeleteService(this->m_hSvc);
	if (!bret)
	{
		ret = GETERRNO();
		ERROR_INFO("can not delete service (%s) error (%d)\n",this->m_svcansi,ret);
		goto fail;
	}

	/*close it as delete it*/
	this->__CloseSvc();

	return 0;
fail:
	this->__CloseSvc();
	this->__CloseScManager();
	return ret;	

}