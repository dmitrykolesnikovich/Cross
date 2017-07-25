#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"
#include "ui_CrossEditor.h"

#include <QMainWindow>

class SceneView;

extern CrossEditor* editor;

class CrossEditor : public QMainWindow, 
					public Game
{
	Q_OBJECT
public:
	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

	Screen* GetStartScreen() override;
	void Update(float sec) override;

	void LoadScene(QString& fileInfo);
	void RestoreSettings();
	void ExceptionMsgBox(const char* msg);

	void OnScreenChanged(Screen* screen);

	void OnNewSceneClick();
	void OnSaveAsClick();
	void OnSetupProjectDirectoryClick();

	void OnFileExplorerClick();
	void OnSceneExplorerClick();

protected:
	void closeEvent(QCloseEvent* eve) override;
	void keyPressEvent(QKeyEvent* key) override;
	void keyReleaseEvent(QKeyEvent* key) override;

private:
	Ui::CrossEditorClass ui;
	QString scene_file;

};

#endif // !CROSS_EDITOR