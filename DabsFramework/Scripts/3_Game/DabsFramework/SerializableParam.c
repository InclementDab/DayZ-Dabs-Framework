class SerializableParam: Managed
{
	string GetSerializeableType()
	{
		return Type().ToString();
	}
	
	bool Write(Serializer ctx)
	{
		return false;
	}
		
	bool Read(Serializer ctx)
	{
		return false;
	}
}

// Parameterless constructor is REQUIRED! due to using .spawn
class SerializableParam1<Class T1>: SerializableParam
{
	T1 param1;

	static SerializableParam7<T1> Create(T1 value1)
	{
		SerializableParam7<T1> param();
		param.param1 = value1;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1);
	}
}

class SerializableParam2<Class T1, Class T2>: SerializableParam
{
	T1 param1;
	T2 param2;

	static SerializableParam7<T1, T2> Create(T1 value1, T2 value2)
	{
		SerializableParam7<T1, T2> param();
		param.param1 = value1;
		param.param2 = value2;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2);
	}
}

class SerializableParam3<Class T1, Class T2, Class T3>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	
	static SerializableParam7<T1, T2, T3> Create(T1 value1, T2 value2, T3 value3)
	{
		SerializableParam7<T1, T2, T3> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3);
	}
}

class SerializableParam4<Class T1, Class T2, Class T3, Class T4>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	T4 param4;
	
	static SerializableParam7<T1, T2, T3, T4> Create(T1 value1, T2 value2, T3 value3, T4 value4)
	{
		SerializableParam7<T1, T2, T3, T4> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		param.param4 = value4;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4);
	}
}

class SerializableParam5<Class T1, Class T2, Class T3, Class T4, Class T5>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	T4 param4;
	T5 param5;
	
	static SerializableParam7<T1, T2, T3, T4, T5> Create(T1 value1, T2 value2, T3 value3, T4 value4, T5 value5)
	{
		SerializableParam7<T1, T2, T3, T4, T5> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		param.param4 = value4;
		param.param5 = value5;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5);
	}
}

class SerializableParam6<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	T4 param4;
	T5 param5;
	T6 param6;
	
	static SerializableParam7<T1, T2, T3, T4, T5, T6> Create(T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6)
	{
		SerializableParam7<T1, T2, T3, T4, T5, T6> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		param.param4 = value4;
		param.param5 = value5;
		param.param6 = value6;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6);
	}
}

class SerializableParam7<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	T4 param4;
	T5 param5;
	T6 param6;
	T7 param7;
	
	static SerializableParam7<T1, T2, T3, T4, T5, T6, T7> Create(T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7)
	{
		SerializableParam7<T1, T2, T3, T4, T5, T6, T7> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		param.param4 = value4;
		param.param5 = value5;
		param.param6 = value6;
		param.param7 = value7;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7);
	}
}

class SerializableParam8<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7, Class T8>: SerializableParam
{
	T1 param1;
	T2 param2;
	T3 param3;
	T4 param4;
	T5 param5;
	T6 param6;
	T7 param7;
	T8 param8;
	
	static SerializableParam8<T1, T2, T3, T4, T5, T6, T7, T8> Create(T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8)
	{
		SerializableParam8<T1, T2, T3, T4, T5, T6, T7, T8> param();
		param.param1 = value1;
		param.param2 = value2;
		param.param3 = value3;
		param.param4 = value4;
		param.param5 = value5;
		param.param6 = value6;
		param.param7 = value7;
		param.param8 = value8;
		return param;
	}
	
	override bool Write(Serializer ctx)
	{
		return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7) && ctx.Write(param8);
	}
		
	override bool Read(Serializer ctx)
	{
		return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7) && ctx.Read(param8);
	}
}