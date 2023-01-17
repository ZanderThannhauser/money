
#include <time.h>
#include <stdlib.h>

#include <assert.h>

#include <debug.h>

#include <memory/srealloc.h>

#include <zebu.h>
#include <evaluate.h>

#include <cmdln/input_path.h>
#include <cmdln/verbose.h>

#include <scope/new.h>
#include <scope/assign.h>
#include <scope/free.h>

#include "process_input.h"

static void print_fraction(mpq_t x)
{
	putchar(' ');
	
	{
		char* num = NULL;
		
		gmp_asprintf(&num, "%Zu", mpq_numref(x));
		
		static const char* lookup[] = {
			"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹",
		};
		
		for (char* i = num; *i; i++)
			fputs(lookup[*i - '0'], stdout);
		
		free(num);
	}
	
	putchar('/');
	
	{
		char* den = NULL;
		
		gmp_asprintf(&den, "%Zu", mpq_denref(x));
		
		static const char* lookup[] = {
			"₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉"
		};
		
		for (char* i = den; *i; i++)
			fputs(lookup[*i - '0'], stdout);
		
		free(den);
	}
}

struct out process_input()
{
	ENTER;
	
	dpvs(input_path);
	
	FILE* stream = fopen(input_path, "r");
	
	if (!stream)
	{
		TODO;
		exit(1);
	}
	
	struct zebu_$start* start = zebu_parse(stream);
	
	struct scope* scope = new_scope();
	
	struct {
		struct transaction {
			time_t time;
			struct value delta;
		}* data;
		unsigned n, cap;
	} transactions = {};
	
