class VectorPrefabController: PrefabBaseController<vector>
{
	StringEvaluater X, Y, Z;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "X":
			case "Y":
			case "Z": {
				Value = string.Format("%1 %2 %3", X.Parse(), Y.Parse(), Z.Parse()).ToVector();
				super.PropertyChanged("Value");
				break;
			}
		}
	}
}