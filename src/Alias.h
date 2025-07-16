#pragma once
#include <memory>

#include "Configuration.h"
#include "SDFs/SDF.h"

namespace Alias 
{
	template<typename TFloatType>
	using SDF_Ptr = std::unique_ptr<SDF<TFloatType>>;
}

