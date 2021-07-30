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
			return -1;
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
	    while (ch == 32) NextChar(pos, ch);
	    if (ch == char) {
	        NextChar(pos, ch);
	        return true;
	    }
	    return false;
	}
	
	protected float ParseExpression(out int pos, out int ch) 
	{
	    float x = ParseTerm(pos, ch);
	    while (!false) {
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
	    if (Eat("+", pos, ch)) return ParseFactor(pos, ch); // unary plus
	    if (Eat("-", pos, ch)) return -ParseFactor(pos, ch); // unary minus
				
	    float x;
	    int startPos = pos;
	    if (Eat("(", pos, ch)) { // parentheses
	        x = ParseExpression(pos, ch);
	        Eat(")", pos, ch);
	    } 
		
		// numbers
		else if (IsNumeric(ch)) { 
	        while (IsNumeric(ch)) {
				NextChar(pos, ch);
			}

	        x = (m_Value.Substring(startPos, pos - startPos)).ToFloat();
	    } 
		
		// functions
		else if (ch >= "a".Hash() && ch <= "z".Hash()) { 
	        while (ch >= "a".Hash() && ch <= "z".Hash()) NextChar(pos, ch);
	        string fnc = m_Value.Substring(startPos, pos - startPos);
	        x = ParseFactor(pos, ch);
	        if (fnc == "sqrt") x = Math.Sqrt(x);
	        else if (fnc == "sin") x = Math.Sin(x * Math.DEG2RAD);
	        else if (fnc == "cos") x = Math.Cos(x * Math.DEG2RAD);
	        else if (fnc == "tan") x = Math.Tan(x * Math.DEG2RAD);
	        else Error("Unknown function: " + fnc);
	    } else {
	        //Error("Unexpected: " + ch);
	    }
	
	    if (Eat("^", pos, ch)) x = Math.Pow(x, ParseFactor(pos, ch)); // exponentiation
	
	    return x;
	}
	
	static bool IsNumeric(string value)
	{
		return (value.Hash() >= "0".Hash() && value.Hash() <= "9".Hash()) || value.Hash() == ".".Hash();
	}
	
	static bool IsNumeric(int ch)
	{
		return (ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash();
	}
}


typedef string StringEvaluater;
class StringEvaluater: string
{	
	float Parse()
	{
		StringEvaluaterEvaluater evaluater = new StringEvaluaterEvaluater();
		return evaluater.Parse(value);
	}	
}