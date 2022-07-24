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
	
	//				 EVRStorm
	//								0,  EVRStorm ptr
	//								1,  EVRStom ptr
	protected ref map<typename, ref EventMap> m_ActiveEvents = new map<typename, ref EventMap>();
	protected ref map<typename, int> m_AmountOfEventsRan = new map<typename, int>(); // amount of event type ran
	protected ref map<typename, float> m_PossibleEventTypes = new map<typename, float>();
	protected ref map<typename, float> m_EventCooldowns = new map<typename, float>();
		
	// deprecated
	static EventManager Start()
	{
		return GetInstance();
	}
	
	// deprecated
	static void Stop()
	{
	}
	
	static EventManager GetInstance()
	{
		return GetDayZGame().GetEventManager();
	}
	
	void EventManager()
	{		
		DayZGame.Event_OnRPC.Insert(OnRPC);
	}
	
	void ~EventManager()
	{
		delete m_ActiveEvents;
		delete m_PossibleEventTypes;
		delete m_EventCooldowns;
		
		if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM)) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ServerCooldownThread);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ServerEventThread);
		}
	}

	/*
		Run this in your init.c
	
		0 (int): Minimum time between events
		1 (int): Maximum time between events
		2 (int): Maximum amount of parallel events
	*/
	void Run(int min_between_events = 550, int max_between_events = 3500, int max_event_count = 2)
	{
		EventManagerLog.Info(this, "Initializing Event Manager");
		m_MaxEventCount = max_event_count;
		m_EventFreqMin = min_between_events;
		m_EventFreqMax = max_between_events;
				
		m_NextEventIn = GetNextEventTime();
		EventManagerLog.Info(this, "Next selection will occur in %1 seconds", m_NextEventIn.ToString());	
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ServerEventThread, m_NextEventIn * 1000);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ServerCooldownThread, 1000);
		
		EventManagerLog.Info(this, "EventManager is now running");
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
		EventManagerLog.Info(this, "Trying to select a new event...");												
		// Just a quick check to make sure we dont run the same event twice
		typename current_type = GetRandomEvent();
		while (current_type == m_LastEventType && m_PossibleEventTypes.Count() > 1) {
			EventManagerLog.Debug(this, "Random event selected was the same as last %1", current_type.ToString());
			current_type = GetRandomEvent();
		}
		
		m_LastEventType = current_type;
		
		//! Start new event
		StartEvent(current_type);
		
		m_NextEventIn = GetNextEventTime();
		EventManagerLog.Info(this, "Next selection will occur in %1 seconds", m_NextEventIn.ToString());
				
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ServerEventThread, m_NextEventIn * 1000);
	}
	
	void RegisterEvent(typename event_type, float frequency = 1.0)
	{
		if (frequency <= 0) {
			return;
		}
		
		m_PossibleEventTypes[event_type] = frequency;
	}
	
	// startup_params are passed to OnStart of the event
	// returns started event
	EventBase StartEvent(typename event_type, bool force = false, Param startup_params = null)
	{
		if (!GetGame().IsServer()) {
			EventManagerLog.Info(this, "StartEvent must be called on SERVER, exiting");
			return null;
		}
		
		// register the map associated with this event type, probably the first time running an event like this
		if (!m_ActiveEvents[event_type]) {
			m_ActiveEvents[event_type] = new EventMap();
		}
		
		int active_event_count;
		foreach (typename type, EventMap events: m_ActiveEvents) {
			if (!events) {
				continue;
			}
			
			active_event_count += events.Count();
		}
		
		if (active_event_count >= m_MaxEventCount && !force) {
			EventManagerLog.Info(this, "Could not start event as we reached the maximum event limit %1", m_MaxEventCount.ToString());
			return null;
		}
		
		if (m_EventCooldowns[event_type] > 0 && !force) {
			EventManagerLog.Info(this, "Could not start event %1 as it is on cooldown for %2 more seconds", event_type.ToString(), m_EventCooldowns[event_type].ToString());
			return null;
		}
						
		EventBase event_base = SpawnEvent(event_type);
		if (!event_base) {
			EventManagerLog.Info(this, "Failed to start event %1", event_type.ToString());
			return null;
		}
		
		// increment the amount of these events ran
		m_AmountOfEventsRan[event_type] = m_AmountOfEventsRan[event_type] + 1;
		
		// event_id is ALWAYS 0 when parallel events are disallowed
		int event_id = m_AmountOfEventsRan[event_type] * (event_base.MaxEventCount() > 1);
		if (m_ActiveEvents[event_type].Count() >= event_base.MaxEventCount()) {  // do not put force here, even FORCE wont allow multiple events to be run
			EventManagerLog.Info(this, "Could not start %1 as the max amount of events for this type has been achieved (%2)", event_type.ToString(), event_base.MaxEventCount().ToString());
			return null;
		}
						
		// check for disallowed evemts		
		foreach (typename etype, EventMap ebase: m_ActiveEvents) {
			if (event_base.GetDisallowedEvents().Find(etype) != -1 && !force) {
				EventManagerLog.Info(this, "Could not run event %1 because it conflicts with event %2...", event_type.ToString(), etype.ToString());
				return null;
			}
		}
		
		if (!event_base.EventActivateCondition() && !force) {
			EventManagerLog.Debug(this, "Could not run %1, failed ActivateCondition", event_type.ToString());
			return null;
		}
		
		// congrats, the event will now be run
		event_base.SetID(event_id);
		
		// assign the event to the map now that we know the id is valid
		m_ActiveEvents[event_type][event_id] = event_base;
		
		// Register event for cooldown
		m_EventCooldowns.Insert(event_type, event_base.GetEventCooldown());
		
		// start the event
		EventManagerLog.Info(this, "Starting event %1", event_type.ToString());
		event_base.OnStart(startup_params);
		return event_base;
	}
	
	bool CancelEvent(EventBase event_base)
	{
		if (!GetGame().IsServer()) {
			EventManagerLog.Info(this, "CancelEvent must be called on SERVER, exiting");
			return false;
		}
		
		// set to discard, its really this simple :)
		event_base.SwitchPhase(EventPhase.DELETE);
		return true;
	}
	
	void DeleteEvent(EventBase event_base)
	{
		EventManagerLog.Debug(this, "Deleting %1, idx: %2", event_base.Type().ToString(), event_base.GetID().ToString());
		if (m_ActiveEvents && m_ActiveEvents[event_base.Type()]) {
			m_ActiveEvents[event_base.Type()].Remove(event_base.GetID());
		}
	}
	
	// you only need to worry about event_id if you allow parralel events
	// returns if event was succesfully cancelled
	bool CancelEvent(typename event_type, int event_id = 0)
	{
		if (!GetGame().IsServer()) {
			EventManagerLog.Info(this, "CancelEvent must be called on SERVER, exiting");
			return false;
		}
				
		if (!m_ActiveEvents[event_type] || !m_ActiveEvents[event_type][event_id]) {
			EventManagerLog.Info(this, "Event %1 is not active", event_type.ToString());
			return false;
		}
		
		CancelEvent(m_ActiveEvents[event_type][event_id]);
		return true;
	}
		
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{	
		switch (rpc_type) {
			case ERPCsDabsFramework.EVENT_PAUSE: {
				EventPauseParams event_pause_params;
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
			
			case ERPCsDabsFramework.EVENT_UPDATE: {								
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
					
					EventManagerLog.Info(this, "Client received event manager update %1: %2", str_event_type, event_phase.ToString());										
										
					if (!m_ActiveEvents[event_type]) {
						m_ActiveEvents[event_type] = new EventMap();
					}
					
					// Case for JIP players	
					if (!m_ActiveEvents[event_type][event_id]) {
						m_ActiveEvents[event_type][event_id] = SpawnEvent(event_type);
						if (!m_ActiveEvents[event_type][event_id]) return;
					}						
					
					// Event finished
					if (event_phase == EventPhase.DELETE) {
						DeleteEvent(m_ActiveEvents[event_type][event_id]);
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
			
			case ERPCsDabsFramework.EVENT_FUNCTION: {
				if (GetGame().IsDedicatedServer()) {
					break;
				}
				
				// the variable naming sucks bc this enf sucks
				string str_event_type_fnc;
				if (!ctx.Read(str_event_type_fnc)) {
					break;
				}
				
				typename event_type_fnc = str_event_type_fnc.ToType();
				int event_id_fnc;
				if (!ctx.Read(event_id_fnc)) {
					break;
				}
				
				if (!m_ActiveEvents[event_type_fnc] || !m_ActiveEvents[event_type_fnc][event_id_fnc]) {
					EventManagerLog.Info(this, "Event not found when calling client function");
					break; // failed
				}
				
				string event_fnc_name;
				if (!ctx.Read(event_fnc_name)) {
					break;
				}
				
				string event_param_type_fnc;
				if (!ctx.Read(event_param_type_fnc)) {
					break;
				}
				
				SerializableParam client_fnc_param;
				if (event_param_type_fnc != "null") {
					client_fnc_param = SerializableParam.Cast(event_param_type_fnc.ToType().Spawn());
					client_fnc_param.Read(ctx);
				}
				
				g_Script.CallFunctionParams(m_ActiveEvents[event_type_fnc][event_id_fnc], event_fnc_name, null, client_fnc_param);				
				break;
			}
		}
		
		foreach (typename et, EventMap event_base_map: m_ActiveEvents) {
			foreach (int id, EventBase event_base: event_base_map) {
				if (event_base) {
					event_base.OnRPC(sender, target, rpc_type, ctx);
				}
			}
		}
	}
	
	bool CallFunctionOnClient(EventBase target, string function_name, SerializableParam params, PlayerIdentity identity = null)
	{
		EventManagerLog.Debug(target, "Calling %1 on clients", function_name);
		
		if (!GetGame().IsServer()) {
			return false;
		}
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(target.Type().ToString());
		rpc.Write(target.GetID());
		rpc.Write(function_name);	
		
		// handle data
		if (params) {
			rpc.Write(params.GetSerializeableType());
			params.Write(rpc);
		} else {
			rpc.Write("null");
		}
		
		rpc.Send(null, ERPCsDabsFramework.EVENT_FUNCTION, true, identity);
		return true;
	}
		
	void DispatchEventInfo(Man player)
	{
		EventManagerLog.Debug(this, "Sending In Progress info to %1", player.ToString());
		foreach (typename event_type, EventMap event_map: m_ActiveEvents) {
			foreach (int event_id, EventBase event_base: event_map) {
				if (!event_base) {
					continue;
				}
				
				event_base.SyncToClient(player.GetIdentity());
			}
		}
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
		if (!m_ActiveEvents[event_type]) {
			return null;
		}
		
		return m_ActiveEvents[event_type][event_id];
	}
	
	array<EventBase> GetEvents(typename event_type) 
	{
		if (!m_ActiveEvents[event_type]) {
			return {};
		}
		
		return m_ActiveEvents[event_type].GetValueArray();
	}
	
	array<EventBase> GetEventsInherited(typename event_type) 
	{		
		array<EventBase> active_events = {};
		foreach (typename event_checked_type, EventMap event_map: m_ActiveEvents) {
			if (!event_checked_type.IsInherited(event_type)) {
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
		foreach (typename event_type, EventMap event_map: m_ActiveEvents) {
			foreach (int event_id, EventBase event_base: event_map) {
				active_events.Insert(event_base);
			}
		}
		
		return active_events;
	}
	
	void DumpInfo()
	{
		array<EventBase> active_events = GetActiveEvents();
		
		EventManagerLog.Info(this, "There are %1 events running", active_events.Count().ToString());
		
		foreach (EventBase event_base: active_events) {						
			EventManagerLog.Info(this, "Event %1 is running in phase %2 with %3 seconds remaining", event_base.ToString(), event_base.GetCurrentPhase().ToString(), event_base.GetCurrentPhaseTimeRemaining().ToString());
		}
		
		if (m_PossibleEventTypes.Count() == 0 && GetGame().IsServer()) {
			EventManagerLog.Info(this, "Cannot debug Event Percentages with no events registered");
			return;
		}
		
		EventManagerLog.Info(this, "New events will be selected between %1 and %2 minutes", (m_EventFreqMin / 60).ToString(), (m_EventFreqMax / 60).ToString());
		EventManagerLog.Info(this, "There is a maximum event count of %1", m_MaxEventCount.ToString());
		float total_freq;
		foreach (typename typet, float freqt: m_PossibleEventTypes) {
			total_freq += freqt;
		}
		
		foreach (typename type, float freq: m_PossibleEventTypes) {
			float value = ((1 / total_freq) * freq) * 100;
			EventManagerLog.Info(this, "Chance of %1 is %2 percent", type.ToString(), value.ToString());
		}
	}
}