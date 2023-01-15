
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
				int sgn = mpz_sgn(value.cents);
				mpz_t abs, dollar, cents;
				mpz_init(abs), mpz_init(dollar), mpz_init(cents);
				mpz_abs(abs, value.cents);
				mpz_tdiv_qr_ui(dollar, cents, abs, 100);
				gmp_printf("on line %3u: %s = %s%'Zu.%02Zu\n",
					assignment->startline, name,
					sgn < 0 ? "-" : "", dollar, cents);
				mpz_clear(abs), mpz_clear(dollar), mpz_clear(cents);
			}
			#endif
			
			mpz_clear(value.cents);
		}
		else if (statement->transaction)
		{
			struct zebu_transaction* transaction = statement->transaction;
			
			struct tm tm = {};
			
			if (transaction->year)
			{
				struct value year = evaluate_postfix(transaction->year, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, year.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_year = mpz_get_ui(dollar) - 1900;
				
				dpv(tm.tm_year);
				
				mpz_clear(dollar), mpz_clear(year.cents);
			}
			
			if (transaction->month)
			{
				struct value month = evaluate_postfix(transaction->month, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, month.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_mon = mpz_get_ui(dollar) - 1;
				
				dpv(tm.tm_mon);
				
				mpz_clear(dollar), mpz_clear(month.cents);
			}
			
			if (transaction->day)
			{
				struct value day = evaluate_postfix(transaction->day, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, day.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_mday = mpz_get_ui(dollar);
				
				dpv(tm.tm_mday);
				
				mpz_clear(dollar), mpz_clear(day.cents);
			}
			else
			{
				tm.tm_mday = 1;
			}
			
			if (transaction->hour)
			{
				struct value hour = evaluate_postfix(transaction->hour, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, hour.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_hour = mpz_get_ui(dollar);
				
				dpv(tm.tm_hour);
				
				mpz_clear(dollar), mpz_clear(hour.cents);
			}
			
			if (transaction->minute)
			{
				struct value minute = evaluate_postfix(transaction->minute, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, minute.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_min = mpz_get_ui(dollar);
				
				dpv(tm.tm_min);
				
				mpz_clear(dollar), mpz_clear(minute.cents);
			}
			
			if (transaction->second)
			{
				struct value second = evaluate_postfix(transaction->second, scope);
				
				mpz_t dollar;
				
				mpz_init(dollar);
				
				mpz_fdiv_q_ui(dollar, second.cents, 100);
				
				if (!mpz_fits_ulong_p(dollar))
				{
					TODO;
					exit(1);
				}
				
				tm.tm_sec = mpz_get_ui(dollar);
				
				dpv(tm.tm_sec);
				
				mpz_clear(dollar), mpz_clear(second.cents);
			}
			
			struct value delta = evaluate(transaction->delta, scope);
			
			tm.tm_isdst = -1;
			
			time_t time = mktime(&tm);
			
			if (time == (time_t) -1)
			{
				TODO;
				exit(1);
			}
			
			dpv(time);
			
			#ifdef VERBOSE
			if (verbose)
			{
				struct tm *tm = localtime(&time);
				char buffer[300];
				strftime(
					buffer, sizeof(buffer), "%Y %b %d %a %I:%M:%S %p", tm);
				int sgn = mpz_sgn(delta.cents);
				mpz_t abs, dollar, cents;
				mpz_init(abs), mpz_init(dollar), mpz_init(cents);
				mpz_abs(abs, delta.cents);
				mpz_tdiv_qr_ui(dollar, cents, abs, 100);
				gmp_printf("on line %3u: at %s: %s$%'Zu.%02Zu\n",
					transaction->startline, buffer,
					sgn < 0 ? "-" : "", dollar, cents);
				mpz_clear(abs), mpz_clear(dollar), mpz_clear(cents);
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
	
	mpz_t sum;
	
	mpz_init_set_ui(sum, 0);
	
	struct out retval;
	retval.n = 2 * (transactions.n);
	retval.points = malloc(sizeof(*retval.points) * retval.n);
	
	unsigned I = 0;
	
	time_t now;
	
	time(&now);
	
	unsigned i, n;
	for (i = 0, n = transactions.n; i < n; i++)
	{
		struct transaction* transaction = &transactions.data[i];
		
		{
			retval.points[I].x = transaction->time - now;
			retval.points[I].y = mpz_get_d(sum);
			I++;
		}
		
		// print transaction:
		{
			struct tm *tm = localtime(&transaction->time);
			
			char buffer[300];
			
			strftime(buffer, sizeof(buffer), "%Y %b %d %a %I:%M:%S %p", tm);
			
			int sgn = mpz_sgn(transaction->delta.cents);

			mpz_t abs, dollar, cents;

			mpz_init(abs), mpz_init(dollar), mpz_init(cents);

			mpz_abs(abs, transaction->delta.cents);

			mpz_tdiv_qr_ui(dollar, cents, abs, 100);

			gmp_printf("%s: %c$%'5Zu.%02Zu\n", buffer, sgn < 0 ? '-' : '+', dollar, cents);
			
			mpz_clear(abs), mpz_clear(dollar), mpz_clear(cents);
		}
		
		mpz_add(sum, sum, transaction->delta.cents);
		
		// print new sum:
		{
			int sgn = mpz_sgn(sum);
			
			mpz_t abs, dollar, cents;
			
			mpz_init(abs), mpz_init(dollar), mpz_init(cents);
			
			mpz_abs(abs, sum);
			
			mpz_tdiv_qr_ui(dollar, cents, abs, 100);
			
			if (sgn < 0)
			{
				gmp_printf("%40s\e[31m($%'5Zu.%02Zu)\e[0m\n", "", dollar, cents);
			}
			else if (!sgn)
			{
				gmp_printf("%40s \e[33m$%'5Zu.%02Zu\e[0m\n", "", dollar, cents);
			}
			else
			{
				gmp_printf("%40s \e[32m$%'5Zu.%02Zu\e[0m\n", "", dollar, cents);
			}
			
			mpz_clear(abs), mpz_clear(dollar), mpz_clear(cents);
		}
		
		{
			retval.points[I].x = transaction->time - now;
			retval.points[I].y = mpz_get_d(sum);
			I++;
		}
	}
	
	while (transactions.n--)
		mpz_clear(transactions.data[transactions.n].delta.cents);
	
	free_zebu_$start(start);
	
	free(transactions.data);
	
	free_scope(scope);
	
	mpz_clear(sum);
	
	fclose(stream);
	
	EXIT;
	return retval;
}





