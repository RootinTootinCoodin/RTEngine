#include "UITests.h"

#include <random>



#include "par_shapes/par_shapes.h"




UITests::UITests(Application* App) : UIElement(App)
{
	name = "Tests";
}


UITests::~UITests()
{
}

void UITests::Draw()
{
	pcg32 seed; 
	seed.seed(pcg_extras::seed_seq_from<std::random_device>()); // if i put this values in the header, i get weir compiler errors
	if (ImGui::Begin(name.c_str(),&show_window))
	{
		if (ImGui::CollapsingHeader("Random Number Generator"))
		{
			ImGui::Text("Generate a float between 0.0 and 1.0");
			static float result = 0.0f;
			if (ImGui::Button("Generate Float"))
			{
				std::uniform_real_distribution<float> uniform_dist(0.0, 1.0);
				result = uniform_dist(seed);
			}
			ImGui::Text("Float generated: %f", result);

			ImGui::Separator();

			ImGui::Text("Integer between two values");
			static int value1 = 0;
			static int value2 = 0;
			static int result2 = 0.0f;
			ImGui::DragInt("Value 1", &value1);
			ImGui::DragInt("Value 2", &value2);
			if (ImGui::Button("Generate Integer"))
			{
				

				if (value1 <= value2)
				{
					std::uniform_int_distribution<int> uniform_dist(value1, value2);
					result2 = uniform_dist(seed);
				}
				else
				{
					std::uniform_int_distribution<int> uniform_dist(value2, value1);
					result2 = uniform_dist(seed);
				}

			}

			ImGui::Text("Integer generated: %i", result2);
		}

	}
	ImGui::End();
}
