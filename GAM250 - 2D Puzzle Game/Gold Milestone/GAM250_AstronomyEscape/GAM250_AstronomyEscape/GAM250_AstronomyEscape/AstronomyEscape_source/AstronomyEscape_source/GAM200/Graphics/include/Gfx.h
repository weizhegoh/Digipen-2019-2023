/*****************************************************************
*\file		 Gfx.h
*\brief		 Header file declarations for Gfx.

*\author(s)   Lee Liang Ping	61 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <glslshader.h>
#include <stb_image.h>
#include <FreeImage.h>
#include <GameTypes.h>
#include <PointLight.h>

#define STB_IMAGE_IMPLEMENTATION

namespace Graphics
{
	struct Gfx
	{
		struct Model
		{
			GLenum primitive_type;
			GLuint primitive_cnt;
			GLuint vaoid;
			GLuint draw_cnt;
			bool texturecheck;

			struct Vertex
			{
				glm::vec3 position;
				glm::vec3 colour;
				glm::vec2 texture_coord;
			};
		};
		static GLuint GfxLoadTexture(const char* fileName);
		static Gfx::Model GfxLoadFixedModel(const char* fileName);
		static Gfx::Model GfxLoadFixedModel2();
		static Gfx::Model GfxBuildHotspotModel();

		struct RenderFunctions
		{
			void ToggleBlender(bool set);
			void ClearBitBuffer(unsigned int set);

			void SetTransform(
				glm::mat4 modelMxt44, 
				glm::mat4 viewMxt44, 
				glm::mat4 projectionMxt44,
				GLSLShader& shdrpgm);
			void SetRenderScreen(FreeRenderType type, GLSLShader& shdrpgm);

			void DrawTexure(Model model);
			void DrawArray(Model model, DrawOption option);
			void DrawElements(Model model, DrawOption option);
			void BindTexture(GLuint texture);
			void BindVertexArray(GLuint vaoid);
			void DeleteVAO(GLuint id);
			void Draw(
				Model model,
				GLSLShader* shdrpgm,
				glm::mat4& modelMxt33,
				glm::mat4& viewMxt33,
				glm::mat4& projectionMxt33,
				glm::vec3& colour);
			
			void SetTransparency(float transparent, GLSLShader& shdrpgm);
			void SetLight(PointLight& pointLight, glm::vec3 position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm);
			void SetLight(PointLight& pointLight, mathEngine::Vector3D position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm);
		private:
			GLenum GetOption(DrawOption option);
		};
		
	};
}