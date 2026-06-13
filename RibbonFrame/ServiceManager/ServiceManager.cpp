#include "ServiceManager.h"

CServiceManager CServiceManager::m_instance;

bool CServiceManager::RegisterService(const char* className, IService* pService)
{
    std::string strClassName(className);
    auto iter = m_serviceMap.find(strClassName);
    //如果服务已注册，返回失败
    if (iter != m_serviceMap.end())
        return false;
    std::unique_ptr<IService> service = std::unique_ptr<IService>(pService);
    m_serviceMap[strClassName] = std::move(service);
    return true;
}

IService* CServiceManager::GetService(const char* className) const
{
    std::string strClassName(className);
    auto iter = m_serviceMap.find(strClassName);
    if (iter != m_serviceMap.end())
        return iter->second.get();
    return nullptr;
}

CServiceManager& CServiceManager::Instance()
{
    return m_instance;
}
