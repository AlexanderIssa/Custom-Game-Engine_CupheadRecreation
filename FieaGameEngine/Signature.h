#pragma once
#include <string>
#include "Datum.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Signature
		{
		public:
			Signature() = default;
			~Signature() = default;

			bool operator==(const Signature& other) const 
			{
				return Name == other.Name && Type == other.Type && Size == other.Size && Offset == other.Offset;
			}
			bool operator!=(const Signature& other) const
			{
				return !(*this == other);
			}

			std::string Name = "";
			Datum::DataType Type = Datum::DataType::UNKNOWN;
			size_t Size = 0;
			size_t Offset = 0;
		};
	}
}