	for (unsigned i = 0, n = start->statements.n; i < n; i++)
	{
		struct zebu_statement* statement = start->statements.data[i];
		
		if (statement->assignment)
		{
			struct zebu_assignment* assignment = statement->assignment;
			
			char* name = (char*) assignment->name->data;
			
			dpvs(name);
			
			struct value value = evaluate(assignment->expression, scope);
			
			scope_assign(scope, name, value);
			
			#ifdef VERBOSE
			if (verbose)
			{
				gmp_printf("on line %3u: %s = %+Qu\n",
					assignment->startline, name, value.dollar);
			}
			#endif
			
			mpq_clear(value.dollar);
		}
		else if (statement->transaction)
		{
			struct zebu_transaction* transaction = statement->transaction;
			
			struct tm tm = {};
			
			tm.tm_isdst = -1;
			
			if (transaction->year)
			{
				struct value x = evaluate_postfix(transaction->year, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_year = mpz_get_ui(mpq_numref(x.dollar)) - 1900;
				
				dpv(tm.tm_year);
				
				mpq_clear(x.dollar);
			}
			
			if (transaction->month)
			{
				struct value x = evaluate_postfix(transaction->month, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_mon = mpz_get_ui(mpq_numref(x.dollar)) - 1;
				
				dpv(tm.tm_mon);
				
				mpq_clear(x.dollar);
			}
			
			if (transaction->day)
			{
				struct value x = evaluate_postfix(transaction->day, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_mday = mpz_get_ui(mpq_numref(x.dollar));
				
				dpv(tm.tm_mday);
				
				mpq_clear(x.dollar);
			}
			else
			{
				tm.tm_mday = 1;
			}
			
			if (transaction->hour)
			{
				struct value x = evaluate_postfix(transaction->hour, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_hour = mpz_get_ui(mpq_numref(x.dollar));
				
				dpv(tm.tm_hour);
				
				mpq_clear(x.dollar);
			}
			
			if (transaction->minute)
			{
				struct value x = evaluate_postfix(transaction->minute, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_min = mpz_get_ui(mpq_numref(x.dollar));
				
				dpv(tm.tm_min);
				
				mpq_clear(x.dollar);
			}
			
			if (transaction->second)
			{
				struct value x = evaluate_postfix(transaction->second, scope);
				
				if (mpz_cmp_ui(mpq_denref(x.dollar), 1))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_sec = mpz_get_ui(mpq_numref(x.dollar));
				
				dpv(tm.tm_sec);
				
				mpq_clear(x.dollar);
			}
			
			time_t time = mktime(&tm);
			
			if (time == (time_t) -1)
			{
				TODO;
				exit(1);
			}
			
			dpv(time);
			
			struct value delta = evaluate(transaction->delta, scope);
			
			#ifdef VERBOSE
			if (verbose)
			{
				struct tm *tm = localtime(&time);
				
				char buffer[300];
				
				strftime(buffer, sizeof(buffer), "%Y/%m/%d@%H:%M:%S", tm);
				
				gmp_printf("on line %3u: at %s: %'Qu\n",
					transaction->startline, buffer, delta.dollar);
			}
			#endif
			
			if (transactions.n == transactions.cap)
			{
				transactions.cap = transactions.cap << 1 ?: 1;
				transactions.data = srealloc(transactions.data,
					sizeof(*transactions.data) * transactions.cap);
			}
			
			transactions.data[transactions.n++] = (struct transaction) {
				.time = time,
				.delta = delta,
			};
		}
		else
		{
			TODO;
		}
	}
	
	int cmp(const void* a, const void* b)
	{
		const struct transaction *A = a, *B = b;
		
		if (A->time > B->time)
			return +1;
		else if (A->time < B->time)
			return -1;
		else
			return +0;
	}
	
	#ifdef VERBOSE
	if (verbose)
	{
		printf("sorting transactions...\n");
	}
	#endif
	
	qsort(transactions.data, transactions.n, sizeof(*transactions.data), cmp);
	
	mpq_t sum;
	
	mpq_init(sum);
	
	struct out retval;
	retval.n = 2 * (transactions.n);
	retval.points = malloc(sizeof(*retval.points) * retval.n);
	
	unsigned I = 0;
	
	time_t now;
	
	time(&now);
	
	mpq_t hundred;
	
	mpq_init(hundred);
	
	mpq_set_ui(hundred, 100, 1);
	
	unsigned i, n;
	for (i = 0, n = transactions.n; i < n; i++)
	{
		struct transaction* transaction = &transactions.data[i];
		
		{
			retval.points[I].x = transaction->time - now;
			retval.points[I].y = mpq_get_d(sum);
			I++;
		}
		
		// print transaction:
		{
			{
				struct tm *tm = localtime(&transaction->time);
				
				char buffer[300];
				
				strftime(buffer, sizeof(buffer), "%Y/%m/%d@%H:%M:%S", tm);
				
				printf("%s: ", buffer);
			}
			
			int sgn = mpq_sgn(transaction->delta.dollar);
			
			mpq_t tmp;
			
			mpq_init(tmp);
			
			mpq_abs(tmp, transaction->delta.dollar);
			
			mpq_mul(tmp, tmp, hundred);
			
			{
				mpz_t dollar, cents;
				
				mpz_init(dollar), mpz_init(cents);
				
				mpz_fdiv_q(cents, mpq_numref(tmp), mpq_denref(tmp));
				
				mpz_fdiv_qr_ui(dollar, cents, cents, 100);
				
				gmp_printf("%c$%'5Zu.%02Zu", sgn < 0 ? '-' : '+', dollar, cents);
				
				mpz_clear(dollar), mpz_clear(cents);
			}
			
			mpz_fdiv_r(mpq_numref(tmp), mpq_numref(tmp), mpq_denref(tmp));
			
			if (mpq_sgn(tmp))
			{
				print_fraction(tmp);
			}
			
			puts("");
			
			mpq_clear(tmp);
		}
		
		mpq_add(sum, sum, transaction->delta.dollar);
		
		// print new sum:
		{
			int sgn = mpq_sgn(sum);
			
			mpq_t tmp;
			
			mpq_init(tmp);
			
			mpq_abs(tmp, sum);
			
			mpq_mul(tmp, tmp, hundred);
			
			printf("%40s", "");
			
			if (sgn < 0)
				printf("\e[31m");
			else if (mpq_cmp_ui(sum, 1000, 1) < 0)
				printf("\e[33m");
			else
				printf("\e[32m");
			
			{
				mpz_t dollar, cents;
			
				mpz_init(dollar), mpz_init(cents);
				
				mpz_fdiv_q(cents, mpq_numref(tmp), mpq_denref(tmp));
				
				mpz_fdiv_qr_ui(dollar, cents, cents, 100);
				
				gmp_printf("%c$%'5Zu.%02Zu", sgn < 0 ? '-' : '+', dollar, cents);
				
				mpz_clear(dollar), mpz_clear(cents);
			}
			
			mpz_fdiv_r(mpq_numref(tmp), mpq_numref(tmp), mpq_denref(tmp));
			
			if (mpq_sgn(tmp))
			{
				print_fraction(tmp);
			}
			
			puts("\e[0m");
			
			mpq_clear(tmp);
		}
		
		{
			retval.points[I].x = transaction->time - now;
			retval.points[I].y = mpq_get_d(sum);
			I++;
		}
	}
	
	while (transactions.n--)
	{
		mpq_clear(transactions.data[transactions.n].delta.dollar);
	}
	
	free_zebu_$start(start);
	
	free(transactions.data);
	
	mpq_clear(hundred);
	
	free_scope(scope);
	
	mpq_clear(sum);
	
	fclose(stream);
	
	EXIT;
	return retval;
}





