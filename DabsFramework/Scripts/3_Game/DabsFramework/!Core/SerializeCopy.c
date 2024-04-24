class SerializeCopy<Class T>
{
	private void SerializeCopy();
	private void ~SerializeCopy();
	
	static T CreateCopy(notnull SerializableBase original, int version)
	{
		typename current_type = original.Type();
		SerializableBase serializable_base = current_type.Spawn();
		if (!serializable_base) {
			return null;
		}
		
		ScriptReadWriteContext context = new ScriptReadWriteContext();
		original.Write(context.GetWriteContext(), version);
		serializable_base.Read(context.GetReadContext(), version);
		
		T new_type;
		Class.CastTo(new_type, serializable_base);
		
		return new_type;
	}
}