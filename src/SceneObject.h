#pragma once

#include <memory>

#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "LightSource.h"
#include "Transform.h"

class SceneObject
{
public:
	SceneObject();
	SceneObject(std::unique_ptr<Transform> transform, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material,
		std::unique_ptr<Shader> shader, std::unique_ptr<LightSource> lightSource);

	void Update();
	void Draw();

	const std::shared_ptr<Transform>& GetTransform() { return m_Transform; };
	const std::shared_ptr<Mesh>& GetMesh() { return m_Mesh; };
	const std::shared_ptr<Material>& GetMaterial() { return m_Material; };
	const std::shared_ptr<Shader>& GetShader() { return m_Shader; };
	const std::shared_ptr<LightSource>& GetLightSource() { return m_LightSource; };

	const std::shared_ptr<bool> GetActive() { return m_Active; };

	void SetTransform(std::unique_ptr<Transform>& transform) { m_Transform = std::move(transform); };
	void SetMesh(std::unique_ptr<Mesh>& mesh) { m_Mesh = std::move(mesh); };
	void SetMaterial(std::unique_ptr<Material>& material) { m_Material = std::move(material); };
	void SetShader(std::unique_ptr<Shader>& shader) { m_Shader = std::move(shader); };
	void SetLightSource(std::unique_ptr<LightSource>& lightSource) { m_LightSource = std::move(lightSource); };

private:
	std::shared_ptr<Transform> m_Transform;
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<LightSource> m_LightSource;

	std::shared_ptr<bool> m_Active;
};
