class GenericWrapper: Managed
{
    typename GetDataType(int variable_index = 0)
    {
        return Type().GetVariableType(variable_index);
    }
}