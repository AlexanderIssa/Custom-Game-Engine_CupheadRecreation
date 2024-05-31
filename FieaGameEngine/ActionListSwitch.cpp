#include"pch.h"
#include "ActionListSwitch.h"
#include "GameObject.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionListSwitch);

		/*
		* Default constructor for ActionListSwitch
		*/
		ActionListSwitch::ActionListSwitch() : ActionList(ActionListSwitch::TypeIdClass()) {}

		/*
		* Clone function for ActionListSwitch
		* @return - A new ActionListSwitch object that is a copy of this ActionListSwitch object
		*/
		ActionListSwitch* ActionListSwitch::clone() const
		{
			return new ActionListSwitch(*this);
		}

		/*
		* Specialized constructor for ActionListSwitch
		*/
		void ActionListSwitch::onEnable()
		{

		}

		/*
		* Specialized constructor for ActionListSwith
		*/
		void ActionListSwitch::onDisable()
		{
			if (!getParentGameObject()->alive)
			{
				currentCase = "";
				caseList.clear();
				cases.clear();
			}
		}

		// Excute the switch statement
		// Also check if the current case can fall through
		void ActionListSwitch::update(const GameTime& tick)
		{		
			if (cases.size() == 0)
				return;

			if (currentCase == "")
				return;

			// Update current case
			cases[currentCase].action->update(tick);

			// If the current case can fall through
			if (cases[currentCase].fallThrough)
			{
				const std::string& nextCase = cases[currentCase].nextCase;

				// If next case is not empty, update the next case
				if (nextCase != "")
				{
					cases[nextCase].action->update(tick);
				}
			}
		}

		/*
		* Add a case to the switch statement
		* @param caseName - The name of the case to add
		* @param className - The name of the Action class to create
		* @param instanceName - The name of the instance of the Action class to create
		*/
		void ActionListSwitch::addCase(const std::string& caseName, 
			const std::string& className, const std::string& instanceName, bool fallThrough)
		{
			if (caseName == "" || caseName.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			{
				throw std::invalid_argument("Cannot add an Action with an empty key");
			}

			Action* action = createAction(className, instanceName);

			// Create a case data for linked list
			CaseData caseData;
			caseData.name = caseName;
			caseData.fallThrough = fallThrough;
			caseData.action = action;

			// Update the fall through cases
			updateCaseChain(caseData, false);
		}

		void ActionListSwitch::addCase(const std::string& caseName, const std::string& className, bool fallThrough)
		{
			addCase(caseName, className, "", fallThrough);
		}

		/*
		* Remove a case from the switch statement
		* @param caseName - The name of the case to remove
		*/
		void ActionListSwitch::removeCase(const std::string& caseName)
		{
			//Check if the case exists
			if (cases.find(caseName) == cases.end())
			{
				throw std::invalid_argument("Case does not exist in this ActionListSwitch");
			}

			// Get the case data
			CaseData& caseData = cases[caseName];
			removeAction(cases[caseName].action->name);
			updateCaseChain(caseData, true);
		}

		/*
		* Set the current case
		* @param caseName - The name of the case to set as the current case
		*/
		void ActionListSwitch::setCurrentCase(const std::string& caseName)
		{
			//Check if the case exists
			if (cases.find(caseName) == cases.end())
			{
				throw std::invalid_argument("Case does not exist in this ActionListSwitch");
			}

			currentCase = caseName;
		}

		/*
		* Prescribed attributes for every ActionListSwitch object
		* @return - A vector of Signatures representing the prescribed attributes for ActionListSwitch
		*/
		std::vector<Signature> ActionListSwitch::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_currentCase"s, Datum::DataType::STRING, 1, offsetof(ActionListSwitch, currentCase) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string ActionListSwitch::ToString() const
		{
			return "ActionListSwitch"s;
		}

		/*
		* Update the fall through cases
		* @param caseName - The name of the case to update the fall through for
		*/
		void ActionListSwitch::updateCaseChain(CaseData& caseData, bool remove)
		{
			if (remove)
			{
				for (auto& successor: caseList)
				{
					if (successor->nextCase == caseData.name)
					{
						if (caseData.nextCase == "") // if caseData is the last case in the chain
						{
							successor->nextCase = "";
						}
						else // if caseData is in the middle of the chain
						{
							successor->nextCase = caseData.nextCase;
						}
						break;
					}
				}
				caseList.erase(std::remove_if(caseList.begin(), caseList.end(), 
					[&caseData](const CaseData* data) { return data->name == caseData.name; }), caseList.end());
				cases.erase(caseData.name);
			}
			else
			{
				cases[caseData.name] = caseData;
				if (caseList.size() > 0)
				{
					CaseData* lastCase = caseList.back();
					lastCase->nextCase = caseData.name;
				}
				caseList.push_back(&cases[caseData.name]);
			}
		}
	}
}