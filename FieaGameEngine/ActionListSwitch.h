#pragma once
#include "ActionList.h"
#include <unordered_map>
#include <vector>

namespace Fiea
{
	namespace GameEngine
	{
		class ActionListSwitch final : public ActionList
		{
			RTTI_DECLARATIONS(ActionListSwitch, ActionList);

		struct CaseData
		{
			std::string name;
			std::string nextCase;
			Action* action;
			bool fallThrough = false;
		};

		public:
			// Default constructor
			ActionListSwitch();

			// Copy constructor
			ActionListSwitch(const ActionListSwitch& other) = default;

			// Move constructor
			ActionListSwitch(ActionListSwitch&& other) noexcept = default;

			// Copy assignment operator
			ActionListSwitch& operator=(const ActionListSwitch& rhs) = default;

			// Move assignment operator
			ActionListSwitch& operator=(ActionListSwitch&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] ActionListSwitch* clone() const override;

			// Destructor
			~ActionListSwitch() override = default;

			// Enable function to be called when the object is enabled
			virtual void onEnable() override;

			// Disable function to be called when the object is disabled
			virtual void onDisable() override;

			// Update function to be called every frame
			void update(const GameTime&) override;

			// Add a case to the switch statement
			void addCase(const std::string& caseName, const std::string& className, const std::string& instanceName, bool fallThrough = false);
			void addCase(const std::string& caseName, const std::string& className, bool fallThrough = false);

			// Remove a case from the switch statement
			void removeCase(const std::string& caseName);

			// Set the current case
			void setCurrentCase(const std::string& caseName);

			// Get the prescribed attributes of the ActionListSwitch class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

		private:
			std::string currentCase = "";

			std::vector<CaseData*> caseList;
			std::unordered_map<std::string, CaseData> cases;

			void updateCaseChain(CaseData& caseData, bool remove);
		};
	}
}
