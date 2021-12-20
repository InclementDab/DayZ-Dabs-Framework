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
    class House;
    class NetworkLightBase: House
    {
        displayName = "NetworkLight";
        scope=2;
        model = "";
    };

    class NetworkSpotLight: NetworkLightBase {};
    class NetworkPointLight: NetworkLightBase {};

    class NetworkParticleBase: House
    {
        displayName = "NetworkParticle";
        scope=2;
        model = "";
    };
};