#pragma once

#include "Engine/UUID.h"

namespace Engine
{
	class Asset
	{
	public:
		UUID GetUUID() { return m_UUID; }

	private:
		UUID m_UUID;
	};
}