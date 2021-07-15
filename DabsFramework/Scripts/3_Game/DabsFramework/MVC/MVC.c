/*
	A bunch of this code in this file is an unmanaged mess. MVC Singleton is really weird
	also dont think i need to create an instance of the TypeConverters every time we use it. probably just use one instance

	edit1: okay fixed the type converter thingy

	edit2: drunk tyler seems to have solved most of it, however the whole TypeConverter
	fiasco actually might bite us in the ass, considering it was previously just creating 
	each TypeConverter on each ViewBinding PogO

	
*/

class MVC
{
	static ref MVC m_Instance;
	
	static MVC Start()
	{
		m_Instance = new MVC();
		return m_Instance;
	}

	static void Stop()
	{
		delete m_Instance;
	}
	
	static MVC GetInstance()
	{
		return m_Instance;
	}
		
	protected static ref TTypeNameTypenameMap m_WidgetControllerHashMap;
	static WidgetController GetWidgetController(Widget data) 
	{
		if (!m_Instance) {
			MVC.Start();
		}
	
		WidgetController widgetController = WidgetController.Cast(m_WidgetControllerHashMap.Get(data.Type()).Spawn());
		g_Script.Call(widgetController, "SetWidget", data);
		return widgetController;
	}	
	
	protected static ref TypeConversionHashMap m_TypeConverterHashMap;
	static TypeConverter GetTypeConversion(typename type) 
	{
		if (!m_Instance) {
			MVC.Start();
		}
				
		return m_TypeConverterHashMap[type]; 
	}
	
	void MVC()
	{
		Log("MVC");
		
		m_TypeConverterHashMap = new TypeConversionHashMap();
		RegisterConversionTemplates(m_TypeConverterHashMap);
		
		m_WidgetControllerHashMap = new TTypeNameTypenameMap();
		RegisterWidgetControllers(m_WidgetControllerHashMap);
	}
	
	void ~MVC() 
	{ 
		Log("~MVC"); 
	}
	
	// Override THIS to add your own Custom Conversion Templates
	// this determines how data is represented in the UI itself
	// i.e. you can assign a TextWidget to float, due to the TypeConversion's GetString()
	void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		Log("MVC::RegisterConversionTemplates");
		type_conversions.Insert(bool, TypeConversionBool);
		type_conversions.Insert(int, TypeConversionInt);
		type_conversions.Insert(float, TypeConversionFloat);
		type_conversions.Insert(string, TypeConversionString);
		type_conversions.Insert(vector, TypeConversionVector);
		type_conversions.Insert(Widget, TypeConversionWidget);
		type_conversions.Insert(Object, TypeConversionObject);
		type_conversions.Insert(Observable, TypeConversionObservable);
		type_conversions.Insert(ScriptedViewBase, TypeConversionScriptView);
		type_conversions.Insert(StringEvaluater, TypeConversionEquation);
	}
	
	// Override THIS to add your own Widget widget_controllers 
	// this determins how the Widget controls the data sent to it
	// Great for prefabs
	void RegisterWidgetControllers(out TTypeNameTypenameMap widget_controllers)
	{
		Log("MVC::RegisterWidgetControllers");
		
		widget_controllers.Insert(Widget, WidgetBaseController);
		widget_controllers.Insert(SpacerBaseWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(WrapSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(GridSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(ScrollWidget, SpacerBaseWidgetController);

		widget_controllers.Insert(ButtonWidget, ButtonWidgetController);
		widget_controllers.Insert(CheckBoxWidget, CheckBoxWidgetController);
		widget_controllers.Insert(EditBoxWidget, EditBoxWidgetController);
		widget_controllers.Insert(SliderWidget, SliderWidgetController);
		widget_controllers.Insert(ProgressBarWidget, ProgressBarController);
		widget_controllers.Insert(TextWidget, TextWidgetController);
		widget_controllers.Insert(MultilineTextWidget, TextWidgetController);
		widget_controllers.Insert(MultilineEditBoxWidget, MultilineEditBoxWidgetController);
		widget_controllers.Insert(XComboBoxWidget, XComboBoxWidgetController);
		widget_controllers.Insert(ImageWidget, ImageWidgetController);
		widget_controllers.Insert(TextListboxWidget, TextListboxController);
		widget_controllers.Insert(RichTextWidget, TextWidgetController);
		widget_controllers.Insert(ItemPreviewWidget, ItemPreviewWidgetController);
		widget_controllers.Insert(PlayerPreviewWidget, PlayerPreviewWidgetController);
		widget_controllers.Insert(HtmlWidget, HtmlWidgetController);
		widget_controllers.Insert(CanvasWidget, CanvasWidgetController);
	}	
	
	static void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("MVCLog::Trace %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
	
	static void Log(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("MVCLog::Log %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		string msg = string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		//PrintFormat("MVCLog::Error %1", msg);
		Error2("MVC Error", msg);
		
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog("MVC Error", msg);
#endif
	}
}

// Workbench safe variant of GetGame()
// https://feedback.bistudio.com/T153287
DayZGame GetWorkbenchGame() 
{
	// dont want to call this while actually in game
#ifdef COMPONENT_SYSTEM 
	if (!g_Game) {
		g_Game = new DayZGame();
	}
#endif
	
	return g_Game;
}