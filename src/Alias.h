#pragma once
#include <memory>

#include "Configuration.h"
#include "SDFs/SDF.h"
#include "GFs/GF.h"

namespace Alias 
{
	template<typename TFloatType>
	using SDF_UPtr = std::unique_ptr<SDF<TFloatType>>;

	template<typename TFloatType>
	using SDF_WPtr = std::weak_ptr<SDF<TFloatType>>;

	template<typename TFloatType>
	using SDF_RPtr = SDF<TFloatType>*;

	
	template<typename TFloatType>
	using GF_UPtr = std::unique_ptr<GF<TFloatType>>;
}

