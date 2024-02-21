#ifdef DIAG_DEVELOPER

modded class PluginDiagMenu
{	
	static const int RPC_DIAG_RESET_PROFILER = 3566037;
	static const int RPC_DIAG_DUMP_PROFILER = 3566038;
	
	protected static int m_DabsFrameworkRootID;
		protected static int m_PPEffectsID;
	
			protected static ref map<int, int> m_PPEEffectMenuIDs = new map<int, int>();
			protected static ref map<int, ref array<int>> m_PPEffectsParamData = new map<int, ref array<int>>();
	
		protected static int m_ResetProfilerID;
		protected static int m_DumpProfilerID;

	protected override void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();
		
		m_DabsFrameworkRootID = GetModdedDiagID();
		
		m_PPEffectsID = GetModdedDiagID();
		
		PPEManagerStatic.CreateManagerStatic();
		PPEManager manager = PPEManagerStatic.GetPPEManager();
		map<int, ref PPEClassBase> classes = EnScriptVar<map<int, ref PPEClassBase>>.Get(manager, "m_PPEClassMap");
		
		Print(classes);
		
		
		m_ResetProfilerID = GetModdedDiagID();
		m_DumpProfilerID = GetModdedDiagID();		
		
	}
		
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
		
		DiagMenu.RegisterMenu(m_DabsFrameworkRootID, "Dabs Framework", DiagMenuIDs.MODDED_MENU);
		{
			DiagMenu.RegisterMenu(m_PPEffectsID, "Post Processing", m_DabsFrameworkRootID);
			{
				map<int, ref PPEClassBase> classes = EnScriptVar<map<int, ref PPEClassBase>>.Get(PPEManagerStatic.GetPPEManager(), "m_PPEClassMap");
				foreach (int id, PPEClassBase class_: classes) {	
					
					
				}
			}
			
 			DiagMenu.RegisterItem(m_ResetProfilerID, "[", "Reset Profiler", m_DabsFrameworkRootID, "");
			DiagMenu.RegisterItem(m_DumpProfilerID, "]", "Dump Profiler", m_DabsFrameworkRootID, "");
		}
	}
		
	override void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(player, rpc_type, ctx);
		
		switch (rpc_type) {						
			case RPC_DIAG_RESET_PROFILER: {
				EnProfiler.ResetSession(true);
				break;
			}
			
			case RPC_DIAG_DUMP_PROFILER: {
				EnProfiler.Dump();
				break;
			}
		}
	}
}

#endif

