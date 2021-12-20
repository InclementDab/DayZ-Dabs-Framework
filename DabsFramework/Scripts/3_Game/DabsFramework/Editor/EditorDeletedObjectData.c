static int DeletedObjectId;

/* 
	For future reference, it is possible now with GetDebugName() to obtain the id and p3d of an object safely
	this would not be a terrible way to ensure deleted objects without a Type dont get improperly removed
	however we need to backport all old information and handle empty Types from the past
*/

class EditorDeletedObjectData: SerializableBase
{
	[NonSerialized()]
	int ID;
	
	string Type;
	vector Position;
	int Flags;
	
	[NonSerialized()]
	Object WorldObject;
	
	void EditorDeletedObjectData()
	{
		DeletedObjectId++;
		ID = DeletedObjectId;
	}
	
	int GetID()
	{
		return ID;
	}
	
	static EditorDeletedObjectData Create(string type, vector position)
	{		
		EditorDeletedObjectData data = new EditorDeletedObjectData();
		data.Type = type;
		data.Position = position;
		
		// todo handle multiple objects of the same name in a 1m radius
		data.WorldObject = data.FindObject();
		return data;
	}
	
	static EditorDeletedObjectData Create(Object object)
	{
		if (!object) {
			return null;
		}
		
		EditorDeletedObjectData data = new EditorDeletedObjectData();
		data.Type = object.GetType();
		data.Position = object.GetWorldPosition();
		data.WorldObject = object;
		return data;
	}
	
	Object FindObject(float radius = 0.1)
	{				
		array<Object> objects = {};
		GetGame().GetObjectsAtPosition3D(Position, radius, objects, null);
		
		foreach (Object object: objects) {
			string type;
			g_Game.ObjectGetType(object, type);
			if (type == Type) {
				return object;
			}
		}
		
		return null;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(Position);
		serializer.Write(Flags);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		serializer.Read(Position);
		serializer.Read(Flags);
		
		return true;
	}
}