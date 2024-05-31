#include "pch.h"
#include "ParticleRenderAction.h"
#include "GameClock.h"
#include "GameObject.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ParticleRenderAction);
	ParticleRenderAction::ParticleRenderAction(IdType type) : RenderAction(type)
	{
	}

	ParticleRenderAction::ParticleRenderAction() : RenderAction(TypeIdClass())
	{
	}

	ParticleRenderAction::ParticleRenderAction(const ParticleRenderAction& other) : RenderAction(other),  _anim(other._anim)
	{
	}

	ParticleRenderAction::~ParticleRenderAction()
	{
	}

	RenderAction* ParticleRenderAction::clone() const
	{
		return new ParticleRenderAction(*this);
	}

	void ParticleRenderAction::onEnable()
	{
		RenderAction::onEnable();
	}

	void ParticleRenderAction::onDisable()
	{
		RenderAction::onDisable();
	}

	void ParticleRenderAction::update(const GameTime& time)
	{
		if (jeffVariable < 0) { return; }
		currFrame += 24.0f * time.Frame() * 0.001f;

		if (currFrame >= _anim.nFrames)
		{
			currFrame = 0.0;
			getParentGameObject()->enabled = false; // TODO: instead of enabled = false, what about just set jeffVariable to -0.9
		}
	}

	std::vector<Signature> ParticleRenderAction::prescribedAttributes()
	{
		return std::vector<Signature> {
			{ "int_startingCell"s, Datum::DataType::INTEGER, 1, offsetof(ParticleRenderAction, _anim.startingCell)},
			{ "int_nFrames"s, Datum::DataType::INTEGER, 1, offsetof(ParticleRenderAction, _anim.nFrames) },
			{ "int_nCols"s, Datum::DataType::INTEGER, 1, offsetof(ParticleRenderAction, nCols) },
			{ "float_jeffVariable"s, Datum::DataType::FLOAT, 1, offsetof(ParticleRenderAction, jeffVariable) }
		};
	}

	bool ParticleRenderAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string ParticleRenderAction::ToString() const
	{
		return std::string("ParticleRenderAction");
	}

	int ParticleRenderAction::getFrameIndex() const
	{
		return (int)currFrame + _anim.startingCell;
	}

	void ParticleRenderAction::SetTextureAndAnim(CTexture& tex, SpriteAnim& anim)
	{
		this->texture = tex;
		_anim = anim;
		nCols = anim.nFrames;
	}


}
