#pragma once
#include "interface/IMFCModuleService.h"
class MFCModuleService : public IMFCModuleService
{
public:
    virtual void SetEditText(const wchar_t* strText) override;

};

