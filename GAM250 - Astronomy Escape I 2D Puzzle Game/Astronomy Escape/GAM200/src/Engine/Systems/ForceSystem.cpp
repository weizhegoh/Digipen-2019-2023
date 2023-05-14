#include <ForceSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;

void ForceSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Rigidbody>());
	signature.set(mGameECSmanager.GetComponentType<Position>());
	signature.set(mGameECSmanager.GetComponentType<Force>());
	mGameECSmanager.SetSystemBitSignature<ForceSystem>(signature);
}

void ForceSystem::Update(EntitySystemOption systemoption)
{
	// bool check, den call addforce depending on the forcetype
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		if (objecttype.objType == ObjectType::MAIN_CHARACTER ||
			objecttype.objType == ObjectType::PUSHABLE_BARREL ||
			objecttype.objType == ObjectType::MONSTER)
		{
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// Apply Friction
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			if (objecttype.objType != ObjectType::PUSHABLE_BARREL &&
				objecttype.objType != ObjectType::PARTICLE)
			{
				State& state = mGameECSmanager.GetComponent<State>(entity);
				if (state.objectState == ObjectState::IDLE)
					rigidbody.velocity = glm::vec3(0.f, 0.f, 0.f);
				//else
				//	rigidbody.velocity *= VELOCITY_CAP;
			}
		}
		else if (objecttype.objType == ObjectType::PARTICLE)
		{

		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Apply Epsioln
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (rigidbody.velocity.x < 0.02f && rigidbody.velocity.x > -0.02f)
			rigidbody.velocity.x = 0.0f;
		if (rigidbody.velocity.y < 0.02f && rigidbody.velocity.y > -0.02f)
			rigidbody.velocity.y = 0.0f;

		FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
		//Force& forces = mGameECSmanager.GetComponent<Force>(entity);
		for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
		{
			rigidbody.Integrate(position.pos, /*forces.mResultantForce*/mathEngine::Vector3D(0.f, 0.f, 0.f), g_dt);
			//position.pos.x += rigidbody.velocity.x * g_dt;
			//position.pos.y += rigidbody.velocity.y * g_dt;
		}
	}

}