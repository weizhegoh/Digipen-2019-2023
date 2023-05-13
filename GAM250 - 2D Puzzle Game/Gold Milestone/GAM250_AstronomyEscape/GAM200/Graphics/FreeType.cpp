//
///*****************************************************************
//*\file		 freetype.cpp
//*\brief		 freetype functions definitions.
//
//*\author(s)   lee liang ping	121 lines x 100% code contribution
//
//copyright (c) 2020 digipen institute of technology.
//reproduction or disclosure of this file or its contents without the
//prior written consent of digipen institute of technology is prohibited.
//*******************************************************************/
//
//#include <enginepch.h>
//#include <freetype.h>
//#include <fakeengine.h>
//
//extern std::unique_ptr<FakeEngine> gFakeEngine;
//void Graphics::FreeType::Init(std::string file)
//{
//	FT_Library ft;
//	FT_Face face;
//	if (FT_Init_FreeType(&ft))
//		std::cout << "error::freetype: could not init freetype library" << std::endl;
//	if (FT_New_Face(ft, file.c_str(), 0, &face))
//		std::cout << "error::freetype: failed to load font" << std::endl;
//	FT_Set_Pixel_Sizes(face, 0, 48);
//
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
//    
//
//	int texturewidth = 0, textureheight = 0;
//	for (unsigned char c = 0; c < 128; c++)
//	{
//		// load character glyph 
//		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//		{
//			std::cout << "error::freetytpe: failed to load glyph" << std::endl;
//			continue;
//		}
//		texturewidth += face->glyph->bitmap.width;
//		textureheight = std::max(textureheight, (int)face->glyph->bitmap.rows);
//	}
//    atlas_width = texturewidth;
//	atlas_height = textureheight;
//
//    
//    glActiveTexture(GL_TEXTURE0);
//	glGenTextures(1, &textureId);
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texturewidth, textureheight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	unsigned int x = 0;
//	Character character;
//	for (unsigned char c = 0; c < 128; ++c)
//	{
//		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//		{
//			std::cout << "error::freetytpe: failed to load glyph" << std::endl;
//			continue;
//		}
//        
//        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//        
//		character.texOffset = (float)x / texturewidth;
//		x += face->glyph->bitmap.width;
//
//		character.advance.x = face->glyph->advance.x >> 6;
//		character.advance.y = face->glyph->metrics.vertAdvance >> 6;
//
//		character.dimensions.x = face->glyph->bitmap.width;
//		character.dimensions.y = face->glyph->bitmap.rows;
//
//		character.bearing.x = face->glyph->bitmap_left;
//		character.bearing.y = face->glyph->bitmap_top;
//
//        Characters.insert(std::pair<GLchar, Character>(c, character));
//
//
//	}
//    
//    FT_Done_Face(face);
//	FT_Done_FreeType(ft);
//
//}
//
//void Graphics::FreeType::RenderText(std::string text, float x, float y, float scale, glm::vec3 color, FreeRenderType type)
//{
//	// activate corresponding render state	
//    
//    GLSLShader& shdrpgm = gFakeEngine->GetShader(ShaderOption::TEXT);
//	shdrpgm.Use();
//	shdrpgm.SetUniform("textcolor", color);
//
//	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
//	glm::mat4 model = glm::mat4(1.0f);
//	glm::mat4 projection;
//	if (FreeRenderType::MODEL_WORLD == type)
//	{
//		glm::vec3 glmposition = glm::vec3(x, y, -1.f);
//		model = glm::translate(model, glmposition);
//		glm::mat4 view = glm::lookAt(
//			glm::vec3(camera2d.pos.x, camera2d.pos.y, 0.f),
//			glm::vec3(camera2d.pos.x, camera2d.pos.y, 1.0f),
//			glm::vec3(0.f, 1.f, 0.f)); // up vector
//		glm::mat4 proj = glm::ortho(
//			float(camera2d.cam_window.x),
//			float(-camera2d.cam_window.x),
//			float(-camera2d.cam_window.y),
//			float(camera2d.cam_window.y));
//		projection = proj * view * model;
//
//	}
//	else if (FreeRenderType::SCREEN == type)
//	{
//		glm::vec3 glmposition = glm::normalize(glm::vec3(x, y, 1.f));
//		model = glm::translate(model, glmposition);
//		Window& window = gFakeEngine->GetGraphicManager<Window>();
//		glm::mat4 proj = glm::ortho(
//			float(-window.GetHalfWidth()),
//			float(window.GetHalfWidth()),
//			float(-window.GetHalfHeight()),
//			float(window.GetHalfHeight()));
//
//		projection = proj * model;
//	}
//	shdrpgm.SetUniform("projection", projection);
//
//
//	//glactivetexture(gl_texture0);
//	//glbindvertexarray(vao);
//	//glbindvertexarray(vao);
//	//glbindbuffer(gl_array_buffer, vbo);
//	//glbufferdata(gl_array_buffer, sizeof(glfloat) * 6 * 4 * text.size(), null, gl_dynamic_draw);
//	//glenablevertexattribarray(0);
//	//glvertexattribpointer(0, 4, gl_float, gl_false, 4 * sizeof(float), 0);
//	//glbindbuffer(gl_array_buffer, 0);
//	//glbindvertexarray(0);
//
//
//
//	/*float smallestx, smallesty, biggestx, biggesty;
//
//	for (int i = 0; i < vertices.size(); i += 4) {
//		normverts.push_back(normalisefloat(vertices[i], smallestx, biggestx));
//		normverts.push_back(normalisefloat(vertices[i + 1], smallesty, biggesty) * 2 - 1.0f);
//		normverts.push_back(vertices[i + 2]);
//		normverts.push_back(vertices[i + 3]);
//	}
//
//
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    glGenBuffers(1, &vbo);
//    glBindBuffer(gl_array_buffer, vbo);
//    glBufferData(gl_array_buffer, normverts.size() * sizeof(normverts[0]), &normverts[0], GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//*/
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    
//	// iterate through all characters
//	std::string::const_iterator c;
//    
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	for (c = text.begin(); c != text.end(); c++)
//	{
//
//		// render glyph texture over quad
//
//
//
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	}
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//float Graphics::FreeType::normaliseFloat(float num, float min, float max)
//{
//	return 2 * ((num - min) / (max - min)) - 1;
//}
//


