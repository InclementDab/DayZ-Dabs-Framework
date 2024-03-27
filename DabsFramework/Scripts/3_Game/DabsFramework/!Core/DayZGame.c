modded class EffectSound
{
	AbstractWave GetWaveObject()
	{
		return m_SoundWaveObject;
	}
}

modded class DayZGame
{	
	protected ref LoggerManager m_LoggerManager;
	protected ref EventManager m_EventManager;
	protected ref SuppressedObjectManager m_SuppressedObjectManager;
	
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
		m_SuppressedObjectManager = new SuppressedObjectManager();
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
#ifdef DIAG_DEVELOPER
		if (m_EventLogging) {		
			string event_type = DebugEventTypeId(eventTypeId);
			if (event_type != string.Empty) {
				Print(string.Format("[%1]: %2", event_type, params));
			}
		}
#endif

		switch (eventTypeId) {
			case MPSessionStartEventTypeID: {
				m_EventManager = new EventManager();
				m_SuppressedObjectManager = new SuppressedObjectManager();
				break;
			}
			
			case MPSessionEndEventTypeID: {
				delete m_EventManager;
				delete m_SuppressedObjectManager;
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
	
	void SetDate(DateTime date)
	{
		int year, month, day, hour, minute, second;
		date.GetDate(year, month, day, hour, minute, second);
		GetWorld().SetDate(year, month, day, hour, minute);
	}
	
	DateTime GetDate()
	{
		int year, month, day, hour, minute;
		GetWorld().GetDate(year, month, day, hour, minute);
		return DateTime.Create(year, month, day, hour, minute);
	}
	
	LoggerManager GetLoggerManager()
	{
		return m_LoggerManager;
	}
	
	EventManager GetEventManager()
	{
		return m_EventManager;
	}
	
	SuppressedObjectManager GetSuppressedObjectManager()
	{
		return m_SuppressedObjectManager;
	}
	
	ProfileSettings GetProfileSetting(typename profile_settings_type)
	{
		return m_ProfileSettings[profile_settings_type];
	}
	
	Object GetTerrainObject()
	{
		Object terrain_object;
		vector position, normal;
		float fraction;
		if (!DayZPhysics.RayCastBullet(vector.Zero, vector.Up * -1000.0, PhxInteractionLayers.TERRAIN, null, terrain_object, position, normal, fraction)) {
			Error("Couldnt find terrain with raycast");
			return null;
		}
		
		return terrain_object;
	}

#ifdef DIAG_DEVELOPER
	protected bool m_EventLogging;
	void ToggleEventLogging(bool enabled)
	{
		m_EventLogging = enabled;
	}
			
	// GetGame().RespawnPlayer() - ClientRespawnEventTypeID, then ClientPrepareEventTypeID
	// after the server processes the respawn, takes a few seconds
	// ClientNewEventTypeID, then ClientNewReadyEventTypeID
	static string DebugEventTypeId(EventType event_type)
	{
		switch (event_type) {
			case StartupEventTypeID: return "StartupEventTypeID";
			case WorldCleaupEventTypeID: return "WorldCleaupEventTypeID";
			case MPSessionStartEventTypeID: return "MPSessionStartEventTypeID";
			case MPSessionEndEventTypeID: return "MPSessionEndEventTypeID";
			case MPSessionFailEventTypeID: return "MPSessionFailEventTypeID";
			case MPSessionPlayerReadyEventTypeID: return "MPSessionPlayerReadyEventTypeID";
			//case MPConnectionLostEventTypeID: return "MPConnectionLostEventTypeID";
			case MPConnectionCloseEventTypeID: return "MPConnectionCloseEventTypeID";
			case ProgressEventTypeID: return "ProgressEventTypeID";
			case NetworkManagerClientEventTypeID: return "NetworkManagerClientEventTypeID";
			case NetworkManagerServerEventTypeID: return "NetworkManagerServerEventTypeID";
			case DialogQueuedEventTypeID: return "DialogQueuedEventTypeID";
			case ChatMessageEventTypeID: return "ChatMessageEventTypeID";
			case ChatChannelEventTypeID: return "ChatChannelEventTypeID";
			case ClientConnectedEventTypeID: return "ClientConnectedEventTypeID";
			case ClientPrepareEventTypeID: return "ClientPrepareEventTypeID";
			case ClientNewEventTypeID: return "ClientNewEventTypeID";
			case ClientNewReadyEventTypeID: return "ClientNewReadyEventTypeID";
			case ClientRespawnEventTypeID: return "ClientRespawnEventTypeID";
			case ClientReconnectEventTypeID: return "ClientReconnectEventTypeID";
			case ClientReadyEventTypeID: return "ClientReadyEventTypeID";
			case ClientDisconnectedEventTypeID: return "ClientDisconnectedEventTypeID";
			case ClientRemovedEventTypeID: return "ClientRemovedEventTypeID";
			case ConnectivityStatsUpdatedEventTypeID: return "ConnectivityStatsUpdatedEventTypeID";
			case ServerFpsStatsUpdatedEventTypeID: return "ServerFpsStatsUpdatedEventTypeID";
			case LogoutCancelEventTypeID: return "LogoutCancelEventTypeID";
			case LoginTimeEventTypeID: return "LoginTimeEventTypeID";
			case RespawnEventTypeID: return "RespawnEventTypeID";
			case PreloadEventTypeID: return "PreloadEventTypeID";
			case LogoutEventTypeID: return "LogoutEventTypeID";
			case LoginStatusEventTypeID: return "LoginStatusEventTypeID";
			case SelectedUserChangedEventTypeID: return "SelectedUserChangedEventTypeID";
			//case ScriptLogEventTypeID: return "ScriptLogEventTypeID";
			case VONStateEventTypeID: return "VONStateEventTypeID";
			case VONStartSpeakingEventTypeID: return "VONStartSpeakingEventTypeID";
			case VONStopSpeakingEventTypeID: return "VONStopSpeakingEventTypeID";
			case VONUserStartedTransmittingAudioEventTypeID: return "VONUserStartedTransmittingAudioEventTypeID";
			case VONUserStoppedTransmittingAudioEventTypeID: return "VONUserStoppedTransmittingAudioEventTypeID";
			case PartyChatStatusChangedEventTypeID: return "PartyChatStatusChangedEventTypeID";
			case DLCOwnerShipFailedEventTypeID: return "DLCOwnerShipFailedEventTypeID";
			case SetFreeCameraEventTypeID: return "SetFreeCameraEventTypeID";
			case ConnectingStartEventTypeID: return "ConnectingStartEventTypeID";
			case ConnectingAbortEventTypeID: return "ConnectingAbortEventTypeID";
			case PlayerDeathEventTypeID: return "PlayerDeathEventTypeID";
			case ReloadShadersEventTypeID: return "ReloadShadersEventTypeID";
			case LoadWorldProgressEventTypeID: return "LoadWorldProgressEventTypeID";
			case SignStatusEventTypeID: return "SignStatusEventTypeID";
			case SetPausedEventTypeID: return "SetPausedEventTypeID";
			case TerminationEventTypeID: return "TerminationEventTypeID";
			case UserSettingsChangedEventTypeID: return "UserSettingsChangedEventTypeID";
			case StorageChangedEventTypeID: return "StorageChangedEventTypeID";
			case BeforeResetEventTypeID: return "BeforeResetEventTypeID";
			//case AfterRenderEventTypeID: return "AfterRenderEventTypeID";
			case AfterResetEventTypeID: return "AfterResetEventTypeID";
			case CrashLogEventTypeID: return "CrashLogEventTypeID";
			case ConsoleEventTypeID: return "ConsoleEventTypeID";
		}
	
		return string.Empty;
	}
#endif
}

const EventType ReloadShadersEventTypeID;
const EventType LoadWorldProgressEventTypeID;
const EventType SignStatusEventTypeID;
const EventType SetPausedEventTypeID;
const EventType TerminationEventTypeID;
const EventType UserSettingsChangedEventTypeID;
const EventType StorageChangedEventTypeID;
const EventType BeforeResetEventTypeID;
const EventType AfterRenderEventTypeID;
const EventType AfterResetEventTypeID;
const EventType CrashLogEventTypeID;
const EventType ConsoleEventTypeID;