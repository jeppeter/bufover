#ifndef __INST_DRV_H__
#define __INST_DRV_H__

#include <windows.h>

#define  SVC_NAME_SIZE        256
#define  SYS_FILE_SIZE        256
#define  SVC_DESC_SIZE        256

class InstDrv
{

public:
	InstDrv(const char* pSvcName);
	virtual ~InstDrv();
	int RegisterDrv(const char* pDesc);
	int SetDrvBin(const char* sysfile);
	int UnregisterDrv();
	int RunDrv();
	int StopDrv();

private:
	TCHAR m_svcname[SVC_NAME_SIZE];
	TCHAR m_sysfile[SYS_FILE_SIZE];
	TCHAR m_svcdesc[SVC_DESC_SIZE];
	char m_svcansi[SVC_NAME_SIZE];
	char m_sysansi[SYS_FILE_SIZE];
	SC_HANDLE m_hSC;
	SC_HANDLE m_hSvc;

private:
	int __OpenScManager(DWORD access);
	int __OpenSvc(DWORD access);
	int __CreateService();
	void __CloseScManager();
	void __CloseSvc();
};


#endif /*__INST_DRV_H__*/