#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "Shaders/Shader.h"
#include <QLabel.h>

MeshComponent::MeshComponent(QWidget* parent) :
	ComponentView(parent)
{ }

MeshComponent::~MeshComponent() { }

void MeshComponent::OnEntitySelected(Entity* entity){
	Mesh* mesh = entity->GetComponent<Mesh>();
	if(mesh){
		QString title = "Shader - ";
		title += mesh->GetMaterial()->GetShader()->GetFilename().c_str();
		shader_filename->setText(title);
	}else{
		hide();
	}
}

void MeshComponent::showEvent(QShowEvent *e){
	ComponentView::showEvent(e);
	shader_filename = findChild<QLabel*>();
}