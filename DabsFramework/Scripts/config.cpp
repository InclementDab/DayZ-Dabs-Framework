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
		creditsJson = "DabsFramework/Scripts/Credits.json";
		versionPath = "DabsFramework/Scripts/Version.hpp";
		type = "mod";
		defines[] = {"DF_VERSION_1_8_0"};
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
					"DabsFramework/gui/imagesets/prefabs.imageset",
					"DabsFramework/gui/icons/brands.imageset",
					"DabsFramework/gui/icons/light.imageset",
					"DabsFramework/gui/icons/regular.imageset",
					"DabsFramework/gui/icons/solid.imageset",
					"DabsFramework/gui/icons/thin.imageset"
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
					"DabsFramework/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"DabsFramework/scripts/5_Mission"
				};
			};
		};
	};
};
