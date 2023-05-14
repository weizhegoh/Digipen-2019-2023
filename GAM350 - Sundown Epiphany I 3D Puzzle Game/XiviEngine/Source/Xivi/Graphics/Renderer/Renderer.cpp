#include <pch.h>
//#include "Renderer.h"
//#include "SceneData.h"
//#include "../Shader/glslshader.h"
//#include "../Gfx/Gfx.h"
//namespace Xivi::Graphics
//{
//	void Renderer::RenderAnimationModel(GLSLShader& shdrpgm, SceneData& sceneData)
//	{
//		auto& camera = sceneData.cameraScene;
//		auto& modelList = sceneData.animationScene;
//
//		shdrpgm.Use();
//		for (int i = 0; i < modelList.size(); ++i)
//		{
//			auto& scene = modelList[i];
//			Gfx::LightRenderer::SetLightOn(scene.onLight, shdrpgm);
//			if(scene.onLight)
//				PassLight(shdrpgm, i, sceneData);
//			shdrpgm.SetUniform("uColor", scene.colour);
//			Gfx::Renderer::SetTransform(scene.animator.GetFinalBoneMatrices(), shdrpgm);
//			Gfx::Renderer::SetTransform(scene.transformMatrix, camera.viewMat, camera.projMat, shdrpgm);
//			Gfx::Renderer::BindMaterialTexture(scene.diffuse, scene.specular, scene.shinness, shdrpgm);
//			scene.model.ourModel.Draw();
//			Gfx::Renderer::UnBindTexture();
//		}
//		shdrpgm.UnUse();
//	}
//
//	void Renderer::RenderStaticModel(GLSLShader& shdrpgm, SceneData& sceneData)
//	{
//		auto& camera = sceneData.cameraScene;
//		auto& modelList = sceneData.staticScene;
//
//		shdrpgm.Use();
//		for (int i = 0; i < modelList.size(); ++i)
//		{
//			auto& model = modelList[i];
//			Gfx::LightRenderer::SetLightOn(model.onLight, shdrpgm);
//			if (model.onLight)
//				PassLight(shdrpgm, i, sceneData);
//			shdrpgm.SetUniform("uColor", model.colour);
//			Gfx::Renderer::SetTransform(model.transformMatrix, camera.viewMat, camera.projMat, shdrpgm);
//			Gfx::Renderer::BindMaterialTexture(model.diffuse, model.specular, model.shinness, shdrpgm);
//			model.models.Draw();
//			Gfx::Renderer::UnBindTexture();
//		}
//		shdrpgm.UnUse();
//	}
//
//	void Renderer::RenderModel(GLSLShader& shdrpgm, SceneData& sceneData)
//	{
//		auto& camera = sceneData.cameraScene;
//		auto& modelList = sceneData.modelScene;
//
//		shdrpgm.Use();
//		for (int i = 0; i < modelList.size(); ++i)
//		{
//			auto& model = modelList[i];
//			shdrpgm.SetUniform("uColor", model.colour);
//			Gfx::Renderer::SetTransform(model.transformMatrix, camera.viewMat, camera.projMat, shdrpgm);
//			Gfx::Renderer::BindMaterialTexture(model.diffuse, model.specular, model.shinness, shdrpgm);
//			model.models.Draw();
//			Gfx::Renderer::UnBindTexture();
//		}
//		shdrpgm.UnUse();
//	}
//
//	void Renderer::PassLight(GLSLShader& shdrpgm, size_t index, SceneData& sceneData)
//	{
//		Gfx::LightRenderer::SetBlinnPhong(true, shdrpgm);
//		auto& camera = sceneData.cameraScene;
//		auto& dirlight = sceneData.dirLight;
//		Gfx::LightRenderer::SetViewPos(camera.position, shdrpgm);
//		dirlight.direction = sceneData.animationScene[index].position - sceneData.dirLight.position;
//		Gfx::LightRenderer::SetDirLights(dirlight, shdrpgm);
//		Gfx::LightRenderer::SetPointLights(sceneData.pointLight, shdrpgm);
//		Gfx::LightRenderer::SetSpotLight(camera.position, camera.direction, sceneData.spotLight, shdrpgm);
//	}
//
//}