#include "joltDebugRenderer.h"

namespace cat::physics
{
	joltDebugRenderer::joltDebugRenderer(graphics::renderer* renderer) : 
		m_renderer(renderer)
	{
		JPH::DebugRenderer::Initialize();
	}

	void joltDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
	{
		m_renderer->debugDrawLine({ inFrom.GetX(), inFrom.GetY(), inFrom.GetZ() }, { inTo.GetX(), inTo.GetY(), inTo.GetZ() }, { inColor.r,inColor.g,inColor.b,inColor.a });
	}

	void joltDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, JPH::DebugRenderer::ECastShadow inCastShadow)
	{

	}

	JPH::DebugRenderer::Batch joltDebugRenderer::CreateTriangleBatch(const JPH::DebugRenderer::Triangle* inTriangles, int inTriangleCount)
	{
		return nullptr;
	}

	JPH::DebugRenderer::Batch joltDebugRenderer::CreateTriangleBatch(const JPH::DebugRenderer::Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount)
	{
		return nullptr;
	}

	void joltDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor,
		const JPH::DebugRenderer::GeometryRef& inGeometry, JPH::DebugRenderer::ECullMode inCullMode, JPH::DebugRenderer::ECastShadow inCastShadow, JPH::DebugRenderer::EDrawMode inDrawMode)
	{

	}

	void joltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view& inString, JPH::ColorArg inColor, float inHeight)
	{

	}
}