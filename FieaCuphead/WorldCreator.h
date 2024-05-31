#pragma once
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		class GameObject;
		class WorldCreator
		{
		public:
			GameObject* CreateTheWorld();

		private:
			GameObject* world = nullptr;
			AttributedWrapper worldWrapper;
		};
		
	}
}