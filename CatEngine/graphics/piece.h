#pragma once
#include "core/common.h"

#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer.h"
#include "graphics/vertex.h"
#include "graphics/shader.h"
#include "graphics/texture.h"


namespace cat::graphics
{
	class renderer;

	class CATENGINE_API piece
	{
	public:
		// Make a sprite
		piece();

		// Custom form
		explicit piece(
			const std::vector<graphics::vertex>& vertices,
			const std::vector<std::uint32_t>& indices,
			const std::vector<const char*>& texture_names,
			const char* shader_name
			);

		~piece();

		virtual void begin();
		virtual void end(graphics::renderer* render);
		virtual void setTexture(std::uint32_t index, std::shared_ptr<graphics::texture> texture);
		virtual void setTexture(std::uint32_t index, graphics::texture* texture);
		virtual void setTexture(std::uint32_t index, const char* texture_name);
		virtual void loadShader(const char* name);
		virtual void reloadTextures();

		[[nodiscard]] inline graphics::shader* getShader()const;
		[[nodiscard]] inline graphics::vertex_buffer* getVertexBuffer() const;
		[[nodiscard]] inline graphics::index_buffer* getIndexBuffer() const;
		[[nodiscard]] inline graphics::texture* getTexture(std::uint32_t index) const;
		
		[[nodiscard]] inline std::vector<std::shared_ptr<graphics::texture>> 
			getTextures() const;

	private:
		void createPiece(const std::vector<graphics::vertex>& vertices,
			const std::vector<std::uint32_t>& indices,
			const std::vector<const char*>& texture_names,
			const char* shader_name);

		std::shared_ptr<graphics::shader> m_shader;
		std::shared_ptr<graphics::vertex_buffer> m_vertex_buffer;
		std::shared_ptr<graphics::index_buffer>  m_index_buffer;
		std::vector<std::shared_ptr<graphics::texture>> m_textures;
	};
}