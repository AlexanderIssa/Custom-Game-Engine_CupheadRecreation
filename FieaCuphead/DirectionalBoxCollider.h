//#pragma once
//#include "Collision.h"
//
//namespace Fiea::GameEngine
//{
//	class DirectionalBoxCollider : public BoxCollider
//	{
//		RTTI_DECLARATIONS(DirectionalBoxCollider, BoxCollider);
//	protected:
//
//		DirectionalBoxCollider(IdType id);
//	public:
//		DirectionalBoxCollider();
//		DirectionalBoxCollider(const DirectionalBoxCollider& other);
//
//		[[nodiscard]] virtual DirectionalBoxCollider* clone() const override;
//
//		virtual ~DirectionalBoxCollider() override = default;
//
//		virtual void onEnable() override;
//
//		virtual void onDisable() override;
//
//		virtual void update(const GameTime& time) override;
//
//		static std::vector<Signature> prescribedAttributes();
//
//		// Register prescribed attributes
//		static bool registerSignatures();
//
//		// RTTI Overrides
//		virtual std::string ToString() const override;
//
//		virtual AABB getAABB() const override;
//		virtual bool processCollision(AbstractCollider* other, const GameTime& time) override;
//
//	};
//}