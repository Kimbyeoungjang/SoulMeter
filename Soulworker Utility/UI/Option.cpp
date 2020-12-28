#include "pch.h"
#include ".\UI\Option.h"
#include ".\UI\HotKey.h"
#include ".\UI\PlayerTable.h"
#include ".\Damage Meter\Damage Meter.h"

UiOption::UiOption()  : _open(0), _framerate(4) {
	_jobBasicColor[0] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(153, 153, 153, 255)));	// Unknown
	_jobBasicColor[1] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(247, 142, 59, 255)));	// 하루
	_jobBasicColor[2] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(59, 147, 247, 255)));	// 어윈
	_jobBasicColor[3] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(247, 59, 156, 255)));	// 릴리
	_jobBasicColor[4] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(247, 190, 59, 255)));	// 진
	_jobBasicColor[5] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(161, 59, 247, 255)));	// 스텔라
	_jobBasicColor[6] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(223, 1, 1, 255)));	// 이리스
	_jobBasicColor[7] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(138, 2, 4, 255)));		// 치이
	_jobBasicColor[8] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(118, 206, 158, 255)));	// 에프넬
	_jobBasicColor[9] = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(128, 128, 64, 255)));	// 이나비
}

UiOption::~UiOption() {
	
}

BOOL UiOption::ShowStyleSelector(const CHAR* label) {

	ImGui::Text(u8"기본 스타일을 선택하세요.");

	if (ImGui::Combo(label, &_styleIndex, "Classic\0Dark\0Light\0"))
	{
		switch (_styleIndex)
		{
		case 0: ImGui::StyleColorsClassic(); break;
		case 1: ImGui::StyleColorsDark(); break;
		case 2: ImGui::StyleColorsLight(); break;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL UiOption::ShowFontSelector() {

	ImFont* font_current = ImGui::GetFont();

	ImGui::Text(u8"Font Scale을 조정하여 Table 크기를 조정 할 수 있습니다.");
	ImGui::DragFloat("Font Scale", &_fontScale, 0.005f, 0.3f, 2.0f, "%.1f");

	font_current->Scale = _fontScale;

	if (ImGui::Checkbox(u8"단위 설정(1K)", (bool*)&_is1K)) {
		if (_is1M)
			_is1M = FALSE;
	}

	if (ImGui::Checkbox(u8"단위 설정(1M)", (bool*)&_is1M)) {
		if (_is1K)
			_is1K = FALSE;
	}

	return TRUE;
}

BOOL UiOption::ShowTableOption() {

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::SliderFloat("WindowBorderSize", &_windowBorderSize, 0.0f, 1.0f, "%.0f");
	style.WindowBorderSize = _windowBorderSize;
	ImGui::SliderFloat2("CellPadding", (float*)&_cellPadding, 0.0f, 20.0f, "%.0f");
	style.CellPadding = _cellPadding;
	ImGui::SliderFloat("Framerate", &_framerate, 1.0f, 4.0f, "%.0f");
	ImGui::DragFloat("Column Font Scale", &_columnFontScale, 0.005f, 0.3f, 2.0f, "%.1f");
	ImGui::DragFloat("Table Font Scale", &_tableFontScale, 0.005f, 0.3f, 2.0f, "%.1f");
	ImGui::Separator();
	ImGui::ColorEdit4("##ColorText", (FLOAT*)&_textColor, ImGuiColorEditFlags_None); 
	ImGui::SameLine(); 	ImGui::Text(ImGui::GetStyleColorName(0));
	style.Colors[0] = _textColor;
	ImGui::ColorEdit4("##ColorBgr", (FLOAT*)&_windowBg, ImGuiColorEditFlags_None);
	ImGui::SameLine();	ImGui::Text(ImGui::GetStyleColorName(2));
	style.Colors[2] = _windowBg;
	ImGui::ColorEdit4("##ColorOutline", (FLOAT*)&_outlineColor, ImGuiColorEditFlags_None);
	ImGui::SameLine();	ImGui::Text(u8"텍스트 Outline 색상");
	ImGui::ColorEdit4("##ColorActiveColor", (FLOAT*)&_activeColor[1], ImGuiColorEditFlags_None);
	ImGui::SameLine();	ImGui::Text(u8"Active Color");
	ImGui::ColorEdit4("##ColorInActiveColor", (FLOAT*)&_activeColor[0], ImGuiColorEditFlags_None);
	ImGui::SameLine();	ImGui::Text(u8"InActive Color");

	const char job[10][32] = { {u8"Unknown"}, {u8"하루"}, {u8"고윈"}, {u8"릴퀴"}, {u8"진따"}, {u8"스텔라"},{u8"이리스"}, {u8"치이"}, {u8"콩프넬"}, {u8"tag: Big Tits(이나비)"} };

	for (int i = 0; i < 10; i++) {
		ImGui::PushID(i);
		ImGui::ColorEdit4("##Color", (FLOAT*)&_jobColor[i], ImGuiColorEditFlags_None);
		ImGui::SameLine();	ImGui::Text(job[i]);

		if (memcmp(&_jobColor[i], &_jobBasicColor[i], sizeof(ImVec4)) != 0) {
			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); 
			if (ImGui::Button(u8"기본 색상으로 바꾸기")) {
				_jobColor[i] = _jobBasicColor[i];
			}
		}

		ImGui::PopID();
	}

	return TRUE;
}

BOOL UiOption::ShowHotkeySetting() {

	const char* text =
		u8"기본 일시정지/재개 키는 CTRL + END 키입니다.\n"
		u8"기본 초기화 키는 CTRL + DEL 키입니다.\n"
		u8"현재는 option.XML에서 직접 수정해야 합니다.\n"
		u8"각 키의 값은 DirectInput Code Table이 기준입니다.\n"
		u8"각 키는 key1부터 순서대로 넣되, 할당하지 않을 키는 -1로 넣어야 합니다.\n";

	ImGui::Text(text);

	return TRUE;
}

VOID UiOption::Helper() {

	static UINT32 helper = 1;
	CHAR name[128] = { 0 };

	UINT monster[4] = { 1323502223, 1324283942, 1321158942, 1174505472 };
	UINT skill[4] = { 72000233, 72000331, 72000433, 72000638 };

	DAMAGEMETER.SetWorldID(21017);
	DAMAGEMETER.SetMyID(3);

	DAMAGEMETER.InsertDB(0, monster[0]);
	DAMAGEMETER.InsertDB(1, monster[1]);
	DAMAGEMETER.InsertDB(2, monster[2]);
	DAMAGEMETER.InsertDB(3, monster[3]);
	DAMAGEMETER.InsertDB(4, monster[0]);
	DAMAGEMETER.InsertDB(5, monster[1]);
	DAMAGEMETER.InsertDB(6, monster[2]);
	DAMAGEMETER.InsertDB(7, monster[3]);

	for (INT i = 0; i < 4; i++) {
		sprintf_s(name, 128, "%s %d",u8"플레이어", helper);
		DAMAGEMETER.InsertPlayerMetadata(helper, name, helper % 10);
		DAMAGEMETER.AddDamage(helper, helper * 10000, helper * 5000, 1, 1, helper * 2, i % 8, skill[i % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 20000, helper * 5000, 1, 1, helper * 3, (i + 1) % 8, skill[(i + 1) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 30000, helper * 5000, 1, 1, helper * 4, (i + 2) % 8, skill[(i + 2) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 40000, helper * 5000, 1, 1, helper * 5, (i + 3) % 8, skill[(i + 3) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 20000, helper * 5000, 1, 1, helper * 3, (i + 4) % 8, skill[(i + 1) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 30000, helper * 5000, 1, 1, helper * 4, (i + 5) % 8, skill[(i + 2) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 40000, helper * 5000, 1, 1, helper * 5, (i + 6) % 8, skill[(i + 3) % 4]);
		DAMAGEMETER.AddDamage(helper, helper * 40000, helper * 5000, 1, 1, helper * 5, (i + 7) % 8, skill[(i + 3) % 4]);
		helper++;
	}
}

VOID UiOption::OpenOption() {

	_open = TRUE;

	if (DAMAGEMETER.size() < 1) {
		Helper();
		PLAYERTABLE.ResizeTalbe();
	}

	ImGui::Begin("Option", 0, ImGuiWindowFlags_None);

		if (ImGui::Button(u8"더미 데미지 추가")) {
			Helper();
		}

		ImGui::SameLine(); 		
		
		if (ImGui::Button(u8"저장하고 종료하기")) {
			SaveOption();
			_open = FALSE;
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
		ShowStyleSelector("Style");
		ImGui::PopItemWidth();

		ImGui::Separator();

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
		ShowFontSelector();
		ImGui::PopItemWidth();

		if (ImGui::BeginTabBar("##tabs")) {
			if (ImGui::BeginTabItem(u8"Table 설정")) {
				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
				ShowTableOption();
				ImGui::PopItemWidth();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Hotkey 설정")) {
				// 귀찮구만
				ShowHotkeySetting();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
}

VOID UiOption::Init() {

	if (!GetOption()) {
		SetBasicOption();
	}
}

BOOL UiOption::GetOption() {

	tinyxml2::XMLDocument doc;
	ImGuiStyle& style = ImGui::GetStyle();

	if (doc.LoadFile(OPTION_FILE_NAME))
		return FALSE;

	tinyxml2::XMLNode* node = doc.FirstChildElement("SDM");

	if (!node)
		return FALSE;

	// Option
	tinyxml2::XMLElement* ele = node->FirstChildElement("Option");

	if (!ele)
		return FALSE;
	
	auto attr = ele->FindAttribute("StyleIndex");

	if (attr == nullptr)
		return FALSE;

	attr->QueryIntValue(&_styleIndex);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read StyleIndex = %d")), _styleIndex);
#endif

	attr = ele->FindAttribute("GlobalScale");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_fontScale);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read FontScale = %.1f")), _fontScale);
#endif

	attr = ele->FindAttribute("TableScale");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_tableFontScale);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read TableFontScale = %.1f")), _fontScale);
#endif

	attr = ele->FindAttribute("ColumnScale");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_columnFontScale);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read ColumnFontScale = %.1f")), _columnFontScale);
#endif

	attr = ele->FindAttribute("K");

	if (attr == nullptr)
		return FALSE;

	attr->QueryIntValue(&_is1K);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read 1K = %d")), _is1K);
#endif

	attr = ele->FindAttribute("M");

	if (attr == nullptr)
		return FALSE;

	attr->QueryIntValue(&_is1M);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read 1M = %d")), _is1M);
#endif

	attr = ele->FindAttribute("Auto");

	if (attr == nullptr)
		return FALSE;

	attr->QueryIntValue(&_autoSize);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read Auto = %d")), _autoSize);
#endif

	attr = ele->FindAttribute("CellPaddingX");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_cellPadding.x);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read CellPadding X = %f")), _cellPadding.x);
#endif

	attr = ele->FindAttribute("CellPaddingY");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_cellPadding.y);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read CellPadding Y = %f")), _cellPadding.y);
#endif

	attr = ele->FindAttribute("Framerate");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_framerate);

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read Framerate = %f")), _framerate);
#endif

	attr = ele->FindAttribute("BorderSize");

	if (attr == nullptr)
		return FALSE;

	attr->QueryFloatValue(&_windowBorderSize);
	style.WindowBorderSize = _windowBorderSize;

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read WindowBorderSize = %f")), _windowBorderSize);
#endif

	ele = ele->NextSiblingElement("Column");

	if (!ele)
		return FALSE;

	const char column_name[8][16] = { {"NAME"}, {"DPS"}, {"DPER"}, {"DAMAGE"}, {"HIT"}, {"CRIT"}, {"HITS"}, {"MAXC"} };

	for (INT i = 0; i < 8; i++) {
		attr = ele->FindAttribute(column_name[i]);

		if (attr == nullptr)
			return FALSE;

		attr->QueryFloatValue(&_columnWidth[i]);
	}

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read Column Width, NAME = %f, DPS = %f, D%% = %f, DAMAGE = %f, HIT = %f, CRIT = %f, HITS = %f, MAXC = %f")), _columnWidth[0], _columnWidth[1], _columnWidth[2], _columnWidth[3], _columnWidth[4], _columnWidth[5], _columnWidth[6], _columnWidth[7]);
#endif

	// Text Color
	ele = ele->NextSiblingElement("TextColor");

	if (!ele)
		return FALSE;

	const char name[4][8] = { {"r"}, {"g"}, {"b"}, {"a"} };

	for (int i = 0; i < 4; i++) {
		attr = ele->FindAttribute(name[i]);

		if (attr == nullptr)
			return FALSE;

		switch (i) {
		case 0:
			attr->QueryFloatValue(&_textColor.x);
			break;
		case 1:
			attr->QueryFloatValue(&_textColor.y);
			break;
		case 2:
			attr->QueryFloatValue(&_textColor.z);
			break;
		case 3:
			attr->QueryFloatValue(&_textColor.w);
			break;
		}
	}

	style.Colors[0] = _textColor;

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read TextColor = %.1f, %.1f, %.1f, %.1f")), _textColor.x, _textColor.y, _textColor.z, _textColor.w);
#endif

	// WindowBg Color
	ele = ele->NextSiblingElement("WindowBgColor");

	if (!ele)
		return FALSE;

	for (int i = 0; i < 4; i++) {
		attr = ele->FindAttribute(name[i]);

		if (attr == nullptr)
			return FALSE;

		switch (i) {
		case 0:
			attr->QueryFloatValue(&_windowBg.x);
			break;
		case 1:
			attr->QueryFloatValue(&_windowBg.y);
			break;
		case 2:
			attr->QueryFloatValue(&_windowBg.z);
			break;
		case 3:
			attr->QueryFloatValue(&_windowBg.w);
			break;
		}
	}

	style.Colors[2] = _windowBg;

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read WindowBgColor = %.1f, %.1f, %.1f, %.1f")), _windowBg.x, _windowBg.y, _windowBg.z, _windowBg.w);
#endif

	// Outline Color
	ele = ele->NextSiblingElement("OutlineColor");
		
	if (!ele)
		return FALSE;

	for (int i = 0; i < 4; i++) {
		attr = ele->FindAttribute(name[i]);

		if (attr == nullptr)
			return FALSE;
		
		switch (i) {
		case 0:
			attr->QueryFloatValue(&_outlineColor.x);
			break;
		case 1:
			attr->QueryFloatValue(&_outlineColor.y);
			break;
		case 2:
			attr->QueryFloatValue(&_outlineColor.z);
			break;
		case 3:
			attr->QueryFloatValue(&_outlineColor.w);
			break;
		}
	}

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read OutlineColor = %.1f, %.1f, %.1f, %.1f")), _outlineColor.x, _outlineColor.y, _outlineColor.z, _outlineColor.w);
#endif

	// ActiveColor
	ele = ele->NextSiblingElement("ActiveColor");

	if (!ele)
		return FALSE;

	for (int i = 0; i < 4; i++) {
		attr = ele->FindAttribute(name[i]);

		if (attr == nullptr)
			return FALSE;

		switch (i) {
		case 0:
			attr->QueryFloatValue(&_activeColor[1].x);
			break;
		case 1:
			attr->QueryFloatValue(&_activeColor[1].y);
			break;
		case 2:
			attr->QueryFloatValue(&_activeColor[1].z);
			break;
		case 3:
			attr->QueryFloatValue(&_activeColor[1].w);
			break;
		}
	}

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read ActiveColor = %.1f, %.1f, %.1f, %.1f")), _activeColor[1].x, _activeColor[1].y, _activeColor[1].z, _activeColor[1].w);
#endif

	ele = ele->NextSiblingElement("InActiveColor");

	if (!ele)
		return FALSE;

	for (int i = 0; i < 4; i++) {
		attr = ele->FindAttribute(name[i]);

		if (attr == nullptr)
			return FALSE;

		switch (i) {
		case 0:
			attr->QueryFloatValue(&_activeColor[0].x);
			break;
		case 1:
			attr->QueryFloatValue(&_activeColor[0].y);
			break;
		case 2:
			attr->QueryFloatValue(&_activeColor[0].z);
			break;
		case 3:
			attr->QueryFloatValue(&_activeColor[0].w);
			break;
		}
	}

#if DEBUG_READ_XML == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("Read InActiveColor = %.1f, %.1f, %.1f, %.1f")), _activeColor[0].x, _activeColor[0].y, _activeColor[0].z, _activeColor[0].w);
#endif

	for (int i = 0; i < 10; i++) {
		char temp[32] = { 0 };
		sprintf_s(temp, 32, "JobColor%d", i);
		ele = ele->NextSiblingElement(temp);

		if (!ele)
			return FALSE;

		for (int j = 0; j < 4; j++) {
			attr = ele->FindAttribute(name[j]);

			if (attr == nullptr)
				return FALSE;

			switch (j) {
			case 0:
				attr->QueryFloatValue(&_jobColor[i].x);
				break;
			case 1:
				attr->QueryFloatValue(&_jobColor[i].y);
				break;
			case 2:
				attr->QueryFloatValue(&_jobColor[i].z);
				break;
			case 3:
				attr->QueryFloatValue(&_jobColor[i].w);
				break;
			}
		}

#if DEBUG_READ_XML == 1
		Log::WriteLog(const_cast<LPTSTR>(_T("Read JobColor%d = %.1f, %.1f, %.1f, %.1f")), i, _jobColor[i].x, _jobColor[i].y, _jobColor[i].z, _jobColor[i].w);
#endif
	}
	
	INT hotkeyID = 0;

	do {

		INT key[3] = { -1 };
		CHAR name2[AUTO_HOTKEY_NAME_LEN] = { 0 };
		sprintf_s(name2, AUTO_HOTKEY_NAME_LEN, "HOTKEY%d", hotkeyID++);

		ele = ele->NextSiblingElement(name2);

		if (ele == nullptr)
			break;

		for (INT i = 0; i < 3; i++) {

			char temp[12] = { 0 };
			sprintf_s(temp, 12, "key%d", i + 1);

			attr = ele->FindAttribute(temp);

			if (attr == nullptr)
				break;

			attr->QueryIntValue(&key[i]);
		}

		if (ele->GetText() != nullptr) {
			strcpy_s(name2, ele->GetText());
		}

#if DEBUG_READ_XML == 1
		Log::WriteLog(const_cast<LPTSTR>(_T("Read Hotkey %s, key1 = %d, key2 = %d, key3 = %d")), name2, key[0], key[1], key[2]);
#endif
		
		if (strcmp(name2, u8"Toogle") == 0)
			HOTKEY.InsertHotkeyToogle(key[0], key[1], key[2]);
		else if (strcmp(name2, u8"Clear") == 0)
			HOTKEY.InsertHotkeyStop(key[0], key[1], key[2]);
		
	} while (TRUE);

	return TRUE;
}

BOOL UiOption::SaveOption() {
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration();
	doc.LinkEndChild(dec);

	tinyxml2::XMLElement* root = doc.NewElement("SDM");
	doc.LinkEndChild(root);

	tinyxml2::XMLElement* option = doc.NewElement("Option");
	root->LinkEndChild(option);

	option->SetAttribute("StyleIndex", _styleIndex);
	option->SetAttribute("GlobalScale", _fontScale);
	option->SetAttribute("TableScale", _tableFontScale);
	option->SetAttribute("ColumnScale", _columnFontScale);
	option->SetAttribute("K", _is1K);
	option->SetAttribute("M", _is1M);
	option->SetAttribute("Auto", _autoSize);
	option->SetAttribute("CellPaddingX", _cellPadding.x);
	option->SetAttribute("CellPaddingY", _cellPadding.y);
	option->SetAttribute("Framerate", _framerate);
	option->SetAttribute("BorderSize", _windowBorderSize);

	tinyxml2::XMLElement* column = doc.NewElement("Column");
	root->LinkEndChild(column);

	column->SetAttribute("NAME", _columnWidth[0]);
	column->SetAttribute("DPS", _columnWidth[1]);
	column->SetAttribute("DPER", _columnWidth[2]);
	column->SetAttribute("DAMAGE", _columnWidth[3]);
	column->SetAttribute("HIT", _columnWidth[4]);
	column->SetAttribute("CRIT", _columnWidth[5]);
	column->SetAttribute("HITS", _columnWidth[6]);
	column->SetAttribute("MAXC", _columnWidth[7]);

	tinyxml2::XMLElement* text_color = doc.NewElement("TextColor");
	root->LinkEndChild(text_color);
	text_color->SetAttribute("r", _textColor.x);
	text_color->SetAttribute("g", _textColor.y);
	text_color->SetAttribute("b", _textColor.z);
	text_color->SetAttribute("a", _textColor.w);

	tinyxml2::XMLElement* windowbg_color = doc.NewElement("WindowBgColor");
	root->LinkEndChild(windowbg_color);
	windowbg_color->SetAttribute("r", _windowBg.x);
	windowbg_color->SetAttribute("g", _windowBg.y);
	windowbg_color->SetAttribute("b", _windowBg.z);
	windowbg_color->SetAttribute("a", _windowBg.w);
	
	tinyxml2::XMLElement* outline_color = doc.NewElement("OutlineColor");
	root->LinkEndChild(outline_color);
	outline_color->SetAttribute("r", _outlineColor.x);
	outline_color->SetAttribute("g", _outlineColor.y);
	outline_color->SetAttribute("b", _outlineColor.z);
	outline_color->SetAttribute("a", _outlineColor.w);

	tinyxml2::XMLElement* active_color = doc.NewElement("ActiveColor");
	root->LinkEndChild(active_color);
	active_color->SetAttribute("r", _activeColor[1].x);
	active_color->SetAttribute("g", _activeColor[1].y);
	active_color->SetAttribute("b", _activeColor[1].z);
	active_color->SetAttribute("a", _activeColor[1].w);

	tinyxml2::XMLElement* inactive_color = doc.NewElement("InActiveColor");
	root->LinkEndChild(inactive_color);
	inactive_color->SetAttribute("r", _activeColor[0].x);
	inactive_color->SetAttribute("g", _activeColor[0].y);
	inactive_color->SetAttribute("b", _activeColor[0].z);
	inactive_color->SetAttribute("a", _activeColor[0].w);

	for (int i = 0; i < 10; i++) {

		char buffer[32] = { 0 };
		sprintf_s(buffer, 32, "JobColor%d", i);
		tinyxml2::XMLElement* job_color = doc.NewElement(buffer);
		root->LinkEndChild(job_color);

		job_color->SetAttribute("r", _jobColor[i].x);
		job_color->SetAttribute("g", _jobColor[i].y);
		job_color->SetAttribute("b", _jobColor[i].z);
		job_color->SetAttribute("a", _jobColor[i].w);
	}
	
	INT hotkeyid = 0;

	for (auto itr = HOTKEY.begin(); itr != HOTKEY.end(); itr++) {

		char buffer[32] = { 0 };
		sprintf_s(buffer, 32, "HOTKEY%d", hotkeyid++);
		tinyxml2::XMLElement* hotkey = doc.NewElement(buffer);
		root->LinkEndChild(hotkey);

		hotkey->SetAttribute("key1", (*itr)->GetKey()[0]);
		hotkey->SetAttribute("key2", (*itr)->GetKey()[1]);
		hotkey->SetAttribute("key3", (*itr)->GetKey()[2]);
		hotkey->SetText((*itr)->GetName());
	}

	doc.SaveFile(OPTION_FILE_NAME);

	return TRUE;
}

BOOL UiOption::SetBasicOption() {

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	_framerate = 4;
	_windowBorderSize = 1;

	_fontScale = 1; _columnFontScale = 1; _tableFontScale = 1; _styleIndex = 1;

	for (int i = 0; i < 10; i++)
		_jobColor[i] = _jobBasicColor[i];

	_outlineColor = ImVec4(ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 0, 255)));
	_activeColor[0] = style.Colors[10];
	_activeColor[1] = style.Colors[11];
	_textColor = style.Colors[0];
	_windowBg = style.Colors[2];
	
	_is1K = FALSE;
	_is1M = FALSE;
	_autoSize = TRUE;

	_cellPadding.x = 0;
	_cellPadding.y = 0;

	for (INT i = 0; i < 8; i++)
		_columnWidth[i] = 100;
	
	HOTKEY.InsertHotkeyToogle(DIK_LCONTROL, DIK_END, -1);
	HOTKEY.InsertHotkeyStop(DIK_LCONTROL, DIK_DELETE, -1);

	Helper();
	PLAYERTABLE.ResizeTalbe();
	_open = TRUE;

	return TRUE;
}

const ImU32& UiOption::GetJobColor(UINT index) {

	if (index < 0 || index > 9)
		return ImGui::ColorConvertFloat4ToU32(_jobColor[0]);

	return ImGui::ColorConvertFloat4ToU32(_jobColor[index]);
}

const ImU32& UiOption::GetOutlineColor() {
	return ImGui::ColorConvertFloat4ToU32(_outlineColor);
}

const FLOAT& UiOption::GetFontScale() {
	return _fontScale;
}

const FLOAT& UiOption::GetColumnFontScale() {
	return _columnFontScale;
}

const FLOAT& UiOption::GetTableFontScale() {
	return _tableFontScale;
}

const ImVec4& UiOption::GetActiveColor() {
	return _activeColor[1];
}

const ImVec4& UiOption::GetInActiveColor() {
	return _activeColor[0];
}

const BOOL& UiOption::is1K() {
	return _is1K;
}

const BOOL& UiOption::is1M() {
	return _is1M;
}

VOID UiOption::Update() {

	ImFont* font = ImGui::GetFont();
	font->Scale = _fontScale;

	if (_open)
		OpenOption();

#if DEBUG_COLUMN_WIDTH == 1
	for (INT i = 0; i < 8; i++)
		Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [Column Width] [%d] [%f]")), i, UIOPTION[i]);
#endif
}

const BOOL& UiOption::isOption() {
	return _open;
}

const BOOL& UiOption::isAutoSize() {
	return _autoSize;
}

const FLOAT& UiOption::GetColumnNameSize() { 
	return _columnWidth[0];
}
const FLOAT& UiOption::GetColumnDpsSize(){
	return _columnWidth[1];
}
const FLOAT& UiOption::GetColumnDperSize(){
	return _columnWidth[2];
}
const FLOAT& UiOption::GetColumnDamageSize(){
	return _columnWidth[3];
}
const FLOAT& UiOption::GetColumnHitSize(){
	return _columnWidth[4];
}
const FLOAT& UiOption::GetColumnCritSize(){
	return _columnWidth[5];
}
const FLOAT& UiOption::GetColumnHitsSize(){
	return _columnWidth[6];
}
const FLOAT& UiOption::GetColumnMaxcSize(){
	return _columnWidth[7];
}

VOID UiOption::SetColumnNameSize(FLOAT size) {
	_columnWidth[0] = size;
}
VOID UiOption::SetColumnDpsSize(FLOAT size) {
	_columnWidth[1] = size;
}
VOID UiOption::SetColumnDperSize(FLOAT size) {
	_columnWidth[2] = size;
}
VOID UiOption::SetColumnDamageSize(FLOAT size) {
	_columnWidth[3] = size;
}
VOID UiOption::SetColumnHitSize(FLOAT size) {
	_columnWidth[4] = size;
}
VOID UiOption::SetColumnCritSize(FLOAT size) {
	_columnWidth[5] = size;
}
VOID UiOption::SetColumnHitsSize(FLOAT size) {
	_columnWidth[6] = size;
}
VOID UiOption::SetColumnMaxcSize(FLOAT size) {
	_columnWidth[7] = size;
}

FLOAT& UiOption::operator[](INT index) {

	if (index < 0 || index > 7)
		assert(index < 0 || index > 7);

	return _columnWidth[index];
}

const FLOAT& UiOption::GetFramerate() {
	return _framerate;
}

const ImVec4& UiOption::GetWindowBGColor() {
	return _windowBg;
}