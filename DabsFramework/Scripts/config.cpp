
class CfgPatches
{
	class DabsFramework_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts"};
	};
};

class CfgMods 
{
	class DZ_DabsFramework
	{
		name = "Mod Template";
		dir = "DabsFramework";
		credits = "";
		author = "";
		creditsJson = "DabsFramework/Scripts/Data/Credits.json";
		versionPath = "DabsFramework/scripts/Data/Version.hpp";
		inputs = "DabsFramework/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"DabsFramework/gui/imagesets/dayz_editor_gui.imageset"
				};
			};
			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"DabsFramework/scripts/common",
					"DabsFramework/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/common",
					"DabsFramework/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/common",
					"DabsFramework/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/common",
					"DabsFramework/scripts/5_Mission"
				};
			};
		};
	};
};
