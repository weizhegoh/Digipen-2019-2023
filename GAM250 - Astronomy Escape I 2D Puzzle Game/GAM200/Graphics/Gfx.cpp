/*****************************************************************
*\file		 Gfx.cpp
*\brief		 Gfx functions definitions.

*\author(s)   Lee Liang Ping	374 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Gfx.h>
namespace Graphics
{

	GLuint Gfx::GfxLoadTexture(const char* fileName)
	{
		std::cout << "Loading Image file " << fileName << std::endl;
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib(0);
		BYTE* bits(0);
		unsigned int width, height;
		width = height = 0;
		fif = FreeImage_GetFileType(fileName, 0);

		if (fif == FIF_UNKNOWN)
		{
			fif = FreeImage_GetFIFFromFilename(fileName);
			std::cout << "Getting Image Type" << std::endl;
		}
		else
			std::cout << "Failed to get image type" << std::endl;
		ARTLESS_ASSERT(fif == 13, "Image Format is PNG");
		if (FreeImage_FIFSupportsReading(fif))
		{
			dib = FreeImage_Load(fif, fileName);
			std::cout << "Trying to load image: " << std::endl;

		}
		if (!dib)
			ARTLESS_ASSERT(1, "Failed to load image");
		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		std::cout << "Image Height: " << height << "  Width: " << width << std::endl;
		std::cout << "Texture Loaded, generating texture..." << std::endl;
		GLuint texobj_hdl;
		// encapsulate 2d textures
		glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
		glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height,
			GL_BGRA_EXT, GL_UNSIGNED_BYTE, bits);
		FreeImage_Unload(dib);
		return texobj_hdl;
	}

	Gfx::Model Gfx::GfxLoadFixedModel(const char* fileName)
	{
		std::ifstream ifs{ fileName, std::ios::in };
		if (!ifs) {
			std::cout << "ERROR: Unable to open mesh file: "
				<< fileName << "\n";
			exit(EXIT_FAILURE);
		}
		ifs.seekg(0, std::ios::beg);
		std::string line;
		getline(ifs, line); // first line is count of objects in scene
		std::istringstream line_sstm{ line };

		if (!line.compare(fileName)) {
			std::cout << "Different mesh file: " << fileName << std::endl;
			return Gfx::Model();
		}

		glm::vec3 min = { 0.f, 0.f, 0.f };
		glm::vec3 max = { 0.f, 0.f, 0.f };
		std::vector<glm::vec3> pos_vtx;
		std::vector<GLushort> idx_vtx;
		GLenum primi_type{};
		GLuint count{ 0 };
		GLuint counter{ 0 };
		while (!ifs.eof()) {
			getline(ifs, line);
			std::istringstream line_meshvalue{ line };
			char prefix;
			line_meshvalue >> prefix;
			switch (prefix)
			{
			case 'v':
				float vtx_1, vtx_2;
				line_meshvalue >> vtx_1;
				line_meshvalue >> vtx_2;

				if (vtx_1 < min.x)
					min.x = vtx_1;
				else if (vtx_1 > max.x)
					max.x = vtx_1;
				if (vtx_2 < min.y)
					min.y = vtx_2;
				else if (vtx_2 > max.y)
					max.y = vtx_2;

				pos_vtx.push_back({ vtx_1 , vtx_2, 0.f });
				break;
			case 't':
				GLushort idx;
				line_meshvalue >> idx;
				idx_vtx.push_back(idx);
				count++;

				line_meshvalue >> idx;
				idx_vtx.push_back(idx);
				count++;

				line_meshvalue >> idx;
				idx_vtx.push_back(idx);
				count++;

				primi_type = GL_TRIANGLES;
				break;
			case 'f':
				if (counter++ == 0)
				{
					line_meshvalue >> idx;
					idx_vtx.push_back(idx);
					count++;
					line_meshvalue >> idx;
					idx_vtx.push_back(idx);
					count++;
				}
				line_meshvalue >> idx;
				idx_vtx.push_back(idx);
				count++;

				primi_type = GL_TRIANGLE_FAN;
				break;
			}
		}

		//calculate center
		glm::vec3 center = min + max;
		center *= 0.5f;
		//calculate unitScale
		float unitScale = 1 / glm::compMax(max - min);

		for (auto& x : pos_vtx)
		{
			x -= center;
			x *= unitScale;
		}

		// set up VAO
		GLuint vbo_hdl;
		glCreateBuffers(1, &vbo_hdl);
		glNamedBufferStorage(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());
		GLuint vao_hdl;
		// create vertexArray for the point
		glCreateVertexArrays(1, &vao_hdl);
		glEnableVertexArrayAttrib(vao_hdl, 0);
		glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
		glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao_hdl, 0, 0);
		glBindVertexArray(0);

		GLuint ebo_hdl; glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_vtx.size(),
			reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
		glBindVertexArray(0);

		//Setting up information and addresses for mdl
		Gfx::Model mdl;
		mdl.vaoid = vao_hdl;
		mdl.draw_cnt = count;
		mdl.primitive_type = primi_type;
		mdl.primitive_cnt = static_cast<GLuint>(pos_vtx.size());
		return mdl;
	}

	Gfx::Model Gfx::GfxLoadFixedModel2()
	{
		std::array<Gfx::Model::Vertex, 4> vertex;
		vertex[0].position = glm::vec3(-0.5f, -0.5f, 0.0f);
		vertex[1].position = glm::vec3(0.5f, -0.5f, 0.0f);
		vertex[2].position = glm::vec3(0.5f, 0.5f, 0.0f);
		vertex[3].position = glm::vec3(-0.5f, 0.5f, 0.0f);

		//storing colour
		vertex[0].colour = glm::vec3(1.f, 0.f, 0.f);
		vertex[1].colour = glm::vec3(0.f, 1.f, 0.f);
		vertex[2].colour = glm::vec3(0.f, 0.f, 1.f);
		vertex[3].colour = glm::vec3(1.f, 0.f, 1.f);
		//storing texture coordinates


		vertex[0].texture_coord = glm::vec2(0.0f, 0.0f);
		vertex[1].texture_coord = glm::vec2(1.0f, 0.0f);
		vertex[2].texture_coord = glm::vec2(1.0f, 1.0f);
		vertex[3].texture_coord = glm::vec2(0.0f, 1.0f);

		GLuint vbo_hdl = 0;
		std::array<GLushort, 6> idx_vtx;

		//setting up rectangle topology
		idx_vtx[0] = 0; idx_vtx[1] = 1; idx_vtx[2] = 2;
		idx_vtx[3] = 2; idx_vtx[4] = 3; idx_vtx[5] = 0;


		GLsizeiptr buffer_size = sizeof(Gfx::Model::Vertex) * vertex.size();
		glCreateBuffers(1, &vbo_hdl);
		//allows storage to the server side (gpu)
		glNamedBufferStorage(vbo_hdl, buffer_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vbo_hdl, 0, buffer_size, vertex.data());

		//creating a vertex array object
		glCreateVertexArrays(1, &vbo_hdl);
		//creating a vertex array buffer
		glVertexArrayVertexBuffer(vbo_hdl, 0, vbo_hdl, 0, sizeof(Model::Vertex));

		//binding position coordinates into the buffer
		glEnableVertexArrayAttrib(vbo_hdl, 0);
		glVertexArrayAttribFormat(vbo_hdl, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, Model::Vertex::position));
		glVertexArrayAttribBinding(vbo_hdl, 0, 0);

		//binding colour coordinates into the buffer
		glEnableVertexArrayAttrib(vbo_hdl, 1);
		glVertexArrayAttribFormat(vbo_hdl, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, Model::Vertex::colour));
		glVertexArrayAttribBinding(vbo_hdl, 1, 0);

		//binding texture coordinates into the buffer
		glEnableVertexArrayAttrib(vbo_hdl, 2);
		glVertexArrayAttribFormat(vbo_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, Model::Vertex::texture_coord));
		glVertexArrayAttribBinding(vbo_hdl, 2, 0);
		glBindVertexArray(0);

		GLuint ebo_hdl; glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_vtx.size(), reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(vbo_hdl, ebo_hdl);
		glBindVertexArray(0);

		//Setting up information and addresses for mdl
		Gfx::Model mdl;
		mdl.vaoid = vbo_hdl;
		mdl.primitive_type = GL_TRIANGLES; //type of rendered object
		mdl.draw_cnt = static_cast<GLuint>(idx_vtx.size()); //number of vertices
		mdl.primitive_cnt = static_cast<GLuint>(vertex.size()); //number of vertices
		return mdl; //returns the rendered information
	}

	Gfx::Model Gfx::GfxBuildHotspotModel()
	{
		float scale = 1.f;
		std::vector<glm::vec3> pos_vtx;

		pos_vtx.push_back(glm::vec3(scale / 2, scale / 4, 0.0f));
		pos_vtx.push_back(glm::vec3(scale / 2, -scale / 4, 0.0f));
		pos_vtx.push_back(glm::vec3(-scale / 2, -scale / 4, 0.0f));
		pos_vtx.push_back(glm::vec3(-scale / 2, scale / 4, 0.0f));
		pos_vtx.push_back(glm::vec3(scale / 4, scale / 2, 0.0f));
		pos_vtx.push_back(glm::vec3(-scale / 4, scale / 2, 0.0f));
		pos_vtx.push_back(glm::vec3(scale / 4, -scale / 2, 0.0f));
		pos_vtx.push_back(glm::vec3(-scale / 4, -scale / 2, 0.0f));

		// set up VAO
		GLuint vbo_hdl;
		glCreateBuffers(1, &vbo_hdl);
		glNamedBufferStorage(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());
		GLuint vao_hdl;
		// create vertexArray for the point
		glCreateVertexArrays(1, &vao_hdl);
		glEnableVertexArrayAttrib(vao_hdl, 0);
		glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
		glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao_hdl, 0, 0);
		glBindVertexArray(0);

		Gfx::Model mdl;
		mdl.vaoid = vao_hdl;
		mdl.primitive_type = GL_POINTS; //type of rendered object
		mdl.draw_cnt = static_cast<GLuint>(pos_vtx.size()); //number of vertices
		mdl.primitive_cnt = 8; //number of vertices
		return mdl; //returns the rendered information
	}

	void Gfx::RenderFunctions::ToggleBlender(bool set)
	{
		if (set)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void Gfx::RenderFunctions::ClearBitBuffer(unsigned int set)
	{
		glClear(set);
	}

	void Gfx::RenderFunctions::BindTexture(GLuint texture)
	{
		glBindTextureUnit(6, texture);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void Gfx::RenderFunctions::BindVertexArray(GLuint vaoid)
	{
		glBindVertexArray(vaoid);
	}

	void Gfx::RenderFunctions::SetTransform(
		glm::mat4 modelMxt44,
		glm::mat4 viewMxt44,
		glm::mat4 projectionMxt44,
		GLSLShader& shdrpgm
	)
	{
		shdrpgm.SetUniform("uModel", modelMxt44);
		shdrpgm.SetUniform("uView", viewMxt44);
		shdrpgm.SetUniform("uProjection", projectionMxt44);
	}

	void Gfx::RenderFunctions::SetRenderScreen(FreeRenderType type, GLSLShader& shdrpgm)
	{
		if (type == FreeRenderType::MODEL_WORLD)
			shdrpgm.SetUniform("uScreenRender", 0);
		else if (type == FreeRenderType::SCREEN)
			shdrpgm.SetUniform("uScreenRender", 1);
		else
			shdrpgm.SetUniform("uScreenRender", 0);
	}

	void Gfx::RenderFunctions::DrawTexure(Model model)
	{
		glDrawElements(model.primitive_type, model.draw_cnt, GL_UNSIGNED_SHORT, NULL);
	}

	void Gfx::RenderFunctions::DrawArray(Model model, DrawOption option)
	{
		GLenum pOption;
		if (option == DrawOption::DEFAULT)
			pOption = model.primitive_type;
		else
			pOption = GetOption(option);
		glDrawArrays(pOption, NULL, model.draw_cnt);
	}

	void Gfx::RenderFunctions::DrawElements(Model model, DrawOption option)
	{
		GLenum pOption;
		if (option == DrawOption::DEFAULT)
			pOption = model.primitive_type;
		else
			pOption = GetOption(option);
		glDrawElements(pOption, model.draw_cnt, GL_UNSIGNED_SHORT, NULL);
	}

	GLenum Gfx::RenderFunctions::GetOption(DrawOption option)
	{
		switch (option)
		{
		case DrawOption::POINTS:
			return GL_POINTS;
			break;
		case DrawOption::LINE_STRIP:
			return GL_LINE_STRIP;
			break;
		case DrawOption::DEFAULT:
			return GL_LINE_STRIP;
			break;
		default:
			return GL_NONE;
			break;
		}
	}

	void Gfx::RenderFunctions::DeleteVAO(GLuint id)
	{
		GLint max_vtx_attrib = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attrib);
		glBindVertexArray(id);
		for (int i = 0; i < max_vtx_attrib; ++i) {
			GLuint vbo_handle = 0;
			glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo_handle);
			if (vbo_handle > 0) {
				glDeleteBuffers(1, &vbo_handle);
			}
		}
		glDeleteBuffers(1, &id);
	}

	void Gfx::RenderFunctions::Draw(
		Model model,
		GLSLShader* shdrpgm,
		glm::mat4& modelMxt33,
		glm::mat4& viewMxt33,
		glm::mat4& projectionMxt33,
		glm::vec3& colour
	)
	{
		shdrpgm->Use();
		glBindVertexArray(model.vaoid);
		//passing variable to vertex shader
		shdrpgm->SetUniform("uModel", modelMxt33);
		shdrpgm->SetUniform("uView", viewMxt33);
		shdrpgm->SetUniform("uProjection", projectionMxt33);
		shdrpgm->SetUniform("uColor", colour);
		glDrawElements(
			model.primitive_type,
			model.primitive_cnt,
			GL_UNSIGNED_SHORT, NULL);
		glBindVertexArray(0);
		shdrpgm->UnUse();
	}

	void Gfx::RenderFunctions::SetTransparency(float transparent, GLSLShader& shdrpgm)
	{
		shdrpgm.SetUniform("transparency", transparent);
	}

	void Gfx::RenderFunctions::SetLight(PointLight& pointLight, glm::vec3 position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm)
	{
		std::string text = "pointLights[";
		text += std::to_string(lightIndex);
		std::string amb = text + "].ambient";
		shdrpgm.SetUniform(amb.c_str(), pointLight.ambient);

		std::string constant = text + "].constant";
		shdrpgm.SetUniform(constant.c_str(), pointLight.attenuationEq.x);

		std::string linear = text + "].linear";
		shdrpgm.SetUniform(linear.c_str(), pointLight.attenuationEq.y);

		std::string quadratic = text + "].quadratic";
		shdrpgm.SetUniform(quadratic.c_str(), pointLight.attenuationEq.z);

		std::string colourtext = text + "].color";
		shdrpgm.SetUniform(colourtext.c_str(), colour);

		std::string pos = text + "].position";
		shdrpgm.SetUniform(pos.c_str(), position);

		lightIndex++;
	}

	void Gfx::RenderFunctions::SetLight(PointLight& pointLight, mathEngine::Vector3D position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm)
	{
		SetLight(pointLight, glm::vec3(position.x, position.y, position.z), colour, lightIndex, shdrpgm);
	}

}
