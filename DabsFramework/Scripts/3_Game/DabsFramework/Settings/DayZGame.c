modded class DayZGame
{
    protected ref array<string> m_GameIdentities = {};

	protected ref map<typename, ref MissionSetting> m_MissionSettings = new map<typename, ref MissionSetting>();
	
#ifndef SERVER
    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, target, rpc_type, ctx);

        switch (rpc_type) {
            case MissionSetting.RPC_SYNC: {
                string mission_setting_type;
                ctx.Read(mission_setting_type);
				
				Print(mission_setting_type);

                typename mission_setting_typename = mission_setting_type.ToType();
                if (!mission_setting_typename) {
                    ErrorEx(string.Format("INVALID MissionSetting type: %1", mission_setting_type));
                    break;
                }

                MissionSetting mission_setting = MissionSetting.Cast(mission_setting_typename.Spawn());
                if (!mission_setting.Read(ctx, mission_setting.GetVersion())) {
                    ErrorEx(string.Format("INVALID MissionSetting data: %1", mission_setting_type));
                    break;
                }
				
#ifdef DIAG_DEVELOPER
				PrintFormat("Registered Mission Setting %1", mission_setting_typename);
#endif

                m_MissionSettings[mission_setting_typename] = mission_setting;
                break;
            }
        }
    }
#endif

	override void OnEvent(EventType eventTypeId, Param params)
	{
        super.OnEvent(eventTypeId, params);

        switch (eventTypeId) {
            case ClientPrepareEventTypeID: {
                ClientPrepareEventParams client_prepare_event_params = ClientPrepareEventParams.Cast(params);
#ifdef DIAG_DEVELOPER
                PrintFormat("EOnClientPrepare: (identity=%1, use_db=%2, position=%3, yaw=%4, preload_timeout=%5)", client_prepare_event_params.param1, client_prepare_event_params.param2, client_prepare_event_params.param3, client_prepare_event_params.param4, client_prepare_event_params.param5);
#endif
                PlayerIdentity identity = client_prepare_event_params.param1;

                if (identity && m_GameIdentities.Find(identity.GetId()) == -1) {
                    m_GameIdentities.Insert(identity.GetId());
					
                    OnIdentityCreated(identity);
                }

                break;
            }
        }
    }

    protected void OnIdentityCreated(notnull PlayerIdentity identity)
    {
#ifdef DIAG_DEVELOPER
		PrintFormat("OnIdentityCreated %1", identity.GetId());
#endif
		
		// Update identity with mission settings
        foreach (MissionSetting mission_setting: m_MissionSettings) {
            if (mission_setting && mission_setting.IsSynchronized()) {
                mission_setting.Sync(identity);
            }
        }
    }

    protected void OnMissionPathSet(string path)
    {
        if (!GetGame().IsMultiplayer() || GetGame().IsDedicatedServer() && !path.Contains("intro") && !path.Contains("Cutscene")) {
            foreach (typename mission_setting_type, string mission_setting_file: RegisterMissionSetting.s_RegisteredInstances) {				
				string mission_setting_file_verified = mission_setting_file;
				if (!SystemPath.IsPathRooted(mission_setting_file_verified)) {
					if (GetGame().IsDedicatedServer()) {
						mission_setting_file_verified = SystemPath.Mission(mission_setting_file_verified);
					} else {
						mission_setting_file_verified = SystemPath.Profile(mission_setting_file_verified);
					}
				}
				
				// set extension if not present
				string extension = File.GetExtension(mission_setting_file_verified);
				if (extension == string.Empty) {
					mission_setting_file_verified += ".json";
				}
				
                bool file_exists = File.Exists(mission_setting_file_verified);
                MissionSetting mission_setting = MissionSetting.Cast(mission_setting_type.Spawn());
                if (!mission_setting) {
                    ErrorEx(string.Format("failed to create mission setting type: %1, file: %2", mission_setting_type, mission_setting_file_verified));
                    continue;
                }
				
				int code_mission_version = mission_setting.GetVersion();				
                if (!file_exists) {
                    bool save_success = mission_setting.Save();
                    if (!save_success) {
                        ErrorEx(string.Format("failed to save mission settings %2: %1", mission_setting_file_verified, mission_setting_type));
                    }
                } else {
                    string file_text = File.ReadAllText(mission_setting_file_verified);
                    if (!file_text) {
                        ErrorEx(string.Format("empty json file found"));
                        continue;
                    }
					
					// some type punning here, but enfusion shits itself when you dont wrap it this way
					string json_error;
					Managed managed_value = mission_setting;
					GenericWrapper wrapper = mission_setting.GenerateWrapperInstance();
					if (!wrapper) {
						PrintFormat("Invalid Wrapper Instance: %1", mission_setting_type);
						continue;
					}
					
					if (!wrapper.ReadFromJson(managed_value, file_text, json_error)) {
						PrintFormat("json error, file: %1, error: %2", mission_setting_file_verified, json_error);
                        continue;
					}
										
					mission_setting = MissionSetting.Cast(managed_value);
					if (!mission_setting) {
						ErrorEx(string.Format("mission setting error, cast back to MissionSetting failed %1", mission_setting_type));
                        continue;
					}
					
					int existing_mission_version = mission_setting.Version;
                    if (existing_mission_version != code_mission_version) {
                        PrintToRPT(string.Format("mission setting version mismatch %3: updating settings %1 => %2", existing_mission_version, code_mission_version, mission_setting_type));
                        bool resave_success = mission_setting.Save();
                        if (!resave_success) {
                            ErrorEx(string.Format("failed to re-save mission settings %2: %1", mission_setting_file_verified, mission_setting_type));
                        }
                    }
                }

#ifdef DIAG_DEVELOPER
				PrintFormat("Registered Mission Setting %1 to %2", mission_setting_type, mission_setting_file);
#endif
                m_MissionSettings[mission_setting_type] = mission_setting;
            }
        }		
    }

	override void SetMissionPath(string path)
	{
		super.SetMissionPath(path);

		OnMissionPathSet(path);
	}
	
	array<MissionSetting> GetAllMissionSettings()
	{
		return m_MissionSettings.GetValueArray();
	}
	
	MissionSetting GetMissionSetting(typename mission_settings)
	{
		return m_MissionSettings[mission_settings];
	}
}