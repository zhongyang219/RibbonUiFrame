#pragma once
#include "serviceinterface.h"
#define STR_MFC_MODULE_SERVICE "MFCModuleService"
class IMFCModuleService : public IService
{
public:
    virtual void SetEditText(const wchar_t* strText) = 0;
};

