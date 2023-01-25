/*
	Very Simple Calculator
	$Id$
 */

#include "s-compile.h"

int variable[48];

static int expr();
static int aexpr();
static int mexpr();
static int term();

static int
expr()
{
	int d, assign;

	d = aexpr();
	assign = lvalue;
	while (last_token != EOF)
	{
		switch (last_token)
		{
		case '>':
			d = (d > aexpr());
			break;
		case '<':
			d = (d < aexpr());
			break;
		case '=':
			if (assign >= 0)
			{
				d = expr();
				variable[assign] = d;
				break;
			}
			else
			{
				error("Bad assignment");
				break;
			}
		case ')':
			return d;
		case 'r':				// right shift (>>)
			d = (d >> aexpr());
			break;
		case 'l':				// left shift (<<)
			d = (d << aexpr());
			break;
		case 'e':				// == operation
			d = (d==expr());
			break;
		default:
			error("Bad expression");
			return d;
		}
	}
	return d;
}

static int
aexpr()
{
	int d;

	d = mexpr();
	while (last_token != EOF)
	{
		switch (last_token)
		{
		case '-':
			d -= mexpr();
			break;
		case '+':
			d += mexpr();
			break;
		case '|':
			d |= mexpr();
			break;
		default:
			return d;
		}
	}
	return d;
}

static int
mexpr()
{
	int d;

	d = term();
	while (last_token != EOF)
	{
		switch (last_token)
		{
		case '*':
			d *= term();
			break;
		case '/':
			d /= term();
			break;
		case '&':
			d &= term();
			break;
		default:
			return d;
		}
	}
	return d;
}

static int
term()
{
	int d;

	lvalue = -1;
	token();
	if (last_token == EOF)
	{
		error("Term expected");
	}
	switch (last_token)
	{
	case '0':
		d = value;
		token();
		return d;
	case 'v':
		d = lvalue = value;
		token();
		return variable[d];
	case '(':
		d = expr();
		if (last_token != ')')
		{
			error("Unbalanced parenthsis");
		}
		token();
		return d;
	/* -1や-a */
	case '-':
		d = -1*term();
		return d;
	//case '>':
	//	return d;
	//case '<':
	//	return d;
	default:
		token();
		error("Unknown term");
		return 0;
	}
}

int main()
{
	int d;
	char buf[BUFSIZ];

	while (fgets(buf, BUFSIZ, stdin))
	{
		ptr = buf;
		d = expr();
		printf("%s = 0x%08x = %d\n", buf, d, d);
		fflush(stdout);
	}
	return 0;
}

/* end */
