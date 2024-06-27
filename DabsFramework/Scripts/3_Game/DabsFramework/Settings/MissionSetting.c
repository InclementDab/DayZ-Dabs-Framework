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
		return RegisterMissionSetting.s_RegisteredInstances[mission_type];
	}

	string FindSaveFile()
	{
		return MissionSetting.FindSaveFile(Type());
	}

    void Sync(PlayerIdentity identity = null)
    {
#ifdef SERVER
        ScriptRPC sync_rpc = new ScriptRPC();
        sync_rpc.Write(Type().ToString());
        Write(sync_rpc, Version);
        sync_rpc.Send(null, RPC_SYNC, true, identity);

        Event_OnSynchronized.Invoke(this, identity);
#endif
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

        string file_save_string = RegisterMissionSetting.DataToStringStatic(this);
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
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Sync);
        }

        return true;
    }

    int GetVersion()
    {
        return 0;
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
	//private void ~MissionSetting();
}