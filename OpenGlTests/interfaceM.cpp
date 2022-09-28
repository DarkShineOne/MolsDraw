#include "interfaceM.h"

extern short int index;
ifstream opened_file;
vector <atom> bonds;
extern bool fileloaded;
bool the_bool = false;
float Azimut = 0.;
float Polar = 0.;
float Radius = 0.;

interfaceM::interfaceM()
{
}

void interfaceM::gui(std::string atom1)
{
	static char result0[64] = "";
	static char result1[64] = "";
	static char result2[64] = "";
	static char result3[64] = "";
	static char buffer[128] = "";
	static char buffer1[64] = "";
	static char buffer2[64] = "";
	static char buffer3[64] = "";
	static char buffer4[64] = "";
	static char filename[128] = "";
	for (int i = 0; i != atom1.size(); i++) {
		if (index == 0) {buffer1[i] = atom1[i];}
		if (index == 1) {buffer2[i] = atom1[i];}
		if (index == 2) {buffer3[i] = atom1[i];}
		if (index == 3) {buffer4[i] = atom1[i];}
	}

	ImGui::SetNextWindowSize({ 398.f,700.f });

	ImGui::Begin("Commands");
	ImGui::SetCursorPos({ 127.f,56.f });
	if (ImGui::Button("clear", { 57.f,19.f }))
	{
		for (int i = 0; i != 4; i++) {
			buffer1[i] = ' ';
			buffer2[i] = ' ';
			buffer3[i] = ' ';
			buffer4[i] = ' ';
		}
		index = -1;
	}
	ImGui::SetCursorPos({ 43.f,170.f });
	if (ImGui::Button("distance", { 61.f,19.f }))
	{
		if (buffer1[0] && buffer2[0] && buffer1[0] != ' ') {
			float c = distances(stoi(buffer1), stoi(buffer2));
			sprintf_s(result1, "%f", c);
		}
	}
	ImGui::SetCursorPos({ 42.f,205.f });
	if (ImGui::Button("angle", { 57.f,19.f }))
	{
		if (buffer1[0] && buffer2[0] && buffer3[0] && buffer1[0] != ' ') {
			float a = angle(stoi(buffer1), stoi(buffer2), stoi(buffer3));
			sprintf_s(result2, "%f", a);
		}
	}
	ImGui::SetCursorPos({ 42.f,240.f });
	if (ImGui::Button("torsion", { 57.f,19.f }))
	{
		if (buffer1[0] && buffer2[0] && buffer3[0] && buffer4[0] && buffer1[0] != ' ') {
			float b = torsion(stoi(buffer1), stoi(buffer2), stoi(buffer3), stoi(buffer4));
			sprintf_s(result3, "%f", b);
		}
	}
	ImGui::SetCursorPos({ 42.f,310.f });
	if (ImGui::Button("bonds", { 57.f,19.f }))
	{
		if (buffer1[0] && buffer1[0] != ' ') {
			std::string temp;
			temp = bond(stoi(buffer1));
			for (int i = 0; i != temp.size(); i++) {
				cout << temp[i];
				buffer[i] = temp[i];
			}
			cout << buffer << endl;
		}
	}
	ImGui::SetCursorPos({ 23.f,90.f });
	ImGui::PushItemWidth(65.000000);
	ImGui::InputText("a1", buffer1, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 98.f,90.f });
	ImGui::PushItemWidth(65.000000);
	ImGui::InputText("a2", buffer2, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 248.f,90.f });
	ImGui::PushItemWidth(65.000000);
	ImGui::InputText("a4", buffer4, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 173.f,90.f });
	ImGui::PushItemWidth(65.000000);
	ImGui::InputText("a3", buffer3, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 116.f,310.f });
	ImGui::PushItemWidth(220.000000);

	ImGui::InputText("", buffer, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 33.f,355.f });
	if (ImGui::Button("Load File", { 76.f,19.f }))
	{
		if (filename) {
			cout << (const char*)filename << endl;

#ifdef _WIN32  //defining windows os
			load((const char*)filename);
#else
			load((const char*)filename);
#endif
			calc_bonds();
			if (bonds.empty()) fileloaded = false;
			else fileloaded = true;
		}
	}
	ImGui::SetCursorPos({ 124.f,355.f });
	ImGui::PushItemWidth(204.000000);
	ImGui::InputText("File name", filename, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 137.f,170.f });
	ImGui::PushItemWidth(139.000000);
	ImGui::InputText("result1", result1, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 137.f,205.f });
	ImGui::PushItemWidth(139.000000);
	ImGui::InputText("result2", result2, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 137.f,240.f });
	ImGui::PushItemWidth(139.000000);
	ImGui::InputText("result3", result3, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 38.f,403.f });
	ImGui::Checkbox("Classic View", &the_bool);
	if (the_bool == true) {
		ImGui::SetCursorPos({ 48.f,435.f });
		ImGui::PushItemWidth(220.000000);
		ImGui::SliderFloat("Azimut", &Azimut, 0, 6.28319);
		ImGui::PopItemWidth();
		ImGui::SetCursorPos({ 46.f,466.f });
		ImGui::PushItemWidth(220.000000);
		ImGui::SliderFloat("Polar", &Polar, -1.56079632679, 1.56079632679);
		ImGui::PopItemWidth();
		ImGui::SetCursorPos({ 46.f,496.f });
		ImGui::PushItemWidth(220.000000);
		ImGui::SliderFloat("Radius", &Radius, 5, 60);
		ImGui::PopItemWidth();
	}
	ImGui::SetCursorPos({ 33.f,550.f });
	if (ImGui::Button("Exit", { 76.f,19.f }))
	{
		exit(0);
	}
	ImGui::SetCursorPos({ 35.f,600.f });
	ImGui::PushItemWidth(287.000000);
	ImGui::Text("CapsLock for lock/unlock advance control. ");
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 35.f,630.f });
	ImGui::PushItemWidth(259.000000);
	ImGui::Text("WASD, alt, cntl, shift - usable keys.");
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 35.f,660.f });
	ImGui::PushItemWidth(322.000000);
	ImGui::Text("LMouse - rotate camera, RMouse - choose vertex.");
	ImGui::PopItemWidth();

	ImGui::End();


}

