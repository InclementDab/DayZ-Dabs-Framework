class EditorSaveData
{
	int Version = 1;
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
		serializer.Read(Version);
		serializer.Read(MapName);
		serializer.Read(CameraPosition);
		
		int editor_object_count;
		serializer.Read(editor_object_count);
		for (int i = 0; i < editor_object_count; i++) {
			EditorObjectData data();
			data.Read(serializer, Version);
			EditorObjects.Insert(data);
		}
		
		int editor_deleted_object_count;
		serializer.Read(editor_deleted_object_count);
		for (int j = 0; j < editor_deleted_object_count; j++) {
			EditorDeletedObjectData deleted_data();
			deleted_data.Read(serializer, Version);
			EditorDeletedObjects.Insert(deleted_data);
		}
		
		return true;
	}
}