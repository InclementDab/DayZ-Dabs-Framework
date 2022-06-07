/*
*
*	EventManager.c
*
*	Primary Event Manager
*
*	You are free to mod this as you please. 
*	
*	That being said you are NOT free to redistribute / repack this into
*	your own mod. 
*
*/

class EventManager
{			
	// Enable / Disable the multiple event system
	protected int m_MaxEventCount, m_EventFreqMin, m_EventFreqMax;
	protected float m_NextEventIn;
	protected typename m_LastEventType;
	
	protected ref map<typename, float> m_PossibleEventTypes = new map<typename, float>();
	
	//				 EVRStorm
	//								0,  EVRStorm ptr
	//								1,  EVRStorm ptr
	protected ref map<typename, ref map<int, ref EventBase>> m_ActiveEvents = new map<typename, ref map<int, ref EventBase>>();

	protected ref Timer m_ServerEventTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref Timer m_EventCooldownTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref map<typename, int> m_AmountOfEventsRan = new map<typename, int>(); // amount of event type ran
	protected ref map<typename, float> m_EventCooldowns = new map<typename, float>();
	
	protected static ref EventManager m_Instance;
	
	static EventManager Start()
	{
		m_Instance = new EventManager();
		return m_Instance;
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static EventManager GetInstance()
	{
		return m_Instance;
	}
	
	void EventManager()
	{		
		DayZGame.Event_OnRPC.Insert(OnRPC);
	}
	
	void ~EventManager()
	{
		delete m_ActiveEvents;
		delete m_PossibleEventTypes;
		delete m_EventCooldownTimer;
		delete m_EventCooldowns;
		delete m_ServerEventTimer;
	}

	/*
		Run this in your init.c
	
		0 (int): Minimum time between events
		1 (int): Maximum time between events
		2 (int): Maximum amount of parallel events // DEPRICATED
	*/
	void Run(int min_between_events = 550, int max_between_events = 3500, int max_event_count = 2)
	{
		PrintToRPT("Initializing Event Manager");
		
		m_MaxEventCount = max_event_count;
		m_EventFreqMin = min_between_events;
		m_EventFreqMax = max_between_events;
		
		m_EventCooldownTimer.Run(1.0, this, "ServerCooldownThread", null, true);
		
		m_NextEventIn = GetNextEventTime();
		EventManagerInfo("Next selection will occur in %1 seconds", m_NextEventIn.ToString());	
		m_ServerEventTimer.Run(m_NextEventIn, this, "ServerEventThread");
		
		EventManagerInfo("EventManager is now running");
	}
	
	protected void ServerCooldownThread()
	{
		foreach (typename event_type, float event_cooldown: m_EventCooldowns) {
			m_EventCooldowns[event_type] = event_cooldown - 1.0;
			if (event_cooldown <= 0) {
				m_EventCooldowns.Remove(event_type);
			}
		}
	}
			
	protected void ServerEventThread()
	{	
		EventManagerInfo("Trying to select a new event...");												
		// Just a quick check to make sure we dont run the same event twice
		typename current_type = GetRandomEvent();
		while (current_type == m_LastEventType && m_PossibleEventTypes.Count() > 1) {
			EventManagerDebug("Random event selected was the same as last %1", current_type.ToString());
			current_type = GetRandomEvent();
		}
		
		m_LastEventType = current_type;
		
		//! Start new event
		StartEvent(current_type);
		
		m_NextEventIn = GetNextEventTime();
		EventManagerInfo("Next selection will occur in %1 seconds", m_NextEventIn.ToString());
		
		m_ServerEventTimer.Run(m_NextEventIn, this, "ServerEventThread");
	}
	
	void RegisterEvent(typename event_type, float frequency = 1.0)
	{
		if (frequency <= 0) {
			return;
		}
		
		m_PossibleEventTypes[event_type] = frequency;
	}
	
	// startup_params are passed to OnStart of the event
	// returns if the event started succesfully
	bool StartEvent(typename event_type, bool force = false, Param startup_params = null)
	{
		if (!GetGame().IsServer()) {
			EventManagerInfo("StartEvent must be called on SERVER, exiting");
			return false;
		}
		
		// register the map associated with this event type, probably the first time running an event like this
		if (!m_ActiveEvents[event_type]) {
			m_ActiveEvents[event_type] = new map<int, ref EventBase>();
		}
		
		int active_event_count;
		foreach (typename type, map<int, ref EventBase> events: m_ActiveEvents) {
			if (!events) {
				continue;
			}
			
			active_event_count += events.Count();
		}
		
		if (active_event_count >= m_MaxEventCount && !force) {
			EventManagerInfo("Could not start event as we reached the maximum event limit %1", m_MaxEventCount.ToString());
			return false;
		}
		
		if (m_EventCooldowns.Contains(event_type) && !force) {
			EventManagerInfo("Could not start event %1 as it is on cooldown for %2 more seconds", event_type.ToString(), m_EventCooldowns[event_type].ToString());
			return false;
		}
		
		// increment the amount of these events ran
		m_AmountOfEventsRan[event_type] = m_AmountOfEventsRan[event_type] + 1;
				
		EventManagerInfo("Starting event %1", event_type.ToString());
		EventBase event_base = SpawnEvent(event_type);
		if (!event_base) {
			EventManagerInfo("Failed to start event %1", event_type.ToString());
			return false;
		}
		
		// event_id is ALWAYS 0 when parallel events are disallowed
		int event_id = m_AmountOfEventsRan[event_type] * (event_base.MaxParallelEvents() > 0);
		
		if (m_ActiveEvents[event_type].Count() >= event_base.MaxParallelEvents()) {  // do not put force here, even FORCE wont allow multiple events to be run
			EventManagerInfo("Could not start %1 as the max amount of parallel events has been achieved (%2)", event_type.ToString(), event_base.MaxParallelEvents().ToString());
			delete event_base; // dont need to call delete here, its not ref'd yet
			return false;
		}
		
		event_base.SetID(event_id);
		
		// assign the event to the map now that we know the id is valid
		m_ActiveEvents[event_type][event_id] = event_base;
		
		// check for disallowed evemts		
		foreach (typename etype, map<int, ref EventBase> ebase: m_ActiveEvents) {
			if (event_base.GetDisallowedEvents().Find(etype) != -1 && !force) {
				EventManagerInfo("Could not run event %1 because it conflicts with event %2...", event_type.ToString(), etype.ToString());
				DeleteEvent(event_type, event_id);
				return false;
			}
		}
		
		if (!event_base.EventActivateCondition()) {
			DeleteEvent(event_type, event_id);
			return false;
		}
		
		// Register event for cooldown
		m_EventCooldowns.Insert(event_type, event_base.GetEventCooldown());
		
		// start the event
		event_base.OnStart(this, startup_params);
		return true;
	}
	
	// you only need to worry about event_id if you allow parralel events
	// returns if event was succesfully cancelled
	bool CancelEvent(typename event_type, int event_id = 0)
	{
		if (!GetGame().IsServer()) {
			EventManagerInfo("CancelEvent must be called on SERVER, exiting");
			return false;
		}
		
		if (!m_ActiveEvents[event_type]) {
			EventManagerInfo("Event %1 is not active", event_type.ToString());
			return false;
		}
		
		SendActiveEventData(event_type, event_id, EventPhase.DELETE, 0, false, null);
		DeleteEvent(event_type, event_id);
		return true;
	}
	
	void DeleteEvent(typename event_type, int event_id)
	{
		EventManagerDebug("Deleting %1, idx: %2", event_type.ToString(), event_id.ToString());
		if (!m_ActiveEvents || !m_ActiveEvents[event_type]) {
			return;
		}

		// delete just this specific event
		delete m_ActiveEvents[event_type][event_id];
		m_ActiveEvents[event_type].Remove(event_id);
	}
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{	
		switch (rpc_type) {
			case ERPCsDabsFramework.EVENT_MANAGER_SEND_PAUSE: {
				EventManagerPauseParams event_pause_params;
				if (!ctx.Read(event_pause_params)) {
					break;
				}
				
				typename eventp_type = event_pause_params.param1.ToType();
				bool eventp_paused = event_pause_params.param2;
				int eventp_id = event_pause_params.param3;
				
				if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
					// Forced setting for clients since this needs to be controlled separately
					// the client does not have authority to pause events directly, but we do
					EnScript.SetClassVar(m_ActiveEvents[eventp_type][eventp_id], "m_IsPaused", 0, eventp_paused);
				}
				
				break;
			}
			
			case ERPCsDabsFramework.EVENT_MANAGER_UPDATE: {								
				if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
					
					string str_event_type;
					if (!ctx.Read(str_event_type)) {
						break;
					}
					
					typename event_type = str_event_type.ToType();
					int event_id;
					if (!ctx.Read(event_id)) {
						break;
					}

					int event_phase;
					if (!ctx.Read(event_phase)) {
						break;
					}
					
					float event_phase_time;
					if (!ctx.Read(event_phase_time)) {
						break;
					}
					
					bool event_paused;
					if (!ctx.Read(event_paused)) {
						break;
					}
					
					// Set up serialized data
					// this parameter can be null so we check if the ctx reads it successfully
					string event_param_type;
					if (!ctx.Read(event_param_type)) {
						break;
					}
					
					Param client_param;
					if (event_param_type != "null") {
						SerializableParam serializeable_param = SerializableParam.Cast(event_param_type.ToType().Spawn());
						serializeable_param.Read(ctx);
						client_param = serializeable_param.ToParam();
					}
					
					EventManagerInfo("Client received event manager update %1: %2", str_event_type, event_phase.ToString());										
										
					if (!m_ActiveEvents[event_type]) {
						m_ActiveEvents[event_type] = new map<int, ref EventBase>();
					}
					
					// Case for JIP players	
					if (!m_ActiveEvents[event_type][event_id]) {
						m_ActiveEvents[event_type][event_id] = SpawnEvent(event_type);
						if (!m_ActiveEvents[event_type][event_id]) return;
					}						
					
					// Event finished
					if (event_phase == EventPhase.DELETE) {
						DeleteEvent(event_type, event_id);
						break;
					}
					
					// Play catch-up to the current phase
					if (m_ActiveEvents[event_type][event_id].JIPRunPreviousPhases()) {
						for (int i = m_ActiveEvents[event_type][event_id].GetCurrentPhase(); i < event_phase; i++) {
							m_ActiveEvents[event_type][event_id].SwitchPhase(i);
						}					
					}
					
					m_ActiveEvents[event_type][event_id].SwitchPhase(event_phase, event_phase_time, client_param);
					
					// Forced setting for clients since this needs to be controlled separately
					// the client does not have authority to pause events directly, but we do
					EnScript.SetClassVar(m_ActiveEvents[event_type][event_id], "m_IsPaused", 0, event_paused);
				}
				
				break;
			}				
		}
		
