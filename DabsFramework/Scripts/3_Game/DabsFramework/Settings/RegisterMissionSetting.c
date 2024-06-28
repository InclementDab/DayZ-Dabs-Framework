class GenericWrapper1<Class T>: GenericWrapper
{
    private T _;

    string GetDataString(T data)
    {
        string data_string, error_message;
        bool success = JsonFileLoader<T>.MakeData(data, data_string, error_message);
        if (!success) {
            return "ERROR: " + error_message; // idk, cursed
        }

        return data_string;
    }
}