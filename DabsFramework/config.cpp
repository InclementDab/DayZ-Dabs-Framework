class CfgPatches
{
	class DF
	{
		requiredAddons[] = {};
	};
};

class CfgMods
{
    class DabsFramework
    {
        name="Dabs Framework";
        dir="DabsFramework";
        picture="DabsFramework\GUI\images\icon.edds";
        action="https://dab.dev";
        author="Tyler Paul (InclementDab)";
        overview = "Dabs Framework is a delightfully lightweight, feature packed addon that gives modders freedom and tools they need for success";
        defines[] = { "DABSFRAMEWORK_1_8_15" };

        class defs
		{
			class imageSets
			{
				files[]=
				{
					"DabsFramework/gui/imagesets/prefabs.imageset",
					"DabsFramework/gui/icons/brands.imageset",
					"DabsFramework/gui/icons/thin.imageset",
					"DabsFramework/gui/icons/light.imageset",
					"DabsFramework/gui/icons/regular.imageset",
					"DabsFramework/gui/icons/solid.imageset"
				};
			};
			class widgetStyles
			{
				files[]= { "DabsFramework/gui/looknfeel/prefabs.styles" };
			};

			class engineScriptModule 
			{ 
				files[] = { "DabsFramework/Scripts/1_Core"};
			};

			class gameScriptModule
			{
				files[] = { "DabsFramework/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				files[] = { "DabsFramework/Scripts/4_World" };
			};

			class missionScriptModule 
			{
				files[] = { "DabsFramework/Scripts/5_Mission" };
			};

			class workbenchScriptModule
			{
				files[] = { "DabsFramework/ToolAddons" };
			};
		};
    };
};