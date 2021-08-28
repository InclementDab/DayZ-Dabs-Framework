static vector ObjectGetSize(notnull Object object)
{
	vector size[2], result;
	object.ClippingInfo(size);
	for (int i = 0; i < 3; i++) {
		result[i] = Math.AbsFloat(size[0][i]) + Math.AbsFloat(size[1][i]);
	}
	
	return result;
}