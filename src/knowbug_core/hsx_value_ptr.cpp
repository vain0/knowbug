#include "pch.h"
#include "hsx.h"

void hsx_value_ptr_from_label_ptr(HsxValuePtr* value_ptr, HsxLabel const* label_ptr) {
	value_ptr->var_type = HSPVAR_FLAG_LABEL;
	value_ptr->data_ptr = (PDAT const*)label_ptr;
}

void hsx_value_ptr_from_str(HsxValuePtr* value_ptr, char const* str) {
	value_ptr->var_type = HSPVAR_FLAG_STR;
	value_ptr->data_ptr = (PDAT const*)str;
}

void hsx_value_ptr_from_double_ptr(HsxValuePtr* value_ptr, HsxDouble const* double_ptr) {
	value_ptr->var_type = HSPVAR_FLAG_DOUBLE;
	value_ptr->data_ptr = (PDAT const*)double_ptr;
}

void hsx_value_ptr_from_int_ptr(HsxValuePtr* value_ptr, HsxInt const* int_ptr) {
	value_ptr->var_type = HSPVAR_FLAG_INT;
	value_ptr->data_ptr = (PDAT const*)int_ptr;
}

void hsx_value_ptr_from_flex_ptr(HsxValuePtr* value_ptr, FlexValue const* flex_ptr) {
	value_ptr->var_type = HSPVAR_FLAG_STRUCT;
	value_ptr->data_ptr = (PDAT const*)flex_ptr;
}

HsxBool hsx_value_ptr_as_label(HsxValuePtr const* value_ptr, HsxLabel const** label_ptr_out) {
	auto ok = value_ptr != NULL && value_ptr->var_type == HSPVAR_FLAG_LABEL;

	if (label_ptr_out != NULL) {
		*label_ptr_out = ok ? (HsxLabel const*)value_ptr->data_ptr : nullptr;
	}
	return ok;
}

HsxBool hsx_value_ptr_as_str(HsxValuePtr const* value_ptr, char const** str_out) {
	auto ok = value_ptr != NULL && value_ptr->var_type == HSPVAR_FLAG_STR;

}

HsxBool hsx_value_ptr_as_int(HsxValuePtr const* value_ptr, HsxInt const** int_ptr_out);

HsxBool hsx_value_ptr_as_double(HsxValuePtr const* value_ptr, HsxDouble const** double_ptr_out);

HsxBool hsx_value_ptr_as_flex(HsxValuePtr const* value_ptr, FlexValue const** flex_ptr_ptr);
