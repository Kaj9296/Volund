#pragma once


class UI;

class Widget
{
public:

	virtual const char* GetName() = 0;

	virtual void OnEvent(VL::Event* E) {};

	virtual void Draw(VL::TimeStep TS) = 0;

	Widget(UI* ui, bool Active = false);

	virtual ~Widget() = default;

	bool _IsActive = false;

protected:

	static void StartCombo();

	static void NextColumn();

	static void EndCombo();

	static void Align(float Width, float Alignment);

	static std::string TextSelectorControl(const std::string& Name, const std::string& Default, const std::vector<std::string>& SelectableValues);

	static std::string TextControl(const std::string& Name, const std::string& Default);
	
	static std::string FileSelectorControl(const std::string& Name, const std::string& Default, const char* Filter, VL::Ref<VL::Window> Owner);

	static void BoolControl(const std::string& Name, bool* Value);

	static void FloatControl(const std::string& Name, float* Value);

	static void Vec3Control(std::string_view Name, VL::Vec3* Value, float Speed = 0.1f, float DefaultValue = 0.0f);

	UI* _UI = nullptr;

	static inline VL::Entity _SelectedEntity = NULL;
};
