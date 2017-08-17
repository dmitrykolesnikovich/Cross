#include "PropertiesView.h"
#include "CrossEditor.h"
#include "Entity.h"
#include "File.h"

#include "ui_EntityComponent.h"
#include "ui_ShaderView.h"
#include "ui_MaterialView.h"
#include "ui_MeshComponent.h"
#include "ui_LightComponent.h"

#include <QTreeView>
#include <QContextMenuEvent>

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ 
	editor->UIInitialized.Connect(this, &PropertiesView::OnUIInitialized);
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);

	context_menu = new QMenu(this);
	QMenu* addComponent = new QMenu("Add Component", context_menu);
	context_menu->addMenu(addComponent);

	QAction* addLight = new QAction(addComponent);
	addLight->setText("Light");
	connect(addLight, &QAction::triggered, this, &PropertiesView::OnAddComponent<Light>);
	addComponent->addAction(addLight);
}

PropertiesView::~PropertiesView() { 
}

void PropertiesView::OnUIInitialized(){
	QWidget* layoutWidget = findChild<QWidget*>("layout");
	layout = dynamic_cast<QVBoxLayout*>(layoutWidget->layout());
	CreateView<EntityComponent, Ui::EntityComponentClass>("entityComponent");
	CreateView<MeshComponent, Ui::MeshComponentClass>("meshComponent");
	CreateView<LightComponent, Ui::LightComponentClass>("lightComponent");
	CreateView<ShaderView, Ui::ShaderViewClass>("shaderView");
	CreateView<MaterialView, Ui::MaterialViewClass>("materialView");

	for(PropertyView* v : views) {
		v->Initialize();
		v->hide();
	}
}

void PropertiesView::Update(float sec){
	if(selected_entity){
		for(PropertyView* v : views){
			v->Update(sec);
		}
	}
}

void PropertiesView::OnScreenChanged(Screen*){
	selected_entity = NULL;
	for(PropertyView* v : views) {
		v->hide();
	}
}

void PropertiesView::OnEntitySelected(Entity* entity) {
	show();
	for(PropertyView* v : views){
		v->OnEntitySelected(entity);
	}
	selected_entity = entity;
}

void PropertiesView::OnEntityChanged(Entity* entity){
	if(entity == selected_entity){
		for(PropertyView* v : views) {
			v->OnEntitySelected(entity);
		}
	}
}

void PropertiesView::OnFileSelected(string filename){
	for(PropertyView* view : views){
		view->OnFileSelected(filename);
	}
}

void PropertiesView::contextMenuEvent(QContextMenuEvent *event) {
	if(selected_entity){
		context_menu->exec(event->globalPos());
	}
}