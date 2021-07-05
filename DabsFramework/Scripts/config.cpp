class CfgPatches
{
	class DF_Scripts
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
		name = "Dabs Framework";
		dir = "DabsFramework";
		credits = "InclementDab";
		author = "InclementDab";
		overview = "DF is a Lightweight framework designed to provide modders with the tools required to create new tools and mods for the community. Created by InclementDab";
		creditsJson = "DabsFramework/Scripts/Data/Credits.json";
		versionPath = "DabsFramework/Scripts/Data/Version.hpp";
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
					"DabsFramework/gui/imagesets/prefabs.imageset"
				};
			};

			class widgetStyles
			{
				files[]=
				{
					"DabsFramework/gui/looknfeel/prefabs.styles"
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
