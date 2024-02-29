modded class OptionsMenu
{	
	protected ref map<int, ref OptionMenuCustom> m_CustomOptions = new map<int, ref OptionMenuCustom>();
	
	protected ref ScriptCaller m_OnChangedCaller;
	
	void ~OptionsMenu()
	{
		delete m_CustomOptions;
		delete m_OnChangedCaller;
	}
	
	override Widget Init()
	{
		layoutRoot = super.Init();
				
		m_OnChangedCaller = ScriptCaller.Create(OnChanged);
		
		foreach (Param2<typename, string> register_profile_setting: RegisterProfileSetting.Instances) {	
			if (!RegisterProfileSettingBase.Instances[register_profile_setting.param1] || RegisterProfileSettingBase.Instances[register_profile_setting.param1].Count() == 0) {
				continue;
			}
			
			ProfileSettings profile_settings = GetDayZGame().GetProfileSetting(register_profile_setting.param1);
			if (!profile_settings) {
				continue;
			}
			
			int index = m_Tabber.AddTab(register_profile_setting.param2);
			m_CustomOptions[index] = new OptionMenuCustom(layoutRoot.FindAnyWidget("Tab_" + index), profile_settings, m_OnChangedCaller);
		}		
				
		return layoutRoot;
	}
	
	override void OnChanged()
	{
		super.OnChanged();
	
		foreach (int id, OptionMenuCustom custom_menu: m_CustomOptions) {
			if (custom_menu && custom_menu.IsChanged()) {
				m_Apply.ClearFlags(WidgetFlags.IGNOREPOINTER);
				ColorNormal(m_Apply);
			}
		}
	}
	
	override void Apply()
	{
		super.Apply();

		bool custom_option_restart;
		foreach (int id, OptionMenuCustom custom_menu: m_CustomOptions) {
			if (custom_menu && custom_menu.IsChanged()) {
				custom_menu.Apply();
				
				if (custom_menu.NeedsRestart()) {
					custom_option_restart = true;
				}
			}
		}
				
		// one of the custom options required a restart
		if (!m_Options.NeedRestart() && custom_option_restart) {
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#menu_restart_needed", 117, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
		}
	}
	
	override void ResetCurrentTab()
	{
		super.ResetCurrentTab();
		
		if (m_CustomOptions.Contains(m_ActiveTabIdx)) {
			m_CustomOptions[m_ActiveTabIdx].Revert();
		}
	}
	
	override bool IsAnyTabChanged()
	{
		if (super.IsAnyTabChanged()) {
			return true;
		}
		
		foreach (int id, OptionMenuCustom custom_menu: m_CustomOptions) {
			if (custom_menu.IsChanged()) {
				return true;
			}
		}
		
		return false;
	}
}