#include "SceneObject.h"

SceneObject::SceneObject()
	: m_Transform(), m_Mesh(), m_Material(), m_Shader(), m_LightSource(), m_Active(true)
{

}

SceneObject::SceneObject(std::unique_ptr<Transform> transform, std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material,
	std::unique_ptr<Shader> shader, std::unique_ptr<LightSource> lightSource)
	: m_Transform(std::move(transform)), m_Mesh(std::move(mesh)), m_Material(std::move(material)), m_Shader(std::move(shader)),
	m_LightSource(std::move(lightSource)), m_Active(true)
{

}

void SceneObject::Update()
{
	if (!m_Active) return;

	if (m_Shader)
	{
		m_Shader->Use();

		if (m_Transform) m_Shader->SetUniform("model", m_Transform->GetModelMatrix());
		if (m_Material)
		{
			glm::vec4 material = m_Material->GetMaterial();
			m_Shader->SetUniform("material.specular", material[0]);
			m_Shader->SetUniform("material.diffuse", material[1]);
			m_Shader->SetUniform("material.ambient", material[2]);
			m_Shader->SetUniform("material.shininess", material[3]);
		}
	}
}

void SceneObject::Draw()
{
	if (!m_Active) return;

	if (m_Shader) m_Shader->Use();
	if (m_Mesh) m_Mesh->Draw();
}