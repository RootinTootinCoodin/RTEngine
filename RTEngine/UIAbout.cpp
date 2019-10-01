#include "UIAbout.h"
#include <iostream>
#include "Globals.h"

UIAbout::UIAbout(Application * app) : UIElement(app)
{
	name = "About";
	show_window = false;

	//JSON_Value *file = json_parse_file("About.json");
	//text = json_object_get_string(json_object(file), "text");
	////json_value_free(file);
	//JSON_Object* config;
	//JSON_Value* config_value;

	//config_value = json_parse_file_with_comments("user_data.json");
	//if (config_value == NULL)
	//{
	//	LOG("Error opening json file");
	//}
	//else
	//{
	//	LOG("Sucess opening json file");
	//}
	//	

	//config = json_value_get_object(config_value);
	//text = json_object_get_string(json_object_get_object(config,"name"),"name2");
	
}

UIAbout::~UIAbout()
{
}

void UIAbout::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text("RTEngine\nNow that's what I call a rootin' tootin' engine, partner.\nBy Lorién Portella and Joel Cabaco.\n");
		ImGui::Text("3rd Party Libraries Used:");

		ImGui::Text("MIT License\n\nCopyright(c) 2019 RootinTootinCoodin\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\n\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");

		ImGui::Text(text.c_str());
	}
	ImGui::End();
}