class EditorObjectParam: Managed
{
	string GetSerializeableType()
	{
		return Type().ToString();
	}
	
	bool Serialize(Serializer ctx)
	{
		return false;
	}
		
	bool Deserialize(Serializer ctx)
	{
		return false;
	}
}

// Parameterless constructor is REQUIRED! due to using .spawn
class EditorObjectParam1<Class T1>: EditorObjectParam
{
	T1 param1;
	
	static EditorObjectParam1<T1> Create(T1 value)
	{
		EditorObjectParam1<T1> param();
		param.param1 = value;
		return param;
	}
	
	override bool Serialize(Serializer ctx)
	{
		return ctx.Write(param1);
	}
		
	override bool Deserialize(Serializer ctx)
	{
		return ctx.Read(param1);
	}
}