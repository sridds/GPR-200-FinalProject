#pragma once
#include <string>
#include <glm/glm.hpp>

namespace ew {
	std::string loadShaderSourceFromFile(const char* filePath);
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	class Shader {
	public:
		Shader(const char* vertexShader, const char* fragmentShader);
		void use()const;
		void setInt(const char* name, int v) const;
		void setFloat(const char* name, float v) const;
		void setVec2(const char* name, float x, float y) const;
		void setVec2(const char* name, const glm::vec2& v) const;
		void setVec3(const char* name, float x, float y, float z) const;
		void setVec3(const char* name, const glm::vec3& v) const;
		void setVec4(const char* name, float x, float y, float z, float w) const;
		void setVec4(const char* name, const glm::vec4& v) const;
		void setMat4(const char* name, const glm::mat4& m) const;
		int getShaderID();
	private:
		unsigned int m_id; //Shader program handle
	};
}