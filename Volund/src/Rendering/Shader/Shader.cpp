#include "PCH/PCH.h"

#include "Shader.h"

#include "OpenGLShader.h"

#include "Rendering/RenderingAPI/RenderingAPI.h"

namespace Volund
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
        std::stringstream stringStream;
        if (ResourceLibrary::IsResource(filepath))
        {
            stringStream << ResourceLibrary::Fetch(filepath);
        }
        else
        {
            std::ifstream file(filepath);

            if (!file)
            {
                VOLUND_ERROR("Failed to load shader %s!", filepath.c_str());
                return nullptr;
            }

            stringStream << file.rdbuf();
        }

        enum class ShaderType
        {
            None = -1,
            Vertex = 0,
            Fragment = 1,
            Geometry = 2
        };

        std::string line;
        std::stringstream sourceStrings[3];
        ShaderType type = ShaderType::None;

        while (std::getline(stringStream, line))
        {
            //Split into words
            std::vector<std::string> words;
            std::istringstream iss(line);
            std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                std::back_inserter(words));

            if (words.size() <= 0)
            {
                continue;
            }

            if (words.size() >= 2 && words[0] == "#VOLUND_SHADER_TYPE")
            {
                if (words[1] == "VERTEX")
                {
                    type = ShaderType::Vertex;
                }
                else if (words[1] == "FRAGMENT")
                {
                    type = ShaderType::Fragment;
                }
                else if (words[1] == "GEOMETRY")
                {
                    type = ShaderType::Geometry;
                }
            }
            else if ((int32_t)type != -1)
            {
                sourceStrings[(int32_t)type] << line << '\n';
            }
        }

        return Shader::Create(
            sourceStrings[(uint32_t)ShaderType::Vertex].str(),
            sourceStrings[(uint32_t)ShaderType::Fragment].str(),
            sourceStrings[(uint32_t)ShaderType::Geometry].str());
	}

	Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		switch (RenderingAPI::GetSelectedAPI())
		{
		case GraphicsAPI::OpenGL:
		{
			return std::make_shared<OpenGLShader>(vertexSource, fragmentSource, geometrySource);
		}
		break;
		default:
		{
			VOLUND_ERROR("Creating a Shader without a specified GraphicsAPI!");
			return nullptr;
		}
		break;
		}
	}

} //namespace Volund
