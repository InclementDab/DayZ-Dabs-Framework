class Reinterperet<Class T, Class U>
{
	private void Reinterperet();
	private void ~Reinterperet();
	
	static T Run(U in)
	{
		U inarray[1];
		inarray[0] = in;
		
		T outarray[1];
		copyarray(outarray, inarray);
		return outarray[0];
	}
}