#pragma once

#include "Filesystem/Filesystem.h"

#include "AABB/AABB.h"

namespace Volund
{
	template <typename V, typename I>
	class ModelLoader
	{
	public:
		
		AABB aabb;

		std::vector<V> Vertices;
		std::vector<I> Indices;

		ModelLoader(const std::string& Filepath);

	private:
		
		void LoadOBJ(const std::string& Filepath);

		struct ArrayHasher
		{
			std::uint64_t operator()(const std::array<V, 8>& Array) const
			{
				std::uint64_t H = 0;

				for (auto E : Array)
				{
					H ^= std::hash<uint64_t>{}((uint64_t)(E)) + 0x9e3779b9 + (H << 6) + (H >> 2);
				}

				return H;
			}
		};
	};

	template <typename V, typename I>
	ModelLoader<V, I>::ModelLoader(const std::string& Filepath)
	{
		VOLUND_INFO("Loading OBJ file (%s)...", Filepath.data());

		if (Filepath.ends_with(".obj") || Filepath.ends_with(".vobj"))
		{
			this->LoadOBJ(Filepath);
		}
		else
		{
			VOLUND_WARNING("Unable to read unknown model file type (%s)!", Filepath.data());
		}

		this->aabb = AABB(this->Vertices);
	}

	template <typename V, typename I>
	void ModelLoader<V, I>::LoadOBJ(const std::string& Filepath)
	{
		std::vector<V> Geometry;
		std::vector<V> TextureCoords;
		std::vector<V> Normals;

		std::unordered_map<std::array<V, 8>, I, ArrayHasher> VertexToIndexMap;

		std::stringstream Buffer = std::stringstream(VL::Filesystem::LoadFile(Filepath));

		std::string Line;
		while (std::getline(Buffer, Line))
		{
			char LineHeader[16] = {};
			LineHeader[15] = 0;

			sscanf(Line.c_str(), "%15s", LineHeader);

			if (strcmp(LineHeader, "v") == 0)
			{
				float X, Y, Z = 0.0f;
				VOLUND_ASSERT(sscanf(Line.c_str(), "v %f %f %f", &X, &Y, &Z) == 3, "Unable to parse OBJ file (%s)!", Filepath.data());

				Geometry.push_back(X);
				Geometry.push_back(Y);
				Geometry.push_back(Z);
			}
			else if (strcmp(LineHeader, "vt") == 0)
			{
				float X, Y = 0.0f;
				VOLUND_ASSERT(sscanf(Line.c_str(), "vt %f %f", &X, &Y) == 2, "Unable to parse OBJ file (%s)!", Filepath.data());

				TextureCoords.push_back(X);
				TextureCoords.push_back(Y);
			}
			else if (strcmp(LineHeader, "vn") == 0)
			{
				float X, Y, Z = 0.0f;
				VOLUND_ASSERT(sscanf(Line.c_str(), "vn %f %f %f", &X, &Y, &Z) == 3, "Unable to parse OBJ file (%s)!", Filepath.data());

				Normals.push_back(X);
				Normals.push_back(Y);
				Normals.push_back(Z);
			}
			else if (strcmp(LineHeader, "f") == 0)
			{
				uint32_t GeometryIndices[3];
				uint32_t TextureCoordsIndices[3];
				uint32_t NormalsIndices[3];

				VOLUND_ASSERT(sscanf(Line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
					&GeometryIndices[0], &TextureCoordsIndices[0], &NormalsIndices[0],
					&GeometryIndices[1], &TextureCoordsIndices[1], &NormalsIndices[1],
					&GeometryIndices[2], &TextureCoordsIndices[2], &NormalsIndices[2]) == 9, "Unable to parse OBJ file (%s)!", Filepath.data());

				for (uint64_t i = 0; i < 3; i++)
				{
					std::array<V, 8> Vertex;

					if (!Geometry.empty() && !TextureCoords.empty() && !Normals.empty())
					{
						uint32_t GeometryIndex = (GeometryIndices[i] - 1) * 3;
						uint32_t TextureCoordsIndex = (TextureCoordsIndices[i] - 1) * 2;
						uint32_t NormalsIndex = (NormalsIndices[i] - 1) * 3;

						Vertex =
						{
							Geometry[GeometryIndex + 0],
							Geometry[GeometryIndex + 1],
							Geometry[GeometryIndex + 2],
							TextureCoords[TextureCoordsIndex + 0],
							TextureCoords[TextureCoordsIndex + 1],
							Normals[NormalsIndex + 0],
							Normals[NormalsIndex + 1],
							Normals[NormalsIndex + 2]
						};
					}
					/*else if (!Geometry.empty() && TextureCoords.empty() && Normals.empty())
					{
						uint32_t GeometryIndex;
						VOLUND_ASSERT(sscanf(Line.c_str(), "f %d", &GeometryIndex) == 1, "Unable to parse OBJ file (%s)!", Filepath.data());
						GeometryIndex = (GeometryIndex - 1) * 3;

						Vertex =
						{
							Geometry[GeometryIndex + 0],
							Geometry[GeometryIndex + 1],
							Geometry[GeometryIndex + 2],
							0,
							0,
							0,
							0,
							0
						};
					}*/
					else
					{
						VOLUND_ERROR("Unable to parse OBJ file (%s)!", Filepath.data());
					}

					if (VertexToIndexMap.contains(Vertex))
					{
						this->Indices.push_back(VertexToIndexMap[Vertex]);
					}
					else
					{
						this->Indices.push_back((uint32_t)(this->Vertices.size() / 8));

						for (uint64_t j = 0; j < 8; j++)
						{
							this->Vertices.push_back(Vertex[j]);
						}
					}
				}
			}
			else
			{
				//Not implemented
			}
		}
	}
}
