#pragma once
#include "graphics/renderer.h"
#include "physics/physics_core.h"

#ifdef JPH_DEBUG_RENDERER
#include <Jolt/Renderer/DebugRenderer.h>
#else
// Hack to still compile DebugRenderer inside the test framework when Jolt is compiled without
#define JPH_DEBUG_RENDERER
// Make sure the debug renderer symbols don't get imported or exported
#define JPH_DEBUG_RENDERER_EXPORT
#include <Jolt/Renderer/DebugRenderer.h>
#undef JPH_DEBUG_RENDERER
#undef JPH_DEBUG_RENDERER_EXPORT
#endif

namespace cat::physics
{
	class joltDebugRenderer final : public JPH::DebugRenderer
	{
	public:
		JPH_OVERRIDE_NEW_DELETE

		joltDebugRenderer(graphics::renderer* inRenderer);

		virtual void						DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
		virtual void						DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, 
			JPH::DebugRenderer::ECastShadow inCastShadow = JPH::DebugRenderer::ECastShadow::Off) override;

		virtual JPH::DebugRenderer::Batch	CreateTriangleBatch(const JPH::DebugRenderer::Triangle* inTriangles, int inTriangleCount) override;
		virtual JPH::DebugRenderer::Batch	CreateTriangleBatch(const JPH::DebugRenderer::Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) override;

		virtual void						DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor,
			const JPH::DebugRenderer::GeometryRef& inGeometry, JPH::DebugRenderer::ECullMode inCullMode, JPH::DebugRenderer::ECastShadow inCastShadow, JPH::DebugRenderer::EDrawMode inDrawMode) override;

		virtual void						DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view& inString, JPH::ColorArg inColor, float inHeight) override;
	
	private:
		graphics::renderer* m_renderer;
	};
}