enum eDiagMenuRegisterType
{
	MENU, // regular menu
	MENU_CHILD, // child menu
	MENU_ITEM,
	MENU_BOOL,
	MENU_RANGE
}

class RegisterDiag: Class
{
	string Name;	
	eDiagMenuRegisterType Type;
		
	void RegisterDiag(string name)
	{
		Name = name;
	}
}

// Registers a root menu
class RegisterDiagMenu: RegisterDiag
{
	/// menu instances
	static ref array<RegisterDiagMenu> s_Instances = {};
	
	ref array<RegisterDiag> Children = {};
	void RegisterDiagMenu(string name, array<RegisterDiag> children)
	{
		Children = children;
		Type = eDiagMenuRegisterType.MENU;
		
		// child menus will not be iterated here
		foreach (auto diag_child: Children) {
			RegisterDiagMenu diag_menu_child = RegisterDiagMenu.Cast(diag_child);
			if (diag_menu_child) {
				diag_menu_child.Type = eDiagMenuRegisterType.MENU_CHILD;
			}
		}
				
		if (!s_Instances) {
			s_Instances = {};
		}
		
		s_Instances.Insert(this);
	}
	
	void ~RegisterDiagMenu()
	{
		if (Children) {
			for (int i = 0; i < Children.Count(); i++) {
				delete Children[i];
			}
		}
	}
}

class RegisterDiagMenuItems: RegisterDiag
{	
	string Items;
	string Shortcut;
	
	void RegisterDiagMenuItems(string name, string items, string shortcut = string.Empty)
	{
		Name = name;
		Items = items;
		Shortcut = shortcut;
		Type = eDiagMenuRegisterType.MENU_ITEM;
	}
}

class RegisterDiagMenuBool: RegisterDiag
{
	string Shortcut;
	bool Reverse;
	
	void RegisterDiagMenuBool(string name, bool reverse = false, string shortcut = string.Empty)
	{
		Reverse = reverse;
		Shortcut = shortcut;
		Type = eDiagMenuRegisterType.MENU_BOOL;
	}
}

class RegisterDiagMenuRange: RegisterDiag
{
	string Shortcut;
	string Items;
	
	void RegisterDiagMenuRange(string name, string items = string.Empty, string shortcut = string.Empty)
	{
		Items = items;
		Shortcut = shortcut;
		Type = eDiagMenuRegisterType.MENU_RANGE;
	}
}