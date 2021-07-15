class reinterperet_cast<Class T, Class U>
{
	private void reinterperet_cast();
	private void ~reinterperet_cast();
	
	static T Run(U in)
	{
		U inarray[1];
		inarray[0] = in;
		
		T outarray[1];
		copyarray(outarray, inarray);
		return outarray[0];
	}
}