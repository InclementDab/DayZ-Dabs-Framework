// .gproject data
class ProjectSettings: Managed
{	
	ref array<string> Data = {};
	ref map<string, string> Tokens = new map<string, string>();
	ref map<string, ref ProjectSettings> Classes = new map<string, ref ProjectSettings>();
	
	protected ProjectSettings m_Parent;
	
	void ProjectSettings(ProjectSettings parent = null)
	{
		m_Parent = parent;
	}
	
	string Get(string key)
	{
		return Tokens[key];
	}
	
	void Set(string key, string value)
	{
		Tokens[key] = value;
	}
		
	ProjectSettings GetParent()
	{
		return m_Parent;
	}
		
	static ProjectSettings Load(string file)
	{
		string line_content;
		array<string> lines = {};
 		FileHandle file_handle = OpenFile(file, FileMode.READ);
		while (FGets(file_handle, line_content) >= 0) {
			lines.Insert(line_content);
		}
				
		CloseFile(file_handle);	
		
		return Load(lines);
	}
		
	static ProjectSettings Load(inout array<string> lines, ProjectSettings parent = null)
	{				
		ProjectSettings settings = new ProjectSettings(parent);
		
		while (lines.Count() > 0) {
			string token;
			string line = lines[0];
			lines.RemoveOrdered(0);			
			
			int token_id = line.ParseStringEx(token);
			string key;
			while (token_id) {
				
				switch (token_id) {
					case 1: {						
						if (token == "{") {
							while (settings.Classes[key]) {
								key += ".";
							}				
											
							settings.Classes[key] = Load(lines, settings);
						}
							
						if (token == "}") {
							return settings;
						}
						
						break;
					}
					
					// Quoted String
					case 2: {
						if (key != string.Empty) {
							settings[key] = token;
						} else {
							settings.Data.Insert(token);
						}
							
						break;
					}
					
					case 3: {
	
						key = token;
						
						break;
					}
				}
				
				token_id = line.ParseStringEx(token);
			}
		}
		
		return settings;
	}
}