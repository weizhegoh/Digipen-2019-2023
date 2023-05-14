#include <pch.h>
//#include "SceneData.h"
//#include "../Model/Model.h"
//#include "../Model/Animation/Animator.h"
//#include "../Light/LightProperties.h"
//#include "../Texture/Texture.h"
//#include "../Shader/glslshader.h"
//
//namespace Xivi::Graphics
//{
//	void SceneData::SetCamera(
//		const Vec3f& pos,
//		const Math::Matrix4& view,
//		const Math::Matrix4& proj)
//	{
//		cameraScene.position = pos;
//		cameraScene.viewMat = view;
//		cameraScene.projMat = proj;
//	}
//
//	void SceneData::AddPointLight(const PointLight& light)
//	{
//		pointLight.emplace_back(light);
//	}
//
//	void SceneData::AddDirLight(const DirLight& light)
//	{
//		dirLight.emplace_back(light);
//	}
//
//	void SceneData::AddSpotLight(const SpotLight& light)
//	{
//		spotLight = light;
//	}
//
//	void SceneData::AddAnimationScene(const AnimationScene& scene)
//	{
//		animationScene.emplace_back(scene);
//	}
//
//	void SceneData::AddStaticScene(const StaticScene& scene)
//	{
//		staticScene.emplace_back(scene);
//	}
//}