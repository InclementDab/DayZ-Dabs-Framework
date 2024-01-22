// This typedef thing is a hack to get StringEvaluater to evaluate as strings, then you just call .Parse()
typedef string StringEvaluater;
class StringEvaluater: string
{	
	float Parse()
	{
		StringEvaluaterEvaluater evaluater = new StringEvaluaterEvaluater();
		return evaluater.Parse(value);
	}	
}

class StringEvaluaterEvaluater
{
	protected string m_Value;
	
	float Parse(string value)
	{
		m_Value = value;
		int pos = -1;
		int ch = 0;
		NextChar(pos, ch);
		float x = ParseExpression(pos, ch);
		if (pos < value.Length()) {
			Error("Unexpected: " + ch + " " + value);
			return 0;
		}
		
		return x;
	}
	
	protected void NextChar(out int pos, out int ch) 
	{
		pos++;
		if (pos < m_Value.Length()) {
			ch = m_Value[pos].Hash();
		} else {
			ch = -1;
		}
	}
	
	protected bool Eat(string char, out int pos, out int ch)
	{
		return Eat(char.Hash(), pos, ch);
	}
	
	protected bool Eat(int char, out int pos, out int ch) 
	{
		// Space
	    while (ch == 32) {
			NextChar(pos, ch);
		}
		
	    if (ch == char) {
	        NextChar(pos, ch);
	        return true;
	    }
		
	    return false;
	}
	
	protected float ParseExpression(out int pos, out int ch) 
	{
	    float x = ParseTerm(pos, ch);
	    while (true) {
	        if      (Eat("+".Hash(), pos, ch)) x += ParseTerm(pos, ch); // addition
	        else if (Eat("-".Hash(), pos, ch)) x -= ParseTerm(pos, ch); // subtraction
	        else return x;
	    }
		
		return x;
	}
	
	protected float ParseTerm(out int pos, out int ch) 
	{
	    float x = ParseFactor(pos, ch);
	    while (true) {
	        if      (Eat("*", pos, ch)) x *= ParseFactor(pos, ch); // multiplication
	        else if (Eat("/", pos, ch)) x /= ParseFactor(pos, ch); // division
	        else return x;
	    }
		
		return x;
	}
	
	protected float ParseFactor(out int pos, out int ch) 
	{
		// unary plus
	    if (Eat("+", pos, ch)) { 
			return ParseFactor(pos, ch); 
		}
		
		// unary minus
	    if (Eat("-", pos, ch)) { 
			return -ParseFactor(pos, ch); 
		}
		
	    float x;
	    int start_pos = pos;
		
		// parentheses
	    if (Eat("(", pos, ch)) { 
			// cant return because pos and ch are stored which is DUMB!
	        x = ParseExpression(pos, ch);
	        Eat(")", pos, ch);
	    } 
		
		// numbers
		else if (IsNumeric(ch)) { 
	        while (IsNumeric(ch)) {
				NextChar(pos, ch);
			}

	        x = (m_Value.Substring(start_pos, pos - start_pos)).ToFloat();
	    } 
		
		// functions
		else if (IsAlphabetical(ch)) { 
	        while (IsAlphabetical(ch)) { 
				NextChar(pos, ch);
			}
			
	        // Handle functions
			x = EvaluateFunction(m_Value.Substring(start_pos, pos - start_pos), ParseFactor(pos, ch));
	    }
	
		// exponentiation
	    if (Eat("^", pos, ch)) { 
			x = Math.Pow(x, ParseFactor(pos, ch)); 
		}
	
	    return x;
	}
	
	static float EvaluateFunction(string fnc, float value)
	{
		switch (fnc) {
			// todo: more functionality!
			case "sqrt": return Math.Sqrt(value);			
			case "sin": return Math.Sin(value);			
			case "cos": return Math.Cos(value);
			case "tan": return Math.Tan(value);
		}
		
		Error("Unknown Function: " + fnc);
		return 0;
	}
	
	// Helper funcs
	static bool IsNumeric(string ch)
	{
		return IsNumeric(ch.Hash());
	}
	
	static bool IsNumeric(int ch)
	{
		return (ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash();
	}
	
	static bool IsAlphabetical(string ch)
	{
		return IsAlphabetical(ch.Hash());
	}
	
	static bool IsAlphabetical(int ch)
	{
		return (ch >= "a".Hash() && ch <= "z".Hash());
	}
}