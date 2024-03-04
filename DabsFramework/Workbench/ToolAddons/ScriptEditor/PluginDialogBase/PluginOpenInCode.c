[WorkbenchPluginAttribute("Open In Code", "Opens Current File in Code", "Ctrl+=", "", {"ScriptEditor"})]
class PluginOpenInCode: PluginDialogBase
{
	void PluginOpenInCode()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);		
		Workbench.RunCmd(string.Format("code %1", GetAbsolutePath(current_file)));
		
		Workbench.RunCmd("call sendkeys.bat \"code.exe\" \" \"");
	}
}