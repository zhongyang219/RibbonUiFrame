#pragma once
#include "serviceinterface.h"
#include <string>
#include <map>
#include <memory>
class CServiceManager
{
public:
    bool RegisterService(const char* className, IService* pService);
    IService* GetService(const char* className) const;

    static CServiceManager& Instance();

private:
    static CServiceManager m_instance;
    std::map<std::string, std::unique_ptr<IService>> m_serviceMap;
};

