class CfgPatches
{
	class DF_Objects
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgVehicles
{
    class Static;
    class NetworkLightBase: Static
    {
        scope=2;
    };

    class NetworkSpotLight: NetworkLightBase {};
    class NetworkPointLight: NetworkLightBase {};
};