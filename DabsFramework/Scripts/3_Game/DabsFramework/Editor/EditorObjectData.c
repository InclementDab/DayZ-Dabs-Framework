class EditorObjectData: SerializableBase
{	
	//@ Corresponds to the spawnable typename, identical to ITEM_SpawnerObject.name
	string Uuid;
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

	EditorObjectFlags Flags;
	
	[NonSerialized()]
	ModStructure Mod;
	
	[NonSerialized()]
	string Icon;
	
	[NonSerialized()]
	Object WorldObject;
	
	[NonSerialized()]
	ref map<string, ref SerializableParam> Parameters = new map<string, ref SerializableParam>();
		
	Object CreateObject(int flags = ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY)
	{
		if (Type.Contains("\\") || Type.Contains("/")) {
			return GetGame().CreateStaticObjectUsingP3D(Type, Position, Orientation, Scale);
		}
		
		Object object = GetGame().CreateObjectEx(Type, Position, flags);
		object.SetOrientation(Orientation);
		object.SetScale(Scale);
		return object;
	}
	
	static EditorObjectData Create(Serializer serializer)
	{
		EditorObjectData data = new EditorObjectData();
		data.Read(serializer, 0);
		return data;
	}
	
	static EditorObjectData Create(string type, vector transform[4], EditorObjectFlags flags = EFE_DEFAULT)
	{	
		return Create(type, transform[3], Math3D.MatrixToAngles(transform), 1, flags);
	}
	
	static EditorObjectData Create(string type, vector position, vector orientation, float scale, EditorObjectFlags flags)
	{				
		//if (GetGame().GetModelName(type) == "UNKNOWN_P3D_FILE") {
			//EditorLog.Warning(string.Format("EditorObjectData::Create %1 is not a valid Object Type!", type));
			//return null;
		//}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = type; 
		data.Model = GetModelName(data.Type);
		data.Position = position; 
		data.Orientation = orientation;
		data.Scale = scale;
		data.Flags = flags;
		data.DisplayName = data.Type;
		//data.Mod = GetModFromObject(data.Type); todo refactor.

		//EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
				
		return data;
	}
	
	static EditorObjectData Create(notnull Object target, EditorObjectFlags flags = EFE_DEFAULT)
	{
		// We do this because all 'baked' objects are ID'd to 3. cant store a bunch of 3's can we?
		// todo... actually we might be able to :)
		if (target.GetID() == 3) { 
			return null;
		}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = target.GetType();
		data.Model = GetModelName(data.Type);
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
	
	static string GetModelName(string class_name)
	{
		if (class_name == string.Empty) {
			return "UNKNOWN_P3D_FILE";
		}
		

		string model_path;
		if (!GetGame().ConfigGetText("CfgVehicles " + class_name + " model", model_path)) {
			return "UNKNOWN_P3D_FILE";
		}
		
		int to_substring_end = model_path.Length() - 4; // -4 to leave out the '.p3d' suffix
		if (to_substring_end < 0) {
			return "UNKNOWN_P3D_FILE";
		}
		
		int to_substring_start = 0;
		
		// Currently we have model path. To get the name out of it we need to parse this string from the end and stop at the first found '\' sign
		for (int i = to_substring_end; i > 0; i--) {
			if (model_path[i] == "\\") {
				to_substring_start = i + 1;
			}
		}
		
		return model_path.Substring(to_substring_start, to_substring_end - to_substring_start);
	}
}
