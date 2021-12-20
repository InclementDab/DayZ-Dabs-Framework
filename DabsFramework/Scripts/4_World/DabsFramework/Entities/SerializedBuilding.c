class SerializedBuilding: Building
{
	void Write(inout map<string, ref SerializableParam> serializable_data) {}
	void Read(map<string, ref SerializableParam> serializable_data)	{}
}