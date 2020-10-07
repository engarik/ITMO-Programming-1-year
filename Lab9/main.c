/* Kornilov Nikita, M3102, 7.10.2020, lab 9, var. 8 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int i, n, months;
	double balance, rate, change;
	char *string;
	
	printf("Please input digit: ");
	scanf("%1d", &n);
	
	switch (n) {
		case 0: {
			string = "Ноль";
			break;
		}
		case 1: {
			string = "Один";
			break;
		}
		case 2: {
			string = "Два";
			break;
		}
		case 3: {
			string = "Три";
			break;
		}
		case 4: {
			string = "Четыре";
			break;
		}
		case 5: {
			string = "Пять";
			break;
		}
		case 6: {
			string = "Шесть";
			break;
		}
		case 7: {
			string = "Семь";
			break;
		}
		case 8: {
			string = "Восемь";
			break;
		}
		case 9: {
			string = "Девять";
			break;
		}
	}
	printf("%i is %s\n", n, string);
	
	printf("Please input bank account balance: ");
	scanf("%lf", &balance);
	printf("Please input year interest rate(in %%): ");
	scanf("%lf", &rate);
	printf("How many months?: ");
	scanf("%i", &months);
	
	change = balance;
	for (i = 0; i <= months; i++) {
		printf("Month:%2i Balance:%.2f Change:%.2f\n", i, balance, balance - change);
		change = balance;
		balance += balance * rate / 1200.0;
	}
	
	return 0;
}
