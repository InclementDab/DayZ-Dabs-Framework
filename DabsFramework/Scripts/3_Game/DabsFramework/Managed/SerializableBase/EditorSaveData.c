class EditorSaveData: SerializableBase
{
	[NonSerialized()]
	static const string BIN_CHECK = "EditorBinned";
	
	static const int Version = 4;
	string MapName;
	vector CameraPosition;
	
	int Year, Month, Day, Hour, Minute, Second;
	float Overcast0, Fog0, Rain0;
	
	ref array<ref ObjectNodeData> ObjectNodes = {};
	ref array<ref EditorHiddenObjectData> EditorHiddenObjects = {};	
		
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(BIN_CHECK);
		serializer.Write(Version);
		serializer.Write(MapName);
		serializer.Write(CameraPosition);
		
		serializer.Write(ObjectNodes.Count());
		foreach (ObjectNodeData data: ObjectNodes) {
			data.Write(serializer, Version);
		}
		
		serializer.Write(EditorHiddenObjects.Count());
		foreach (EditorHiddenObjectData deleted_data: EditorHiddenObjects) {
			deleted_data.Write(serializer, Version);
		}
		
		serializer.Write(Year);
		serializer.Write(Month);
		serializer.Write(Day);
		serializer.Write(Hour);
		serializer.Write(Minute);
		serializer.Write(Second);
		serializer.Write(Overcast0);
		serializer.Write(Fog0);
		serializer.Write(Rain0);
	}

	bool Read(Serializer serializer)
	{
		return Read(serializer, 0);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		string bincheck;
		serializer.Read(bincheck);
		if (bincheck != BIN_CHECK) {
			return false;
		}
		
		serializer.Read(version);
		serializer.Read(MapName);
		serializer.Read(CameraPosition);
		
		int editor_object_count;
		serializer.Read(editor_object_count);
		for (int i = 0; i < editor_object_count; i++) {
			ObjectNodeData data();
			data.Read(serializer, version);
			ObjectNodes.Insert(data);
		}
		
		int editor_deleted_object_count;
		serializer.Read(editor_deleted_object_count);
		for (int j = 0; j < editor_deleted_object_count; j++) {
			EditorHiddenObjectData deleted_data();
			deleted_data.Read(serializer, version);
			EditorHiddenObjects.Insert(deleted_data);
		}
		
		if (version < 4) {
			return true;
		}
		
		serializer.Read(Year);
		serializer.Read(Month);
		serializer.Read(Day);
		serializer.Read(Hour);
		serializer.Read(Minute);
		serializer.Read(Second);
		serializer.Read(Overcast0);
		serializer.Read(Fog0);
		serializer.Read(Rain0);
		return true;
	}
	
	static bool IsBinnedFile(string file)
	{
		FileSerializer serializer();
		
		if (!serializer.Open(file)) {
			return false;
		}
		
		string bincheck;
		if (!serializer.Read(bincheck)) {
			serializer.Close();
			return false;
		}
		
		serializer.Close();
		
		return (bincheck == BIN_CHECK);
	}
}