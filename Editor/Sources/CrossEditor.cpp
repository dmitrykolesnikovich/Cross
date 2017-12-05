#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Platform/Windows/WINSystem.h"

#include <QSettings.h>
#include <QFileDialog.h>
#include <QMessageBox>
#include <QKeyEvent>

CrossEditor* editor = NULL;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	editor = this;
	game = editor;
	ui.setupUi(this);
	UIInitialized();//trigger
	connect(ui.actionNewScene, &QAction::triggered, this, &CrossEditor::OnNewSceneClick);
	connect(ui.actionSaveSceneAs, &QAction::triggered, this, &CrossEditor::OnSaveAsClick);
	connect(ui.actionSetupProjectDirectory, &QAction::triggered, this, &CrossEditor::OnSetupProjectDirectoryClick);
	
	ui.sceneExplorerTree->EntitySelected.Connect(ui.propertiesView, &PropertiesView::OnEntitySelected);
	ui.sceneExplorerTree->EntityChanged.Connect(ui.propertiesView, &PropertiesView::OnEntityChanged);
	ui.fileExplorerTree->FileSelected.Connect(ui.propertiesView, &PropertiesView::OnFileSelected);

	setWindowTitle(QString("Cross Editor - Untitled*"));
	SomethingChanged.Connect(this, &CrossEditor::OnSomethingChanged);
	ScreenChanged.Connect(this, &CrossEditor::OnScreenChanged);
}

void CrossEditor::Update(float sec){
	ui.propertiesView->Update(sec);
}

SceneExplorer* CrossEditor::GetSceneExplorer() {
	return ui.sceneExplorerTree;
}

PropertiesView* CrossEditor::GetPropertyView() {
	return ui.propertiesView;
}

FileExplorer* CrossEditor::GetFileExplorer() {
	return ui.fileExplorerTree;
}

void CrossEditor::closeEvent(QCloseEvent* eve) {
	ui.glHandler->ShutDown();
	ui.sceneExplorerTree->reset();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& scenePath){
	scene_file = scenePath;
	ui.sceneExplorerTree->clearSelection();
	SetScreen(new SceneView(scenePath.toStdString()));
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	if(settings.contains("geometry")){
		setGeometry(settings.value("geometry").value<QRect>());
	}
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::ExceptionMsgBox(const char* msg) {
	QMessageBox msgBox;
	msgBox.setText("Unhandled Exception");
	msgBox.setInformativeText(msg);
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

void CrossEditor::OnNewSceneClick(){
	ui.sceneExplorerTree->reset();
	scene_file = "Untitled";
	setWindowTitle(QString("Cross Editor - Untitled*"));
	SetScreen(new SceneView());
}

void CrossEditor::OnSaveAsClick(){
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().c_str()) + scene_file;
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	if(filePath != ""){
		scene_file = filePath;
		QFileInfo file(filePath);
		setWindowTitle(QString("Cross Editor - ") + QString(file.baseName()));
		game->GetCurrentScene()->SetName(file.baseName().toStdString().c_str());
		game->GetCurrentScene()->Save(filePath.toStdString());
	}
}

void CrossEditor::OnSetupProjectDirectoryClick(){
	QString path = QFileDialog::getExistingDirectory(this, "Setup Project Directory", QDir::currentPath());
	if(path != ""){
		path += "/";
		WINSystem* winSys = dynamic_cast<WINSystem*>(system);
		CROSS_FAIL(winSys, "You are not under Windows opertating system");
		winSys->SetAssetPath(path.toStdString());
		ui.fileExplorerTree->SetupProjectDirectory(path);
	}
}

void CrossEditor::OnSomethingChanged(Action* action){
	setWindowTitle(QString("Cross Editor - ") + GetCurrentScene()->GetName().c_str() + QString("*"));
}

void CrossEditor::OnScreenChanged(Screen* screen){
	setWindowTitle(QString("Cross Editor - ") + screen->GetName().c_str());
}

void CrossEditor::keyPressEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyPressed(k);
	}
}

void CrossEditor::keyReleaseEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyReleased(k);
	}
}