		foreach (typename et, map<int, ref EventBase> event_base_map: m_ActiveEvents) {
			foreach (int id, EventBase event_base: event_base_map) {
				if (event_base) {
					event_base.OnRPC(sender, target, rpc_type, ctx);
				}
			}
		}
	}
	
	void DispatchEventInfo(PlayerBase player)
	{
		EventManagerDebug("Sending In Progress info to %1", player.ToString());
		foreach (typename event_type, map<int, ref EventBase> event_map: m_ActiveEvents) {
			foreach (int event_id, EventBase event_base: event_map) {
				if (!event_base) {
					continue;
				}
				
				SerializableParam data = event_base.GetClientSyncData(event_base.GetCurrentPhase());	
				ScriptRPC rpc = new ScriptRPC();
				rpc.Write(event_type.ToString());
				rpc.Write(event_base.GetID());
				rpc.Write(event_base.GetCurrentPhase());
				rpc.Write(event_base.GetCurrentPhaseTimeRemaining());
				rpc.Write(event_base.IsPaused());
				if (data) {
					rpc.Write(data.GetSerializeableType());
					data.Write(rpc);
				} else {
					rpc.Write("null");
				}
				
				rpc.Send(player, ERPCsDabsFramework.EVENT_MANAGER_UPDATE, true, player.GetIdentity());
			}
		}
	}
	
	static void SendActiveEventData(typename event_type, int event_id, EventPhase phase_id, float time_remaining, bool is_paused, SerializableParam data)
	{
		EventManagerDebug("Sending active Event Data: %1, idx: %2, Phase: %3", event_type.ToString(), event_id.ToString(), typename.EnumToString(EventPhase, phase_id));
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(event_type.ToString());
		rpc.Write(event_id);
		rpc.Write(phase_id);
		rpc.Write(time_remaining);
		rpc.Write(is_paused);
		if (data) {
			rpc.Write(data.GetSerializeableType());
			data.Write(rpc);
		} else {
			rpc.Write("null");
		}
		
		rpc.Send(null, ERPCsDabsFramework.EVENT_MANAGER_UPDATE, true);
	}
	
	static void SendEventPauseData(typename event_type, int event_id, bool is_paused)
	{
		EventManagerDebug("Sending Event Pause Data: %1, idx: %2, Paused: %3", event_type.ToString(), event_id.ToString(), is_paused.ToString());
		GetGame().RPCSingleParam(null, ERPCsDabsFramework.EVENT_MANAGER_SEND_PAUSE, new EventManagerPauseParams(event_type.ToString(), is_paused, event_id), true, null);
	}
	
	bool IsEventActive(typename event_type)
	{
		return (m_ActiveEvents[event_type] && m_ActiveEvents[event_type].Count() > 0);
	}
	
	bool IsEventActive(typename event_type, int event_id)
	{
		return (m_ActiveEvents[event_type] && m_ActiveEvents[event_type][event_id]);
	}
	
	EventBase SpawnEvent(typename event_type)
	{
		if (!event_type.IsInherited(EventBase)) {
			return null;
		}
		
		return EventBase.Cast(event_type.Spawn());
	}
		
	typename GetRandomEvent()
	{
		if (m_PossibleEventTypes.Count() == 0) return typename;
		
		array<typename> possible_types = {};
		foreach (typename type, float freq: m_PossibleEventTypes) {
			for (int i = 0; i < freq * 100; i++) {
				possible_types.Insert(type);
			}
		}
				
		return possible_types.GetRandomElement();
	}
	
	float GetNextEventTime()
	{
		return Math.Clamp(Math.RandomInt(m_EventFreqMin, m_EventFreqMax), 10, int.MAX);
	}
	
	// again, you only need to worry about event_id if you allow parralel events
	EventBase GetEvent(typename event_type, int event_id = 0)
	{
		return m_ActiveEvents[event_type][event_id];
	}
	
	array<EventBase> GetEvents(typename event_type) 
	{
		if (!m_ActiveEvents[event_type]) {
			return {};
		}
		
		return m_ActiveEvents[event_type].GetValueArray();
		array<EventBase> active_events = {};
		
		foreach (typename event_checked_type, map<int, ref EventBase> event_map: m_ActiveEvents) {
			if (event_checked_type != event_type) {
				continue;
			}
			
			foreach (int event_id, EventBase event_base: event_map) {
				active_events.Insert(event_base);
			}
		}
		
		return active_events;
	}
	
	array<EventBase> GetActiveEvents()
	{
		array<EventBase> active_events = {};
		foreach (typename event_type, map<int, ref EventBase> event_map: m_ActiveEvents) {
			foreach (int event_id, EventBase event_base: event_map) {
				active_events.Insert(event_base);
			}
		}
		
		return active_events;
	}
	
	void DumpInfo()
	{
		if (m_PossibleEventTypes.Count() == 0) {
			EventManagerInfo("Cannot debug Event Percentages with no events registered");
			return;
		}
		
		EventManagerInfo("New events will be selected between %1 and %2 minutes", (m_EventFreqMin / 60).ToString(), (m_EventFreqMax / 60).ToString());
		EventManagerInfo("There is a maximum event count of %1", m_MaxEventCount.ToString());
		float total_freq;
		foreach (typename typet, float freqt: m_PossibleEventTypes) {
			total_freq += freqt;
		}
		
		foreach (typename type, float freq: m_PossibleEventTypes) {
			float value = ((1 / total_freq) * freq) * 100;
			EventManagerInfo("Chance of %1 is %2 percent", type.ToString(), value.ToString());
		}
		
		EventManagerInfo("There are %1 events running", m_ActiveEvents.Count().ToString());
		
		foreach (typename event_checked_type, map<int, ref EventBase> event_map: m_ActiveEvents) {			
			foreach (int event_id, EventBase event_base: event_map) {
				EventManagerInfo("Event %1 is running in phase %2 with %3 seconds remaining", event_base.ToString(), event_base.GetCurrentPhase().ToString(), event_base.GetCurrentPhaseTimeRemaining().ToString());
			}
		}
	}
			
	static void EventManagerDebug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG		
		PrintFormat("[DF][EventManager]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
#endif
	}
	
	static void EventManagerInfo(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("[DF][EventManager]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
}