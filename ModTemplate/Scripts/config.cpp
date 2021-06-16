
class CfgPatches
{
	class ModTemplate_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts"};
	};
};

class CfgMods 
{
	class DZ_ModTemplate
	{
		name = "Mod Template";
		dir = "ModTemplate";
		credits = "";
		author = "";
		creditsJson = "ModTemplate/Scripts/Data/Credits.json";
		versionPath = "ModTemplate/scripts/Data/Version.hpp";
		inputs = "ModTemplate/Scripts/Data/Inputs.xml";
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
					"ModTemplate/gui/imagesets/dayz_editor_gui.imageset"
				};
			};
			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"ModTemplate/scripts/common",
					"ModTemplate/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"ModTemplate/scripts/common",
					"ModTemplate/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"ModTemplate/scripts/common",
					"ModTemplate/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"ModTemplate/scripts/common",
					"ModTemplate/scripts/5_Mission"
				};
			};
		};
	};
};
