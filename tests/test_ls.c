#include "../include/header.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(my_test, simple_test, .init = redirect_all_std)
{

}
