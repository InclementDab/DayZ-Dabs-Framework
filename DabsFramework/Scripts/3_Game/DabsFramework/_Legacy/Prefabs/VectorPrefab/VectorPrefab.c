class VectorPrefab: PrefabBase<vector>
{
	protected VectorPrefabController m_VectorPrefabController;
	
	void VectorPrefab(string caption, Class binding_context, string binding_name)
	{
		m_VectorPrefabController = VectorPrefabController.Cast(GetController());
		
		m_VectorPrefabController.X = m_PrefabBaseController.Value[0].ToString(false);
		m_VectorPrefabController.Y = m_PrefabBaseController.Value[1].ToString(false);
		m_VectorPrefabController.Z = m_PrefabBaseController.Value[2].ToString(false);
		m_VectorPrefabController.NotifyPropertyChanged("X", false);
		m_VectorPrefabController.NotifyPropertyChanged("Y", false);
		m_VectorPrefabController.NotifyPropertyChanged("Z", false);
	}
		
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/VectorPrefab.layout";
	}
	
	override typename GetControllerType() 
	{
		return VectorPrefabController;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel;
		motion *= 0.5;
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			// code sucks because it needs to be perfectly accurate, since floats get cut off at high numbers
			if (wheel == 1) { 
				motion = 0.1;
			} else {
				motion = -0.1;
			}
		}
				
		switch (w.GetName()) {
			
			case "x_value": {
				m_VectorPrefabController.X = (m_VectorPrefabController.X.Parse() + motion).ToString(false);
				m_VectorPrefabController.NotifyPropertyChanged("X");
				break;
			}
			
			case "y_value": {
				m_VectorPrefabController.Y = (m_VectorPrefabController.Y.Parse() + motion).ToString(false);
				m_VectorPrefabController.NotifyPropertyChanged("Y");
				break;
			}
			
			case "z_value": {
				m_VectorPrefabController.Z = (m_VectorPrefabController.Z.Parse() + motion).ToString(false);
				m_VectorPrefabController.NotifyPropertyChanged("Z");
				break;
			}
		}
		
		return true;
	}
}