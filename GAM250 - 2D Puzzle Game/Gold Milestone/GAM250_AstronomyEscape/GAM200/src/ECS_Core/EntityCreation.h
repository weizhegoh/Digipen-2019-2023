/*****************************************************************
*\file		 EntityCreation.h
*\brief		 Entity Creation Helper Functions

*\author(s)   Lee Liang Ping	272 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

namespace EntityCreation
{
	struct any
	{
		enum type
		{
			entityType,
			render,
			transform,
			position,
			rigidbody,
			aabb,
			colour,
			light,
			flags,
			ai,
			ui,
			collisionFlag,
			inventory,
			laser,
			state,
			lifetime,
			particle_state,
			particle_type,
			animation,
			renderpos,
			rendertex,
			linesegment,
			parentID,
			force,
			loadingtype,
			comet,
			pointlight,
			lightFlag,
			lever,
			teleporter,
			ray,
			particle_actions,
			physicflag,

			et_lightnormal,
			et_light_mainChar,
			et_light_keyobjective,
			et_light_keyobjective_parts
		};
		any(EntityType e) { m_data.entityType = e; m_type = entityType; }
		any(Render e) { m_data.render = e; m_type = render; }
		any(Transform e) { m_data.transform = e; m_type = transform; }
		any(Position e) { m_data.position = e; m_type = position; }
		any(Rigidbody e) { m_data.rigidbody = e; m_type = rigidbody; }
		any(AABB e) { m_data.aabb = e; m_type = aabb; }
		any(Colour e) { m_data.colour = e; m_type = colour; }
		any(Light e) { m_data.light = e; m_type = light; }
		any(Flags e) { m_data.flags = e; m_type = flags; }
		any(AI e) { m_data.ai = e; m_type = ai; }
		any(UI e) { m_data.ui = e; m_type = ui; }
		any(CollisionFlag e) { m_data.collisionFlag = e; m_type = collisionFlag; }
		any(Inventory e) { m_data.inventory = e; m_type = inventory; }
		any(Laser e) { m_data.laser = e; m_type = laser; }
		any(State e) { m_data.state = e; m_type = state; }
		any(LifeTime e) { m_data.lifetime = e; m_type = lifetime; }
		any(ParticleStates e) { m_data.particle_state = e; m_type = particle_state; }
		any(ParticleType e) { m_data.particle_type = e; m_type = particle_type; }
		any(Animation e) { m_data.animation = e; m_type = animation; }
		any(RenderPos e) { m_data.renderpos = e; m_type = renderpos; }
		any(RenderTex e) { m_data.rendertex = e; m_type = rendertex; }
		any(LineSegment e) { m_data.linesegment = e; m_type = linesegment; }
		any(ParentId e) { m_data.parentID = e; m_type = parentID; }
		any(Force e) { m_data.force = e; m_type = force; }
		any(LoadingType e) { m_data.loadingtype = e; m_type = loadingtype; }
		any(Comet e) { m_data.comet = e; m_type = comet; }
		any(PointLight e) { m_data.pointlight = e; m_type = pointlight; }
		any(LightFlag e) { m_data.lightFlag = e; m_type = lightFlag; }
		any(Lever e) { m_data.lever = e; m_type = lever; }
		any(Teleporter e) { m_data.teleporter = e; m_type = teleporter; }
		any(Ray e) { m_data.ray = e; m_type = ray; }
		any(ParticleActions e) { m_data.particle_actions = e; m_type = particle_actions; }
		any(PhysicFlag e) { m_data.physicflag = e; m_type = physicflag; }

		//Light Entity Type
		any(ET_LightNormal e) { m_data.et_lightnormal = e; m_type = et_lightnormal; }
		any(ET_Light_MainChar e) { m_data.et_light_mainChar = e; m_type = et_light_mainChar; }
		any(ET_Light_KeyObjectives e) { m_data.et_light_keyobjective = e; m_type = et_light_keyobjective; }
		any(ET_Light_KeyObjectives_Parts e) { m_data.et_light_keyobjective_parts = e; m_type = et_light_keyobjective_parts; }

		type get_type() const { return m_type; }
		EntityType get_entity() const { return m_data.entityType; }
		Render get_render() const { return m_data.render; }
		Transform get_transform() const { return m_data.transform; }
		Position get_position() const { return m_data.position; }
		Rigidbody get_rigidbody() const { return m_data.rigidbody; }
		AABB get_aabb() const { return m_data.aabb; }
		Colour get_colour() const { return m_data.colour; }
		Light get_light() const { return m_data.light; }
		Flags get_flags() const { return m_data.flags; }
		AI get_ai() const { return m_data.ai; }
		UI get_ui() const { return m_data.ui; }
		CollisionFlag get_collisionflag() const { return m_data.collisionFlag; }
		Inventory get_inventory() const { return m_data.inventory; }
		Laser get_laser() const { return m_data.laser; }
		State get_state() const { return m_data.state; }
		LifeTime get_lifetime() const { return m_data.lifetime; }
		ParticleStates get_particlestate() const { return m_data.particle_state; }
		ParticleType get_particletype() const { return m_data.particle_type; }
		Animation get_animation() const { return m_data.animation; }
		RenderPos get_renderpos() const { return m_data.renderpos; }
		RenderTex get_rendertex() const { return m_data.rendertex; }
		LineSegment get_linesegment() const { return m_data.linesegment; }
		ParentId get_parentId() const { return m_data.parentID; }
		Force get_force() const { return m_data.force; }
		LoadingType get_loadaingType() const { return m_data.loadingtype; }
		Comet get_comet() const { return m_data.comet; }
		PointLight get_pointLight() const { return m_data.pointlight; }
		LightFlag get_lightFlag() const { return m_data.lightFlag; }
		Lever get_lever() const { return m_data.lever; }
		Teleporter get_teleporter() const { return m_data.teleporter; }
		Ray get_ray() const { return m_data.ray; }
		ParticleActions get_pActions() const { return m_data.particle_actions; }
		PhysicFlag get_physicflag() const { return m_data.physicflag; }


		//Light Entity Type
		ET_LightNormal get_et_LightNormal() const { return m_data.et_lightnormal; }
		ET_Light_MainChar get_et_light_mainChar() const { return m_data.et_light_mainChar; }
		ET_Light_KeyObjectives get_et_light_keyobjective() const { return m_data.et_light_keyobjective; }
		ET_Light_KeyObjectives_Parts get_et_light_keyobjective_parts() const { return m_data.et_light_keyobjective_parts; }
	
	private:
		type m_type;
		struct M_data
		{
			EntityType entityType;
			Render render;
			Transform transform;
			Position position;
			Rigidbody rigidbody;
			AABB aabb;
			Colour colour;
			Light light;
			Flags flags;
			AI ai;
			UI ui;
			CollisionFlag collisionFlag;
			Inventory inventory;
			Laser laser;
			State state;
			LifeTime lifetime;
			ParticleStates particle_state;
			ParticleType particle_type;
			Animation animation;
			RenderPos renderpos;
			RenderTex rendertex;
			LineSegment linesegment;
			ParentId parentID;
			Force force;
			LoadingType loadingtype;
			Comet comet;
			PointLight pointlight;
			LightFlag lightFlag;
			Lever lever;
			Teleporter teleporter;
			Ray ray;
			ParticleActions particle_actions;
			PhysicFlag physicflag;

			ET_LightNormal et_lightnormal;
			ET_Light_MainChar et_light_mainChar;
			ET_Light_KeyObjectives et_light_keyobjective;
			ET_Light_KeyObjectives_Parts et_light_keyobjective_parts;
		};
		M_data m_data;
	};

	template <class ...Args>
	Entity CreateEntity_imp(EntitySystemOption systemoption, const Args&... args)
	{
		auto& ecsGameEngine = gFakeEngine->GetECSManager<ECSManager>(systemoption);
		Entity entity = ecsGameEngine.CreateEntity();
		std::vector<any> vec = { args... };
		for (unsigned i = 0; i < vec.size(); ++i) {
			switch (vec[i].get_type()) {
			case any::entityType:				ecsGameEngine.AddComponent(entity, vec[i].get_entity()); break;
			case any::render:					ecsGameEngine.AddComponent(entity, vec[i].get_render()); break;
			case any::transform:				ecsGameEngine.AddComponent(entity, vec[i].get_transform()); break;
			case any::position:					ecsGameEngine.AddComponent(entity, vec[i].get_position()); break;
			case any::rigidbody:				ecsGameEngine.AddComponent(entity, vec[i].get_rigidbody()); break;
			case any::aabb:						ecsGameEngine.AddComponent(entity, vec[i].get_aabb()); break;
			case any::colour:					ecsGameEngine.AddComponent(entity, vec[i].get_colour()); break;
			case any::light:					ecsGameEngine.AddComponent(entity, vec[i].get_light()); break;
			case any::flags:					ecsGameEngine.AddComponent(entity, vec[i].get_flags()); break;
			case any::ai:						ecsGameEngine.AddComponent(entity, vec[i].get_ai()); break;
			case any::collisionFlag:			ecsGameEngine.AddComponent(entity, vec[i].get_collisionflag()); break;
			case any::inventory:				ecsGameEngine.AddComponent(entity, vec[i].get_inventory()); break;
			case any::laser:					ecsGameEngine.AddComponent(entity, vec[i].get_laser()); break;
			case any::state:					ecsGameEngine.AddComponent(entity, vec[i].get_state()); break;
			case any::lifetime:					ecsGameEngine.AddComponent(entity, vec[i].get_lifetime()); break;
			case any::particle_state:			ecsGameEngine.AddComponent(entity, vec[i].get_particlestate()); break;
			case any::particle_type:			ecsGameEngine.AddComponent(entity, vec[i].get_particletype()); break;
			case any::animation:				ecsGameEngine.AddComponent(entity, vec[i].get_animation()); break;
			case any::renderpos:				ecsGameEngine.AddComponent(entity, vec[i].get_renderpos()); break;
			case any::rendertex:				ecsGameEngine.AddComponent(entity, vec[i].get_rendertex()); break;
			case any::linesegment:				ecsGameEngine.AddComponent(entity, vec[i].get_linesegment()); break;
			case any::parentID:					ecsGameEngine.AddComponent(entity, vec[i].get_parentId()); break;
			case any::force:					ecsGameEngine.AddComponent(entity, vec[i].get_force()); break;
			case any::loadingtype:				ecsGameEngine.AddComponent(entity, vec[i].get_loadaingType()); break;
			case any::comet:					ecsGameEngine.AddComponent(entity, vec[i].get_comet()); break;
			case any::pointlight:				ecsGameEngine.AddComponent(entity, vec[i].get_pointLight()); break;
			case any::et_lightnormal:			ecsGameEngine.AddComponent(entity, vec[i].get_et_LightNormal()); break;
			case any::et_light_mainChar:		ecsGameEngine.AddComponent(entity, vec[i].get_et_light_mainChar()); break;
			case any::et_light_keyobjective:	ecsGameEngine.AddComponent(entity, vec[i].get_et_light_keyobjective()); break;
			case any::lightFlag:				ecsGameEngine.AddComponent(entity, vec[i].get_lightFlag()); break;
			case any::ray:						ecsGameEngine.AddComponent(entity, vec[i].get_ray()); break;
			case any::particle_actions:			ecsGameEngine.AddComponent(entity, vec[i].get_pActions()); break;
			case any::et_light_keyobjective_parts:			ecsGameEngine.AddComponent(entity, vec[i].get_et_light_keyobjective_parts()); break;
			case any::physicflag:				ecsGameEngine.AddComponent(entity, vec[i].get_physicflag()); break;
			}
		}
		return entity;
	}

	template <class ...Args>
	Entity CreateEntity(EntitySystemOption systemoption, Args... args)
	{
		return CreateEntity_imp(systemoption, any(args)...);
	}

	template <class ...Args>
	void AssignEntity_imp(EntitySystemOption systemoption, Entity entity, const Args&... args)
	{
		auto& ecsGameEngine = gFakeEngine->GetECSManager<ECSManager>(systemoption);
		std::vector<any> vec = { args... };
		for (unsigned i = 0; i < vec.size(); ++i) {
			switch (vec[i].get_type()) {
			case any::entityType:				ecsGameEngine.GetComponent<EntityType>(entity) = vec[i].get_entity(); break;
			case any::render:					ecsGameEngine.GetComponent<Render>(entity) = vec[i].get_render(); break;
			case any::transform:				ecsGameEngine.GetComponent<Transform>(entity) = vec[i].get_transform(); break;
			case any::position:					ecsGameEngine.GetComponent<Position>(entity) = vec[i].get_position(); break;
			case any::rigidbody:				ecsGameEngine.GetComponent<Rigidbody>(entity) = vec[i].get_rigidbody(); break;
			case any::aabb:						ecsGameEngine.GetComponent<AABB>(entity) = vec[i].get_aabb(); break;
			case any::colour:					ecsGameEngine.GetComponent<Colour>(entity) = vec[i].get_colour(); break;
			case any::light:					ecsGameEngine.GetComponent<Light>(entity) = vec[i].get_light(); break;
			case any::flags:					ecsGameEngine.GetComponent<Flags>(entity) = vec[i].get_flags(); break;
			case any::ai:						ecsGameEngine.GetComponent<AI>(entity) = vec[i].get_ai(); break;
			case any::ui:						ecsGameEngine.GetComponent<UI>(entity) = vec[i].get_ui(); break; 
			case any::collisionFlag:			ecsGameEngine.GetComponent<CollisionFlag>(entity) = vec[i].get_collisionflag(); break;
			case any::inventory:				ecsGameEngine.GetComponent<Inventory>(entity) = vec[i].get_inventory(); break;
			case any::laser:					ecsGameEngine.GetComponent<Laser>(entity) = vec[i].get_laser(); break;
			case any::state:					ecsGameEngine.GetComponent<State>(entity) = vec[i].get_state(); break;
			case any::lifetime:					ecsGameEngine.GetComponent<LifeTime>(entity) = vec[i].get_lifetime(); break;
			case any::particle_state:			ecsGameEngine.GetComponent<ParticleStates>(entity) = vec[i].get_particlestate(); break;
			case any::particle_type:			ecsGameEngine.GetComponent<ParticleType>(entity) = vec[i].get_particletype(); break;
			case any::animation:				ecsGameEngine.GetComponent<Animation>(entity) = vec[i].get_animation(); break;
			case any::renderpos:				ecsGameEngine.GetComponent<RenderPos>(entity) = vec[i].get_renderpos(); break;
			case any::rendertex:				ecsGameEngine.GetComponent<RenderTex>(entity) = vec[i].get_rendertex(); break;
			case any::linesegment:				ecsGameEngine.GetComponent<LineSegment>(entity) = vec[i].get_linesegment(); break;
			case any::parentID:					ecsGameEngine.GetComponent<ParentId>(entity) = vec[i].get_parentId(); break;
			case any::force:					ecsGameEngine.GetComponent<Force>(entity) = vec[i].get_force(); break;
			case any::loadingtype:				ecsGameEngine.GetComponent<LoadingType>(entity) = vec[i].get_loadaingType(); break;
			case any::comet:					ecsGameEngine.GetComponent<Comet>(entity) = vec[i].get_comet(); break;
			case any::pointlight:				ecsGameEngine.GetComponent<PointLight>(entity) = vec[i].get_pointLight(); break;
			case any::et_lightnormal:			ecsGameEngine.GetComponent<ET_LightNormal>(entity) = vec[i].get_et_LightNormal(); break;
			case any::et_light_mainChar:		ecsGameEngine.GetComponent<ET_Light_MainChar>(entity) = vec[i].get_et_light_mainChar(); break;
			case any::et_light_keyobjective:	ecsGameEngine.GetComponent<ET_Light_KeyObjectives>(entity) = vec[i].get_et_light_keyobjective(); break;
			case any::et_light_keyobjective_parts:	ecsGameEngine.GetComponent<ET_Light_KeyObjectives_Parts>(entity) = vec[i].get_et_light_keyobjective_parts(); break;
			case any::lightFlag:				ecsGameEngine.GetComponent<LightFlag>(entity) = vec[i].get_lightFlag(); break;
			case any::lever:					ecsGameEngine.GetComponent<Lever>(entity) = vec[i].get_lever(); break;
			case any::teleporter:				ecsGameEngine.GetComponent<Teleporter>(entity) = vec[i].get_teleporter(); break;
			case any::ray:						ecsGameEngine.GetComponent<Ray>(entity) = vec[i].get_ray(); break;
			case any::particle_actions:			std::cout << " Assign Your Own by calling .Add " << std::endl; break;
			case any::physicflag:				ecsGameEngine.GetComponent<PhysicFlag>(entity) = vec[i].get_physicflag(); break;
			}
		}
	}

	template <class ...Args>
	void AssignEntity(EntitySystemOption systemoption, Entity entity, Args... args)
	{
		AssignEntity_imp(systemoption, entity, any(args)...);
	}
}