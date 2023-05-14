//#pragma once
#include <pch.h>
//#include "../Model/Animation/Animator.h"
//#include "../Texture/Texture.h"
//#include "../Model/Model.h"
//namespace Xivi::Graphics
//{
//	struct PointLight;
//	struct DirLight;
//	struct SpotLight;
//
//	struct AnimationScene
//	{
//		Vec3f position;
//		AnimationModel model;
//		Animator animator;
//		Math::Matrix4 transformMatrix;
//		Vec3f colour;
//		TexturePtr diffuse;
//		TexturePtr specular;
//		float shinness;
//		bool onLight;
//
//	};
//
//	struct StaticScene
//	{
//		Vec3f position;
//		StaticModel models;
//		Math::Matrix4 transformMatrix;
//		Vec3f colour;
//		TexturePtr diffuse;
//		TexturePtr specular;
//		float shinness;
//		bool onLight;
//	};
//
//	struct CameraScene
//	{
//		Vec3f position;
//		Vec3f direction;
//		Math::Matrix4 viewMat;
//		Math::Matrix4 projMat;
//
//	};
//	struct SceneData
//	{
//
//		std::vector<AnimationScene> animationScene;
//		std::vector<StaticScene> staticScene;
//		std::vector<StaticScene> modelScene;
//		PointLights pointLight;
//		DirLight dirLight;
//		SpotLight spotLight;
//		CameraScene cameraScene;
//
//		void SetCamera(
//			const Vec3f& pos,
//			const Math::Matrix4& view,
//			const Math::Matrix4& proj);
//		void AddPointLight(const PointLight& light);
//		void AddDirLight(const DirLight& light);
//		void AddSpotLight(const SpotLight& light);
//		void AddAnimationScene(const AnimationScene& scene);
//		void AddStaticScene(const StaticScene& scene);
//
//
//	};
//
//
//
//}