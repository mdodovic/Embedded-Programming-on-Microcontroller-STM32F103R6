# 1 ".\\main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 ".\\main.c"

// uklonjene su sve makrodefinicije i svi komentari

int const nepromenjiva = 6;
int promenjiva = 7;

int __attribute__((common)) neinicijalizovana;

int main()
{
 neinicijalizovana = 5 + nepromenjiva + promenjiva; 
 // izvršena je makrozamena makroa "makro" sa njegovom vrednošću 5

 return 0;
}
