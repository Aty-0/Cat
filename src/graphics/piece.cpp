#include "graphics/piece.h"
#include "graphics/renderer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	// By default we are making a sprite
	piece::piece(bool makeSquare) : m_polyMode(GL_FILL), m_cullFace(GL_CW), m_cullMode(GL_BACK)
	{
		if (makeSquare)
		{
			const std::vector<graphics::vertex> vertices = { { glm::vec3(0.5f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, -1.0f) },      // top right
											{ glm::vec3(0.5f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },      // bottom right
											{ glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) },      // bottom left
											{ glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, -1.0f) }       // top left 
			};

			const std::vector<std::uint32_t> indices =
			{
				0, 1, 3,  // first triangle
				1, 2, 3   // second triangle
			};

			createPiece(vertices, indices, { "default_texture" }, "piece");

		}
		else
		{
			m_vertex_buffer = std::make_shared<graphics::vertex_buffer>();
			m_shader = std::make_shared<graphics::shader>();
		}
	}

	piece::piece(
		const std::vector<graphics::vertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<const char*>& texture_names,
		const char* shader_name
	) : m_polyMode(GL_FILL), m_cullFace(GL_CW), m_cullMode(GL_BACK)
	{
		createPiece(vertices, indices, texture_names, shader_name);
	}

	void piece::createPiece(const std::vector<graphics::vertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<const char*>& texture_names,
		const char* shader_name)
	{
		CAT_ASSERT(!vertices.empty());

		m_vertex_buffer = std::make_shared<graphics::vertex_buffer>();

		m_vertex_buffer->gen();
		m_vertex_buffer->setBufferData(vertices, GL_STATIC_DRAW);

		if (!indices.empty())
		{
			m_index_buffer = std::make_shared<graphics::index_buffer>();
			m_index_buffer->gen();
			m_index_buffer->setBufferData(indices, GL_STATIC_DRAW);
		}

		m_vertex_buffer->setAttrib(0, 3, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, pos)));
		m_vertex_buffer->setAttrib(1, 4, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, color)));
		m_vertex_buffer->setAttrib(2, 2, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, uv)));

		m_vertex_buffer->unbindBuffer();

		m_shader = std::make_shared<graphics::shader>();
		loadShader(shader_name);


		for (auto name : texture_names)
		{
			const auto texture = std::make_shared<graphics::texture>();
			CAT_ASSERT(texture->load(name));
			m_textures.push_back(texture);
		}
	}

	piece::~piece()
	{
		m_vertex_buffer.reset();
		m_index_buffer.reset();

		m_shader.reset();

		m_textures.clear();
		m_textures.shrink_to_fit();
	}

	// TODO: Dynamic gl buffer update 
	void piece::addNewVertices(const std::vector<vertex>& vertices)
	{
		m_vertex_buffer->unbindBuffer();

		auto concatenated_vertices = m_vertex_buffer->getVector();
		concatenated_vertices.reserve(concatenated_vertices.size() + vertices.size());
		concatenated_vertices.insert(concatenated_vertices.end(), vertices.begin(), vertices.end());

		m_vertex_buffer->clear();

		m_vertex_buffer->gen();

		m_vertex_buffer->setBufferData(concatenated_vertices, GL_STATIC_DRAW);

		m_vertex_buffer->setAttrib(0, 3, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, pos)));
		m_vertex_buffer->setAttrib(1, 4, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, color)));
		m_vertex_buffer->setAttrib(2, 2, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, uv)));

		m_vertex_buffer->unbindBuffer();
	}
	
	void piece::setCullMode(std::int32_t cullmode, std::int32_t frontface)
	{
		m_cullFace = frontface;
		m_cullMode = cullmode;
	}
	
	void piece::reloadTextures()
	{
		// TODO: 
	}

	void piece::begin()
	{
		m_shader->bind();
	}

	void piece::setTexture(std::uint32_t index, std::shared_ptr<graphics::texture> texture)
	{		
		CAT_ASSERT(texture != nullptr);
	
		for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
		{
			if (index == std::distance(m_textures.begin(), it))
			{
				it->reset();
				it->swap(texture);
				return;
			}
		}
		
		// If index not found but we are want to set it
		// We are just push that texture 
		m_textures.push_back(texture);
	}

	void piece::setTexture(std::uint32_t index, graphics::texture* texture)
	{		
		CAT_ASSERT(texture != nullptr);
		auto shared_tex = std::make_shared<graphics::texture>(*texture);
		setTexture(index, shared_tex);
	}

	void piece::setTexture(std::uint32_t index, const char* texture_name)
	{
		auto texture = m_textures[index];
		if (texture == nullptr)
		{
			texture = std::make_shared<graphics::texture>();
		}

		CAT_ASSERT(texture->load(texture_name));
		setTexture(index, texture);
	}

	void piece::loadShader(const char* name)
	{
		CAT_ASSERT(m_shader != nullptr);
		CAT_ASSERT(m_shader->load(name));
	}

	void piece::setPolyMode(std::int32_t mode)
	{
		m_polyMode = mode;
	}

	void piece::end(graphics::renderer* render)
	{
		std::uint32_t index_bind = 0;
		for (auto texture : m_textures)
		{
			if (texture == nullptr)
				return;

			texture->bind(GL_TEXTURE0 + index_bind);
			index_bind++;
		}

		m_vertex_buffer->bind();
		
		render->cull(m_cullMode, m_cullFace);
		if (m_index_buffer != nullptr)
		{
			render->setPolygonMode(m_polyMode);
			const auto size = m_index_buffer->size();
			render->drawElements(size, GL_TRIANGLES);
			render->setPolygonMode(GL_FILL); // by default
		}
		else
		{
			const auto size = m_vertex_buffer->size();
			render->draw(size, m_polyMode == GL_FILL ? GL_TRIANGLES : GL_LINES);
		}
		render->disableCull();

		m_vertex_buffer->unbindBufferArray();
		m_shader->unbind();

		std::uint32_t index_unbind = 0;
		for (auto texture : m_textures)
		{
			if (texture == nullptr)
				return;

			texture->unbind(GL_TEXTURE0 + index_unbind);
			index_unbind++;
		}
	}
	graphics::shader* piece::getShader() const
	{
		return m_shader.get();
	}
	
	graphics::vertex_buffer* piece::getVertexBuffer()  const
	{
		return m_vertex_buffer.get();
	}
	
	graphics::index_buffer* piece::getIndexBuffer()  const
	{
		return m_index_buffer.get();
	}

	graphics::texture* piece::getTexture(std::uint32_t index) const
	{
		for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
		{
			if (index == std::distance(m_textures.begin(), it))
			{				
				return it->get();
			}
		}

		return nullptr;
	}
	
	std::vector<std::shared_ptr<graphics::texture>>
		piece::getTextures() const
	{
		return m_textures;
	}
}