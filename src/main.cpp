#include <iostream>
#include "zcc.h"

int main(int argc, char *argv[])
{
	Parser parser(*++argv);

	parser.trans_unit();

	//Lex lex(*++argv);
	//Token t;
	//do {
	//	t = lex.next();
	//	std::cout << t;
	//} while (t.getType() != K_EOF);

	return 0;
}