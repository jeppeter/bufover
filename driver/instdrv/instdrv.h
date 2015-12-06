#ifndef __INST_DRV_H__
#define __INST_DRV_H__

#define  SVC_NAME_SIZE        256
#define  SYS_FILE_SIZE        256

class InstDrv
{

public:
	InstDrv(const char* pSvcName,const char* pSysFile);
	virtual ~InstDrv();
	int RegisterDrv(const char* pDesc);
	int UnregisterDrv();
	int RunDrv();
	int StopDrv();

private:
	char m_svcname[SVC_NAME_SIZE];
	char m_sysfile[SYS_FILE_SIZE];
	SC_HANDLE m_hSC;

private:
	int __OpenScManager();
	int __CreateService();
	void __CloseScManager();
};


#endif /*__INST_DRV_H__*/