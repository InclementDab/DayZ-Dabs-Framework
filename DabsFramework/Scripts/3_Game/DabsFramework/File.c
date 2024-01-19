class Directory: Managed
{
	static const string PROFILES = "$profiles:";
}

class FileStream: Managed
{
	
}

typedef string File;
class File: string
{	
	FileHandle CreateHandle(FileMode mode)
	{
		return OpenFile(value, mode);
	}
	
	static FileStream Open()
	{
		
	}
	
	static bool Exists(File file)
	{
		return FileExist(file);
	}
	
	static File Create(string file)
	{
		if (FileExist(file)) {
			DeleteFile(file);
		}
		
		CloseFile(OpenFile(file, FileMode.WRITE));
		return file;
	}
}