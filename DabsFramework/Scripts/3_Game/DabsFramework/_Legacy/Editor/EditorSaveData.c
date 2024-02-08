class EditorSaveData
{
	[NonSerialized()]
	static const string BIN_CHECK = "EditorBinned";
	
	static const int Version = 3;
	string MapName;
	vector CameraPosition;
	
	ref array<ref EditorObjectData> EditorObjects = {};
	ref array<ref EditorDeletedObjectData> EditorDeletedObjects = {};	
		
	void ~EditorSaveData()
	{
		delete EditorObjects;
		delete EditorDeletedObjects;
	}
		
	void Write(Serializer serializer)
	{
		serializer.Write(BIN_CHECK);
		serializer.Write(Version);
		serializer.Write(MapName);
		serializer.Write(CameraPosition);
		
		serializer.Write(EditorObjects.Count());
		foreach (EditorObjectData data: EditorObjects) {
			data.Write(serializer, Version);
		}
		
		serializer.Write(EditorDeletedObjects.Count());
		foreach (EditorDeletedObjectData deleted_data: EditorDeletedObjects) {
			deleted_data.Write(serializer, Version);
		}
	}
	
	bool Read(Serializer serializer)
	{
		string bincheck;
		serializer.Read(bincheck);
		if (bincheck != BIN_CHECK) {
			return false;
		}
		
		int read_version;
		serializer.Read(read_version);
		serializer.Read(MapName);
		serializer.Read(CameraPosition);
		
		int editor_object_count;
		serializer.Read(editor_object_count);
		for (int i = 0; i < editor_object_count; i++) {
			EditorObjectData data();
			data.Read(serializer, read_version);
			EditorObjects.Insert(data);
		}
		
		int editor_deleted_object_count;
		serializer.Read(editor_deleted_object_count);
		for (int j = 0; j < editor_deleted_object_count; j++) {
			EditorDeletedObjectData deleted_data();
			deleted_data.Read(serializer, read_version);
			EditorDeletedObjects.Insert(deleted_data);
		}
		
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
	
	static EditorSaveData LoadFromFile(string file)
	{
		bool binned = IsBinnedFile(file);
		
		FileSerializer serializer = new FileSerializer();
		EditorSaveData save_data = new EditorSaveData();		
					
		if (binned) {	
			if (!serializer.Open(file, FileMode.READ)) {
				Error("Failed to open file " + file);
				return null;
			}
			
			if (!save_data.Read(serializer)) {
				Error("Failed to read file " + file);
				serializer.Close();
				return null;
			}
			
		} else {
			FileHandle fh = OpenFile(file, FileMode.READ);
			string jsonData;
			string error;
	
			if (!fh) {
				Error("Could not open file " + file);
				return null;
			}
		
			string line;
			while (FGets(fh, line) > 0) {
				jsonData = jsonData + "\n" + line;
			}
			
			CloseFile(fh);
	
			bool success = JsonSerializer().ReadFromString(save_data, jsonData, error);
			if (error != string.Empty || !success) {
				Error(error);
				return null;
			}
		}
		
		serializer.Close();		
		return save_data;
	}
}