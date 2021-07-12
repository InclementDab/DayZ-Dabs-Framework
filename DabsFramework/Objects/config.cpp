class CfgPatches
{
	class DF_Objects
	{
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgVehicles
{
    class HouseNoDestruct;
	class thunderboltnorm: HouseNoDestruct
	{
        scope = 2;
		model = "\dz\data\data\blesk1.p3d";
	};
    
	class thunderboltheavy: HouseNoDestruct
	{
        scope = 2;
		model = "\dz\data\data\blesk2.p3d";
	};
};