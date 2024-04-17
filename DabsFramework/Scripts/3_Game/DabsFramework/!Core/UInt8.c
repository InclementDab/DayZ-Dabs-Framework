class uint8: int
{		
    static uint8 Convert(int value)
    {
        return (value & 255);
    }
}

typedef int uint8;