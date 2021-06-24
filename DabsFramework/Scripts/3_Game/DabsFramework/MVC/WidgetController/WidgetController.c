// The most basic abstraction, if you are inheriting from this, use WidgetControllerTemplate, this is all internal
class WidgetController
{
	bool CanTwoWayBind()
	{
		return false;
	}

	// Base Controller Stuff
	void Set(TypeConverter type_converter);
	void Get(out TypeConverter type_converter);

	// Collection Stuff
	void SetSelection(TypeConverter type_converter)
	{
		NotImplementedError("SetSelection");
	}

	void GetSelection(out TypeConverter type_converter)
	{
		NotImplementedError("GetSelection");
	}

	void Insert(TypeConverter type_converter)
	{
		NotImplementedError("Insert");
	}

	void InsertAt(int index, TypeConverter type_converter)
	{
		NotImplementedError("InsertAt");
	}

	void Remove(int index, TypeConverter type_converter)
	{
		NotImplementedError("Remove");
	}

	void Replace(int index, TypeConverter type_converter)
	{
		NotImplementedError("Replace");
	}

	void Move(int index, TypeConverter type_converter)
	{
		NotImplementedError("Move");
	}

	void Swap(int index_a, int index_b)
	{
		NotImplementedError("Swap");
	}

	void Clear()
	{
		NotImplementedError("Clear");
	}

	int Find(TypeConverter type_converter)
	{
		NotImplementedError("Find");
		return -1;
	}

	int Count()
	{
		NotImplementedError("Count");
		return -1;
	}

	private	void NotImplementedError(string function)
	{
		Error(string.Format("%1 does not support function %2", Type(), function));
	}
}