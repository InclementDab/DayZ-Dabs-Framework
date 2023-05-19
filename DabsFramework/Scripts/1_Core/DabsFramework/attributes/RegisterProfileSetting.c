class RegisterProfileSetting: Managed
{
	static ref array<ref Param2<typename, string>> Instances = {};
	
	void RegisterProfileSetting(typename type, string display_name = string.Empty)
	{
		if (!Instances) {
			Instances = {};
		}
						
		Instances.Insert(new Param2<typename, string>(type, display_name));
	}
}