modded class DayZGame
{	
	protected ref LoggerManager m_LoggerManager;
	protected ref EventManager m_EventManager;
	
	protected ref map<typename, ProfileSettings> m_ProfileSettings = new map<typename, ProfileSettings>();
	
	protected ref TTypeNameTypenameMap m_WidgetControllerHashMap = new TTypeNameTypenameMap();
	protected ref TypeConversionHashMap m_TypeConverterHashMap = new TypeConversionHashMap();
				
	void DayZGame()
	{		
		m_LoggerManager = new LoggerManager(this);
		
		RegisterConversionTemplates(m_TypeConverterHashMap); 
		RegisterWidgetControllers(m_WidgetControllerHashMap);
		
		// dedi and offline
#ifdef SERVER
		m_EventManager = new EventManager();
#endif
	}
			
	// Override THIS to add your own Custom Conversion Templates
	// this determines how data is represented in the UI itself
	// i.e. you can assign a TextWidget to float, due to the TypeConversion's GetString()
	void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
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
		widget_controllers.Insert(Widget, WidgetBaseController);
		widget_controllers.Insert(MapWidget, WidgetBaseController);
		widget_controllers.Insert(SpacerBaseWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(WrapSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(GridSpacerWidget, SpacerBaseWidgetController);
		widget_controllers.Insert(ScrollWidget, SpacerBaseWidgetController);

		widget_controllers.Insert(ButtonWidget, ButtonWidgetController);
		widget_controllers.Insert(CheckBoxWidget, CheckBoxWidgetController);
		widget_controllers.Insert(EditBoxWidget, EditBoxWidgetController);
		widget_controllers.Insert(PasswordEditBoxWidget, EditBoxWidgetController);
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
	
	override void RegisterProfilesOptions()
	{
		super.RegisterProfilesOptions();
		
		// Load all ProfileSettings classes
		foreach (Param2<typename, string> profile_settings_type: RegisterProfileSetting.Instances) {
			ProfileSettings profile_settings = ProfileSettings.Cast(profile_settings_type.param1.Spawn());
			if (!profile_settings) {
				continue;
			}
			
			m_ProfileSettings[profile_settings_type.param1] = profile_settings;
		}
	}

#ifdef DIAG_DEVELOPER
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);
		
		switch (rpc_type) {
			case Debug.RPC_UPDATE_DEBUG_SHAPE: {
				Debug.OnDebugShapeRPC(ctx);		
				break;
			}
		}
	}
#endif
	
	// this is the init of event manager for clients
	override void OnEvent(EventType eventTypeId, Param params)
	{
		switch (eventTypeId) {
			case MPSessionStartEventTypeID: {
				m_EventManager = new EventManager();
				break;
			}
			
			case MPSessionEndEventTypeID: {
				delete m_EventManager;
				break;
			}
		}
		
		super.OnEvent(eventTypeId, params);
	}
			
	DayZPlayer GetPlayerByIdentity(PlayerIdentity identity)
	{		
		int high, low;
		if (!IsMultiplayer()) {
			return GetPlayer();
		}
		
		if (!identity) {
			return null;
		}
		
		GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), low, high);
		return DayZPlayer.Cast(GetObjectByNetworkId(low, high));
	}
	
	TypeConverter GetTypeConversion(typename type) 
	{				
		return m_TypeConverterHashMap[type]; 
	}
		
	WidgetController GetWidgetController(Widget data) 
	{	
		WidgetController widgetController = WidgetController.Cast(m_WidgetControllerHashMap.Get(data.Type()).Spawn());
		g_Script.Call(widgetController, "SetWidget", data);
		return widgetController;
	}	
	
	string GetWorldNameEx(bool format = true)
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		if (format) {
			world_name.ToLower();
		}
		
		return world_name;
	}
	
	LoggerManager GetLoggerManager()
	{
		return m_LoggerManager;
	}
	
	EventManager GetEventManager()
	{
		return m_EventManager;
	}
	
	ProfileSettings GetProfileSetting(typename profile_settings_type)
	{
		return m_ProfileSettings[profile_settings_type];
	}
}