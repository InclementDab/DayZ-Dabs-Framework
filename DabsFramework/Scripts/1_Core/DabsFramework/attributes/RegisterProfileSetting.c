class RegisterProfileSetting: Managed
{
	static ref array<typename> Instances = {};
	
	void RegisterProfileSetting(typename type)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(type);
	}
}