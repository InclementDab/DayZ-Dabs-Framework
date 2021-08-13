// temp until i can find a better way to find "First" in a map that doesnt blow the software up
static int EditorObjectID;
class EditorObjectData: SerializableBase
{	
	[NonSerialized()]
	int m_Id;
	int GetID() { return m_Id; }
	
	string Type;
	string DisplayName;
	vector Position;
	vector Orientation;
	float Scale = 1;
	
	[NonSerialized()]
	bool EditorOnly;
	
	[NonSerialized()]
	bool Locked;
	
	[NonSerialized()]
	bool AllowDamage;
	
	[NonSerialized()]
	bool Simulate;
	
	// todo: remove this nonserialized so this can save
	[NonSerialized()]
	ref array<string> Attachments = {};
	
	[NonSerialized()]
	string Model;
	
	[NonSerialized()]
	vector BottomCenter;

	int Flags;
	
	[NonSerialized()]
	ModStructure Mod;
	
	[NonSerialized()]
	string Icon;
	
	[NonSerialized()]
	Object WorldObject;
	
	[NonSerialized()]
	ref map<string, ref SerializableParam> Parameters = new map<string, ref SerializableParam>();
	
	void EditorObjectData() 
	{
		EditorObjectID++;
		m_Id = EditorObjectID;
		
		//Mod = LoadModData(Type, Path); todo idk
		//Icon = EditorPlaceableItem.GetIcon(Mod);
	}
	
	static EditorObjectData Create(string type, vector transform[4], EditorObjectFlags flags = EditorObjectFlags.ALL)
	{	
		return Create(type, transform[3], Math3D.MatrixToAngles(transform), 1, flags);
	}
	
	static EditorObjectData Create(string type, vector position, vector orientation, float scale, EditorObjectFlags flags)
	{				
		if (GetGame().GetModelName(type) == "UNKNOWN_P3D_FILE") {
			//EditorLog.Warning(string.Format("EditorObjectData::Create %1 is not a valid Object Type!", type));
			//return null;
		}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = type; 
		data.Model = GetGame().GetModelName(data.Type);
		data.Position = position; 
		data.Orientation = orientation;
		data.Scale = scale;
		data.Flags = flags;
		data.DisplayName = data.Type;
		//data.Mod = GetModFromObject(data.Type); todo refactor.

		//EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
				
		return data;
	}
	
	static EditorObjectData Create(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		// We do this because all 'baked' objects are ID'd to 3. cant store a bunch of 3's can we?
		// todo... actually we might be able to :)
		if (target.GetID() == 3) { 
			return null;
		}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = target.GetType();
		data.Model = GetGame().GetModelName(data.Type);
		data.WorldObject = target;
		data.Position = data.WorldObject.GetPosition(); 
		data.Orientation = data.WorldObject.GetOrientation(); 
		data.Scale = data.WorldObject.GetScale();
		data.Flags = flags;
		data.DisplayName = data.Type;
		
		//EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
		
		return data;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(DisplayName);
		serializer.Write(Position);
		serializer.Write(Orientation);
		serializer.Write(Scale);
		serializer.Write(Flags);
		
		if (version < 2) {
			return;
		}
		
		serializer.Write(Attachments.Count());
		for (int i = 0; i < Attachments.Count(); i++) {
			serializer.Write(Attachments[i]);
		}
		
		// Serialize parameters
		serializer.Write(Parameters.Count());
		for (int j = 0; j < Parameters.Count(); j++) {
			string key_at_index = Parameters.GetKey(j);
			serializer.Write(key_at_index);
			// write the type of the object that will need to be created
			serializer.Write(Parameters[key_at_index].GetSerializeableType());
			
			// write the data of the object
			Parameters[key_at_index].Write(serializer);
		}
		
		if (version < 3) {
			return;
		}
		
		serializer.Write(EditorOnly);
		serializer.Write(Locked);
		serializer.Write(AllowDamage);
		serializer.Write(Simulate);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		serializer.Read(DisplayName);
		serializer.Read(Position);
		serializer.Read(Orientation);
		serializer.Read(Scale);
		serializer.Read(Flags);
		
		if (version < 2) {
			return true;
		}
		
		int attachments_count;
		serializer.Read(attachments_count);
		for (int i = 0; i < attachments_count; i++) {
			string attachment;
			serializer.Read(attachment);
			Attachments.InsertAt(attachment, i);
		}
		
		int params_count;
		serializer.Read(params_count);
		for (int j = 0; j < params_count; j++) {
			string param_key;
			string param_type;
			serializer.Read(param_key);
			serializer.Read(param_type);
			if (!param_type.ToType()) {
				Error("Invalid Param Type in deserialization, this is corrupt data and will likely cause a crash");
				return false;
			}
			
			SerializableParam param_value = SerializableParam.Cast(param_type.ToType().Spawn());
			if (!param_value) {
				Error("Invalid Param Type in deserialization, this is corrupt data and will likely cause a crash");
				return false;
			}
			
			param_value.Read(serializer);
			Parameters[param_key] = param_value;
		}
		
		if (version < 3) {
			return true;
		}
		
		serializer.Read(EditorOnly);
		serializer.Read(Locked);
		serializer.Read(AllowDamage);
		serializer.Read(Simulate);
		
		return true;
	}
}