/*****************************************************************
*\file		 FreeType.cpp
*\brief		 FreeType functions definitions.

*\author(s)   Lee Liang Ping	121 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FreeType.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
void Graphics::FreeType::Init(std::string file)
{
    FT_Library ft;
    FT_Face face;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    if (FT_New_Face(ft, file.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Graphics::FreeType::RenderText(std::string text, float x, float y, float scale, glm::vec3 color, FreeRenderType type)
{
    // activate corresponding render state	
    GLSLShader& shdrpgm = gFakeEngine->GetShader(ShaderOption::TEXT);
    shdrpgm.Use();
    shdrpgm.SetUniform("textColor", color);

    Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection;
    if (FreeRenderType::MODEL_WORLD == type)
    {
        glm::vec3 glmposition = glm::vec3(x, y, -1.f);
        model = glm::translate(model, glmposition);
        glm::mat4 view = glm::lookAt(
            glm::vec3(camera2d.pos.x, camera2d.pos.y, 0.f),
            glm::vec3(camera2d.pos.x, camera2d.pos.y, 1.0f),
            glm::vec3(0.f, 1.f, 0.f)); // up vector
        glm::mat4 proj = glm::ortho(
            float(camera2d.cam_window.x),
            float(-camera2d.cam_window.x),
            float(-camera2d.cam_window.y),
            float(camera2d.cam_window.y));
        projection = proj * view * model;

    }
    else if (FreeRenderType::SCREEN == type)
    {
        glm::vec3 glmposition = glm::normalize(glm::vec3(x, y, 1.f));
        model = glm::translate(model, glmposition);
        Window& window = gFakeEngine->GetGraphicManager<Window>();
        glm::mat4 proj = glm::ortho(
            float(-window.GetHalfWidth()),
            float(window.GetHalfWidth()),
            float(-window.GetHalfHeight()),
            float(window.GetHalfHeight()));

        projection = proj * model;
    }
    shdrpgm.SetUniform("projection", projection);


    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
