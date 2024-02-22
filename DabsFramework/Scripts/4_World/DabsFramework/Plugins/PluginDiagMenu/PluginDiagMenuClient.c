#ifdef DIAG_DEVELOPER

modded class PluginDiagMenuClient
{	
	protected static int m_DabsFrameworkRootID;	
	protected static int m_PPEManagerRootID;	
			
	/*
	typedef Param2<string,bool> PPETemplateDefBool;
	typedef Param4<string,int,int,int> PPETemplateDefInt;
	typedef Param4<string,float,float,float> PPETemplateDefFloat; //name, default, min, max
	typedef Param5<string,float,float,float,float> PPETemplateDefColor; //name, defaults - floats. Min/Max is always the same, no need to define it here.
	//typedef Param4<string,vector,vector,vector> PPETemplateDefVector;
	typedef Param2<string,ref array<float>> PPETemplateDefVector; //needs to be compatible with every type of vector (vector2 to vector4), hence array<float>...
	typedef Param2<string,string> PPETemplateDefTexture; //Currently unused, setting these parameters during runtime can prove problematic
	typedef Param2<string,string> PPETemplateDefResource; //Currently unused, setting these parameters during runtime can prove problematic

	*/
	
	protected static string GetPostProcessEffectName(int id)
	{
		switch (id) {
			case PostProcessEffectType.None: return "None";
			case PostProcessEffectType.UnderWater: return "UnderWater";
			case PostProcessEffectType.SSAO: return "SSAO";
			case PostProcessEffectType.DepthOfField: return "DepthOfField";
			case PostProcessEffectType.HBAO: return "HBAO";
			case PostProcessEffectType.RotBlur: return "RotBlur";
			case PostProcessEffectType.GodRays: return "GodRays";
			case PostProcessEffectType.Rain: return "Rain";
			case PostProcessEffectType.FilmGrain: return "FilmGrain";
			case PostProcessEffectType.RadialBlur: return "RadialBlur";
			case PostProcessEffectType.ChromAber: return "ChromAber";
			case PostProcessEffectType.WetDistort: return "WetDistort";
			case PostProcessEffectType.DynamicBlur: return "DynamicBlur";
			case PostProcessEffectType.ColorGrading: return "ColorGrading";
			case PostProcessEffectType.Colors: return "Colors";
			case PostProcessEffectType.Glow: return "Glow";
			case PostProcessEffectType.SMAA: return "SMAA";
			case PostProcessEffectType.FXAA: return "FXAA";
			case PostProcessEffectType.Median: return "	Median";
			case PostProcessEffectType.SunMask: return "SunMask";
			case PostProcessEffectType.GaussFilter: return "GaussFilter";
			case PostProcessEffectType.SSR: return "SSR";
		}
		
		return "idk bro";
	}
	
	protected override void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();
				
		m_DabsFrameworkRootID = GetModdedDiagID();
		DiagMenu.RegisterMenu(m_DabsFrameworkRootID, "Dabs Framework", DiagMenuIDs.MODDED_MENU);
		{
 			DiagMenu.RegisterItem(GetModdedDiagID(), "[", "Reset Profiler", m_DabsFrameworkRootID, "", CBResetProfiler);
			DiagMenu.RegisterItem(GetModdedDiagID(), "]", "Dump Profiler", m_DabsFrameworkRootID, "", CBDumpProfiler);
			m_PPEManagerRootID = GetModdedDiagID();
			DiagMenu.RegisterMenu(m_PPEManagerRootID, "Post Processing", m_DabsFrameworkRootID);
		}
		
		
	}
	
	static ref map<int, PPEMatClassParameterCommandData> CommandMap = new map<int, PPEMatClassParameterCommandData>();
	static ref map<int, PPEClassBase> ClassMap = new map<int, PPEClassBase>();
	protected PPEManager m_PPEManager;
			
	protected ref map<int, PPERequesterBase> m_DebugRequesters = new map<int, PPERequesterBase>();
	
	protected void OnPPEUpdate(float timeslice)
	{		
		foreach (int menu_id, PPERequesterBase requester: m_DebugRequesters) {
			bool menu_state = DiagMenu.GetBool(menu_id);
			if (!menu_state) {
				continue;
			}
			
			DbgUI.BeginCleanupScope();
			DbgUI.Begin(string.Format("Debug %1", requester.Type()));
			map<int, ref map<int, ref PPERequestParamDataBase>> data_structure = requester.GetActiveRequestStructure();
			foreach (int mat_id, map<int, ref PPERequestParamDataBase> request_params: data_structure) {
				string pp_name = typename.EnumToString(PostProcessEffectType, mat_id);
				DbgUI.Panel(pp_name, 240, 400);
				bool state;
				DbgUI.Check("Test", state);
				foreach (int param_id, PPERequestParamDataBase param_data: request_params) {
					switch (param_data.GetVarType()) {
						case PPEConstants.VAR_TYPE_BOOL: {
							PPERequestParamDataBool param_bool = PPERequestParamDataBool.Cast(param_data);
							
							//bool state = true;
							//DbgUI.Check("Test", state);

							break;
						}
						
						case PPEConstants.VAR_TYPE_INT: {
							
							break;
						}
						
						case PPEConstants.VAR_TYPE_FLOAT: {

							break;
						}
						
						case PPEConstants.VAR_TYPE_COLOR: {

							break;
						}
					}
				}
			}
			
			DbgUI.End();
			DbgUI.EndCleanupScope();
			
			/*
			map<int, ref PPEMatClassParameterCommandData> commands = EnScriptVar<map<int, ref PPEMatClassParameterCommandData>>.Get(ppe_class, "m_MaterialParamMapStructure");
			foreach (int param_id, PPEMatClassParameterCommandData command_data: commands) {
				Param param = command_data.GetDefaultValues();
					
		
				CommandMap[menu_id] = command_data;
				switch (param.Type().ToString()) {
					case "Param2<string,bool>": {
						PPETemplateDefBool def_bool = PPETemplateDefBool.Cast(param);
						//DbgUI.Check(
						DiagMenu.RegisterBool(menu_id, "", def_bool.param1, parent_menu, false, CBSetBool);
						break;
					}
					
					case "Param4<string,int,int,int>": {
						PPETemplateDefInt def_int = PPETemplateDefInt.Cast(param);
						DiagMenu.RegisterRange(menu_id, "", def_int.param1, parent_menu, string.Format("%1,%2,%3,%4", def_int.param3, def_int.param4, def_int.param2, 1));
						break;
					}
					
					case "Param4<string, float, float, float>": {
						PPETemplateDefFloat def_float = PPETemplateDefFloat.Cast(param);
						DiagMenu.RegisterRange(menu_id, "", def_int.param1, parent_menu, string.Format("%1,%2,%3,%4", def_int.param3, def_int.param4, def_int.param2, 0.001));
						break;
					}
				}
			}*/
		}
		
		
		/*
		DbgUI.Check("Show DbgUI", m_ShowDbgUI);
		if (m_ShowDbgUI) {
			DbgUI.Text("DbgUI Test");		
	
			string name = "";
			DbgUI.InputText("name", name);		
			
			if (DbgUI.Button("Print name"))
			{
				Print(name);		
			}
			
			DbgUI.List("test list", m_DbgListSelection, m_DbgOptions);
			
			DbgUI.Text("Choice = " + m_DbgListSelection.ToString());	
			
			DbgUI.Spacer(10);
			DbgUI.SliderFloat("slider", m_DbgSliderValue, 0, 100);
			DbgUI.Text("Slider value = " + ftoa(m_DbgSliderValue));	
		}
		DbgUI.End();*/
	}
	
	void RegisterPPERequester(PPERequesterBase ppe_requester)
	{
		int pp_menu = GetModdedDiagID();
		m_DebugRequesters[pp_menu] = ppe_requester;
		
		DiagMenu.RegisterBool(pp_menu, "", ppe_requester.Type().ToString(), m_PPEManagerRootID);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnPPEUpdate);
	}
			
	static void CBResetProfiler(int value, int id)
	{
		SendDiagRPC(value, RPC_DIAG_RESET_PROFILER, true);
	}
		
	static void CBDumpProfiler(int value, int id)
	{
		SendDiagRPC(value, RPC_DIAG_DUMP_PROFILER, true);
	}
		
	static void SendDiagRPC(string value, int rpc, bool serverOnly = false)
	{
		CachedObjectsParams.PARAM1_STRING.param1 = value;
		SendDiagRPC(CachedObjectsParams.PARAM1_STRING, rpc, serverOnly);
 	}
}

#endif