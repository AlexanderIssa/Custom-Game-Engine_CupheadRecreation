#pragma once
#include "ParseCoordinator.h"

namespace Fiea
{
	namespace GameEngine
	{
		template<typename T>
		T* ParseCoordinator::getHandler(HandlerRef ref) const
		{
			IParseHandler& handler = *handlers.at(ref);
			return dynamic_cast<T*>(&handler);
		}
	}
}