#ifndef __UI_TEXTEDITOR_H__
#define __UI_TEXTEDITOR_H__

#include "UIElement.h"
#include "TextEditor.h"

class UITextEditor : public UIElement
{
public:
	UITextEditor(Application* app);
	~UITextEditor();

	void Draw();
	void InitializeScriptEditor();

	std::string text;

	TextEditor scriptEditor;
	std::string scriptPath;
};

#endif // !__UI_TEXTEDITOR_H__