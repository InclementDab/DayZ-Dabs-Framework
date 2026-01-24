//! How to use:
// 1. Create a config class inheriting from MissionSetting
// 2. Register it with the RegisterMissionSetting attribute. Wrap the type in a GenericWrapper1, and declare the file name relative to the mission folder.
// 3. (optional) Override IsSynchronized() to true if you wish to synchronize the information to clients.
//			You will need to overwrite Read() and Write() as well as shown below

// Final version should look something like this
/*

	[RegisterMissionSetting(new GenericWrapper1<SimpleCinematicsConfig>, "simple_cinematics.json")]
	class SimpleCinematicsConfig: MissionSetting
	{
		ref array<string> AvailableUsers = {};
		
		override void Write(Serializer serializer, int version)
		{
			super.Write(serializer, version);
	
			serializer.Write(AvailableUsers.Count());
			foreach (string user: AvailableUsers) {
				serializer.Write(user);
			}
		}
	
		override bool Read(Serializer serializer, int version)
		{
			if (!super.Read(serializer, version)) {
	            return false;
	        }
			
			int cnt;
			serializer.Read(cnt);
			for (int i = 0; i < cnt; i++) {
				string v;
				serializer.Read(v);
				AvailableUsers.Insert(v);
			}
	
	        
			return true;
		}
			
		override int GetVersion()
	    {
	        return 1;
	    }
		
		override bool IsSynchronized()
		{
			return true;
		}
	}

*/
class MissionSetting: SerializableBase
{
    const int RPC_SYNC = 239432;

	// Called on server when sync is saved
	static ref ScriptInvoker Event_OnSettingsSaved = new ScriptInvoker();

	// Called on client when sync is recieved from server
	// Called on server when sync is set dirty and re-synchronized
	static ref ScriptInvoker Event_OnSynchronized = new ScriptInvoker();

    int Version = GetVersion();

	static string FindSaveFile(typename mission_type)
	{
		string file_path = RegisterMissionSetting.s_RegisteredInstances[mission_type];
		if (!SystemPath.IsPathRooted(file_path)) {
			if (g_Game.IsDedicatedServer()) {
				file_path = SystemPath.Combine(SystemPath.Mission(), file_path);
			} else {
				file_path = SystemPath.Combine(SystemPath.Profile(), file_path);
			}
		}
		
		return file_path;
	}

	string FindSaveFile()
	{
		return MissionSetting.FindSaveFile(Type());
	}

    void Sync(PlayerIdentity identity = null)
    {
#ifdef SERVER
#ifdef DIAG_DEVELOPER
		PrintFormat("Synchronizing Mission Setting: %1, ver: %2, client_sync: %3", Type(), GetVersion(), IsSynchronized());
#endif
        ScriptRPC sync_rpc = new ScriptRPC();
        sync_rpc.Write(Type().ToString());
        Write(sync_rpc, Version);
        sync_rpc.Send(null, RPC_SYNC, true, identity);

        Event_OnSynchronized.Invoke(this, identity);
#endif
    }

	// Slow, only calls once on mission assign per instance
	GenericWrapper GenerateWrapperInstance()
	{		
		string wrapper_generator_code = string.Format("GenericWrapper GenerateWrapperInstance() { return new GenericWrapper1<%1>(); }", Type());
		string wrapper_generator_file = SystemPath.Profile("_generated.c");
		DeleteFile(wrapper_generator_file);
		FileHandle handle = OpenFile(wrapper_generator_file, FileMode.WRITE);
		FPrintln(handle, wrapper_generator_code);
		CloseFile(handle);
		
		ScriptModule module = ScriptModule.LoadScript(GetGame().GameScript, wrapper_generator_file, false);
		GenericWrapper wrapper;
		module.CallFunction(null, "GenerateWrapperInstance", wrapper, null);
		module.Release();

		return wrapper;
	}
		
    bool Save(bool sync_to_clients = false)
    {
        if (GetGame().IsMultiplayer() && !GetGame().IsDedicatedServer()) {
            return false;
        }

        string save_file = FindSaveFile();
        if (!save_file) {
            return false;
        }

		// This is a hack fix, since sometimes constructors arent called in typename.Spawn()? the Version text will be __useless__ otherwise
		Version = GetVersion();
		
        GenericWrapper data_class_wrapper = GenerateWrapperInstance();
        if (!data_class_wrapper) {
            ErrorEx("No data class wrapper");
            return false;
        }

        string file_save_string;
        g_Script.CallFunction(data_class_wrapper, "GetDataString", file_save_string, this);
		delete data_class_wrapper;
		
        if (!file_save_string || file_save_string.Contains("ERROR")) {
            ErrorEx(file_save_string);
            return false;
        }

        FileHandle file_handle = OpenFile(save_file, FileMode.WRITE);
        if (!file_handle) {
            ErrorEx(string.Format("No file handle created for %1, file: %2", Type(), save_file));
            return false;
        }

        FPrint(file_handle, file_save_string);
        CloseFile(file_handle);

        Event_OnSettingsSaved.Invoke(this);

        if (GetGame().IsMultiplayer() && sync_to_clients) {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Sync, 0, false, null);
        }

        return true;
    }

    int GetVersion()
    {
        return 0;
    }
		
	bool IsSynchronized()
	{
		return false;
	}

    override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);

        serializer.Write(Version);
	}

	override bool Read(Serializer serializer, int version)
	{
		if (!super.Read(serializer, version)) {
            return false;
        }

        serializer.Read(Version);
		return true;
	}

	private void MissionSetting();
}