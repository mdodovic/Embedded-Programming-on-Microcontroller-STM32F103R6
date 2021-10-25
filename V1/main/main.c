#define makro 5
int const nepromenjiva = 6;
int promenjiva = 7;
// int neinicijalizovana;
int __attribute__((common)) neinicijalizovana;

int main()
{
	neinicijalizovana = makro + nepromenjiva + promenjiva;
	
	return 0;
}