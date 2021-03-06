#include "FileExplorer.h"
#include "System.h"
#include "CrossEditor.h"
#include "Scene.h"
#include "Material.h"
#include "File.h"
#include "Entity.h"

#include <QHeaderView>
#include <QFileSystemModel>
#include <QContextMenuEvent>
#include <QDesktopServices>

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	file_system = new QFileSystemModel(this);
	file_system->setReadOnly(false);
	setModel(file_system);

	SetupProjectDirectory(QDir::currentPath() + "/" + QString(system->AssetsPath().c_str()));

	hideColumn(1);
	hideColumn(2);
	hideColumn(3);
	
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

	connect(this, &QTreeView::clicked, this, &FileExplorer::OnItemSelected);
	connect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);

	//context menu creating
	context_menu = new QMenu(this);

	QAction* newFolder = new QAction(context_menu);
	newFolder->setText("New Folder");
	connect(newFolder, &QAction::triggered, this, &FileExplorer::OnNewFolderClick);
	context_menu->addAction(newFolder);
	QAction* newShader = new QAction(context_menu);
	newShader->setText("New Shader");
	connect(newShader, &QAction::triggered, this, &FileExplorer::OnNewShaderClick);
	context_menu->addAction(newShader);
	QAction* newMaterial = new QAction(context_menu);
	newMaterial->setText("New Material");
	connect(newMaterial, &QAction::triggered, this, &FileExplorer::OnNewMaterialClick);
	context_menu->addAction(newMaterial);
	QAction* copyAction = new QAction(context_menu);
	copyAction->setText("Copy");
	copyAction->setShortcut(QKeySequence::Copy);
	connect(copyAction, &QAction::triggered, this, &FileExplorer::OnCopyClick);
	context_menu->addAction(copyAction);
	addAction(copyAction);
	QAction* pasteAction = new QAction(context_menu);
	pasteAction->setText("Paste");
	pasteAction->setShortcut(QKeySequence::Paste);
	connect(pasteAction, &QAction::triggered, this, &FileExplorer::OnPasteClick);
	context_menu->addAction(pasteAction);
	addAction(pasteAction);
	QAction* deleteAction = new QAction(context_menu);
	deleteAction->setText("Delete");
	deleteAction->setShortcut(QKeySequence::Delete);
	connect(deleteAction, &QAction::triggered, this, &FileExplorer::OnDeleteClick);
	context_menu->addAction(deleteAction);
	addAction(deleteAction);
}

FileExplorer::~FileExplorer(){
	delete file_system;
	disconnect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);
}

void FileExplorer::SetupProjectDirectory(QString dir){
	file_system->setRootPath(dir);
	setRootIndex(file_system->index(dir));
}

void FileExplorer::contextMenuEvent(QContextMenuEvent *eve) {
	context_menu->exec(eve->globalPos());
}

void FileExplorer::OnItemSelected(QModelIndex index){
	QFileInfo fileInfo = file_system->fileInfo(index);
	QDir root = file_system->rootDirectory();
	QString filepath = root.relativeFilePath(fileInfo.absoluteFilePath());
	FileSelected(filepath.toStdString());
}

void FileExplorer::OnItemDoubleClick(QModelIndex index){
	QFileInfo fileInfo = file_system->fileInfo(index);
	QDir root = file_system->rootDirectory();
	QString filepath = root.relativeFilePath(fileInfo.absoluteFilePath());
	if(fileInfo.suffix() == "scn") {
		editor->LoadScene(filepath);
	} else if(fileInfo.suffix() == "obj" || fileInfo.suffix() == "fbx"){
		Model* model = game->GetCurrentScene()->GetModel(filepath.toStdString());
		Entity* modelHierarchy = model->GetHierarchy();
		//gfx3D->AdjustMaterial(modelHierarchy, gfx3D->GetDefaultMaterial()->Clone());
		CROSS_ASSERT(false, "Adjust Material function requared");
		game->GetCurrentScene()->AddEntity(modelHierarchy);
	} else if (fileInfo.suffix() == "mat" || fileInfo.suffix() == "sha"){
	} else {
		QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
	}
}

void FileExplorer::OnNewFolderClick(){
	QString selectedDir = GetSelectedDirectory();
	
	QString baseName = "/New Folder";
	QDir dir = selectedDir + baseName;
	int nameNumber = 0;
	while(dir.exists()){
		nameNumber++;
		dir = selectedDir + baseName + " " + QString::number(nameNumber);
	}
	dir.mkdir(dir.absolutePath());
	QModelIndex index = file_system->index(dir.absolutePath());
	edit(index);
}

void FileExplorer::OnNewShaderClick(){
	QString selectedDir = GetSelectedDirectory();
	QString filename = GetAllowedName(selectedDir, "New Shader", ".sha");

	Shader* newShader = new Shader();
	newShader->Save(filename.toStdString());
	delete newShader;
	QModelIndex index = file_system->index(filename);
	edit(index);
}

void FileExplorer::OnNewMaterialClick() {
	QString selectedDir = GetSelectedDirectory();
	QString filename = GetAllowedName(selectedDir, "New Material", ".mat");

	Material* newMaterial = new Material();
	game->GetCurrentScene()->SaveMaterialToXML(newMaterial, filename.toStdString());
	delete newMaterial;
	QModelIndex index = file_system->index(filename);
	edit(index);
}

void FileExplorer::OnCopyClick() {
	clipboard = selectedIndexes();
}

void FileExplorer::OnPasteClick() {
	QString selectedDir = GetSelectedDirectory();
	for(QModelIndex index : clipboard) {
		QFileInfo fileInfo = file_system->fileInfo(index);
		QString path = fileInfo.absoluteFilePath();
		QString originName = fileInfo.baseName();
		QString originExt = "." + fileInfo.suffix();
		QString filename = GetAllowedName(selectedDir, originName, originExt);
		bool result = QFile::copy(path, filename);
		CROSS_ASSERT(result, "Can not copy file");
	}
}

void FileExplorer::OnDeleteClick() {
	QModelIndexList indexes = selectedIndexes();
	for(QModelIndex index : indexes) {
		QFileInfo fileInfo = file_system->fileInfo(index);
		QString path = fileInfo.absoluteFilePath();
		QDir dir(path);
		if(fileInfo.isFile()) {
			dir.remove(path);
		} else {
			dir.removeRecursively();
		}
	}
}

QString FileExplorer::GetSelectedDirectory(){
	QString root = file_system->rootDirectory().absolutePath();
	QModelIndexList indexes = selectedIndexes();
	if(indexes.size() > 0) {
		QFileInfo fileInfo = file_system->fileInfo(indexes.first());
		if(fileInfo.isFile()){
			root = fileInfo.absolutePath();
		}else{
			root = fileInfo.absoluteFilePath();
		}
	}
	return root;
}

QString FileExplorer::GetAllowedName(const QString& dir, const QString& baseName, const QString& extension){
	QString filename = baseName + extension;
	QFile file = dir + "/" + filename;
	int nameNumber = 0;
	while(file.exists()) {
		nameNumber++;
		filename = baseName + " " + QString::number(nameNumber) + extension;
		file.setFileName(dir + "/" + filename);
	}
	return file.fileName();
}