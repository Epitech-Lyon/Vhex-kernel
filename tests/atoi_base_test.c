#include <criterion/criterion.h>

// Internal prototypes.
extern uint32_t atoi_base(char const *str, int base);

Test(atoi_base, base_10)
{
	uint32_t result;

	result = atoi_base("12345", 10);
	cr_assert_eq(result, 12345, "returned: %d\n", result);
	cr_assert_eq(atoi_base("50926", 10), 50926);
	cr_assert_eq(atoi_base("999999", 10), 999999);
}

Test(atoi_base, base_16)
{
	uint32_t result;

	result = atoi_base("abcdef", 16);
	cr_assert_eq(result, 0xabcdef, "returned: %#x\n", result);
	cr_assert_eq(atoi_base("50926", 16), 0x50926);
	cr_assert_eq(atoi_base("999999", 16), 0x999999);
}

Test(atoi_base, base_error)
{
	cr_assert_eq(atoi_base("xxxxxx", 16), 0);
	cr_assert_eq(atoi_base("50926x56", 16), 0x50926);
	cr_assert_eq(atoi_base("o", 16), 0);
}
