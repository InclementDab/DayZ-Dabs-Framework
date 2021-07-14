class SerializableBase
{
	void Write(Serializer serializer, int version);
	void Read(Serializer serializer, int version);
}