class FindWidget<Class T>
{
	private void FindWidget();
	private void ~FindWidget();
	
	static T SearchDown(notnull Widget parent, string name)
	{
		string parent_name = parent.GetName();
		array<string> name_split = {};
		parent_name.Split(".", name_split);
		if (name_split.Count() > 1) {
			parent_name = name_split[0];
		}
		
		T casted_type;
		if (!Class.CastTo(casted_type, parent.FindAnyWidget(string.Format("%1.%2", parent_name, name)))) {
			return null;
		}
		
		return casted_type;
	}
}