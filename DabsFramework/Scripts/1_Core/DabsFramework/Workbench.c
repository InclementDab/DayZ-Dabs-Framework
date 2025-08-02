#ifdef WORKBENCH
// cached data for build tools since workbench is very slow with batch files on win11
static ref map<string, string> s_RegistryEntries = new map<string, string>();
#endif