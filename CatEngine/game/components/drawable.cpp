#include "game/components/drawable.h"
#include "graphics/renderer.h"
#include "game/game_object.h"

namespace cat::game::components
{
	drawable::drawable() : 
		m_color(glm::vec4(1,1,1,1))
	{		

	}

	drawable::~drawable()
	{
		core::utils::safeDelete(m_piece);
	}

	void drawable::onRender(graphics::renderer* render) 
	{				
		if (m_piece == nullptr)
		{
			return;
		}

		const auto transform = getOwner()->getTransform();
		m_piece->begin();

		const auto shader = m_piece->getShader();
		auto& world_mat = transform->getWorldMatrix();
		world_mat = transform->getTransformation();
		shader->setMat4("transform.world", world_mat);
		shader->setVec4("drawable.color", m_color);
		
		m_piece->end(render);
	}

	void drawable::setColor(glm::vec4 color)
	{
		m_color = color;
	}

	glm::vec4 drawable::getColor() const
	{
		return m_color;
	}
}