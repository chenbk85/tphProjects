//---------------------------------------------------------------------
// ģ������:    �������ӷ����api
// �ļ�����:    Serverapi.h
// ������:      qinjian
// ��������:    20140724
// �޸���ʷ:
// ��    ע:    
//----------------------------------------------------------------------
#ifndef CLIENTAPI_H
#define CLIENTAPI_H
#ifdef CLIENTAPI_EXPORTS
#define CLIENTAPI_LIB __declspec(dllexport)
#else
#ifdef WIN32
#define CLIENTAPI_LIB __declspec(dllexport)
#else
#define CLIENTAPI_LIB
#endif
#endif

#include "BusStruct.h"
#include "Package.h"

//�ص��ӿ�
class CClientApiSpi
{
public:
    //����״̬֪ͨ
    virtual void OnNtyStatus(NTY_STATUS_INFO *pStatus){};
    virtual void OnRcvRspData(CPackage *pPackage,int &nEventHandle,USER_HANDLE &handle){};
    virtual void OnRcvFile(char *FileName){};
    virtual void OnRcvReq(int &ReqNo,CPackage *pPara,USER_HANDLE &handle){};
};


class CLIENTAPI_LIB CClientApi
{
public:

    //---------------------------------------------------------------------------
    //����һ�����ݰ�
    virtual CPackage * CreatePackage() = 0;
    //ɾ�����ݰ�
    virtual void ReleasePackage(CPackage * pPackage) = 0;
    //����apiʵ��
    static CClientApi *CreateClientApi();
    //ע��ص��ӿ�
    virtual void RegisterSpi(CClientApiSpi *pGeneralSpi) = 0;
    //�ͷ���Դ
    virtual void Release() = 0;
    //��ʼ��
    virtual int Init(ERROR_INFO &ErrMsg)=0;

    //���ӵ�����
    virtual bool Connect(char *routerip, int port, ERROR_INFO &ErrMsg) = 0;
    //ע��һ������ŵ�����
    virtual int AddService(int seriveno,ERROR_INFO &ErrMsg)= 0;

    //�����ļ���ָ�������߽ڵ�
    virtual int SendFileToService(char *filename,int seriveno,ERROR_INFO &ErrMsg)=0;
    //�����ļ����ͻ�
    virtual int SendFileToClient(char *filename,USER_HANDLE *UserHandle,ERROR_INFO &ErrMsg)=0;

    //�������ݰ���ָ�������߽ڵ�
    virtual int SendDataToService(int nSeriveNo,CPackage *pData,ERROR_INFO &ErrMsg)=0;
    //�����������߽ڵ�
    virtual int SendRequestToService(int nSeriveNo,int nReqNo,CPackage *Para,ERROR_INFO &ErrMsg)=0;
    //�������ݰ����ͻ�
    virtual int SendDataToClient(USER_HANDLE *UserHandle,CPackage *pData,ERROR_INFO &ErrMsg)=0;

public:
    virtual ~CClientApi();

};

#endif