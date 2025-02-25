// Invoke the attribute wherever, you can call useful functions like DumpStackString() to find
// 		where you are in the program. from here it is possible to create reflection
//[AttributeSample()]

// Heres a sample attribute class, NOTE: MUST be Class type!!
class AttributeSample: Class
{
	// The constructor will be called at module load time.
	//	for instance this will be called when the mission is created, shortly after server join
	// 	it will run on module load time, making it impossible to circumvent and/or shut off.
	// and since its the constructor, its unmoddable! 
	void AttributeSample()
	{
		// and heres a simple script to blacklist a server you dont particularly like.
		// not that the server is bad or anything, but the racist owner should go back to rust.
#ifndef SERVER
		string tokens[256]; 
		string addy; 
		int port; 
		for (int i = 0; GetDayZGame().GetHostAddress(addy, port) && i < GetDayZGame().GetHostName().ParseString(tokens); i++) {
			string token_lower = tokens[i];
			token_lower.ToLower();
			if (token_lower.Hash() == -963439320) {
				ErrorModuleHandler.ThrowError(ErrorCategory.ClientKicked, -1); 
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetDayZGame().DisconnectSessionScript, 0, 0, 1);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetDayZGame().DisconnectSessionForce, 0, 0);
			}
		}
#endif
		
		// hope this tutorial was helpful. if you got some joy out of it, hit my patreon up!
		// https://www.patreon.com/c/inclementdab
	}
}