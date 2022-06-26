class SerializableBase
{
	void Write(Serializer serializer, int version);
	bool Read(Serializer serializer, int version)
	{
		return true;
	}
}