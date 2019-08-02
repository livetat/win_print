#include "ruby/ruby.h"
#include "ruby/encoding.h"
#include "library.h"
#include "raw_print.h"

// Get printer names of all system printers.
static VALUE get_printer_names(VALUE self) {
  char errMsg[1024];
  VALUE arr = rb_ary_new();
  int success = GetPrinterNames(arr, errMsg);
  if (success == 0) {
    rb_enc_raise(rb_enc_find("CP1252"), rb_eRuntimeError, errMsg);
  }
  return arr;
}

// Send raw data to a system printer.
static VALUE raw_print(VALUE self, VALUE printer_name, VALUE data, VALUE doc_name) {
  Check_Type(printer_name, T_STRING);
  Check_Type(data, T_STRING);
  Check_Type(doc_name, T_STRING);

  char* pointer_printer_name = StringValueCStr(printer_name);
  char* pointer_data = RSTRING_PTR(StringValue(data));
  int data_len = RSTRING_LEN(StringValue(data));
  char* pointer_doc_name = StringValueCStr(doc_name);

  char errMsg[1024];
  int boolean_out = RawDataToPrinter( pointer_printer_name, pointer_data, data_len, pointer_doc_name, errMsg );
  if (boolean_out == 0) {
    rb_enc_raise(rb_enc_find("CP1252"), rb_eRuntimeError, errMsg);
  }
  return Qtrue;
}

// Register class methods:
//   WinPrint::Native.get_printer_names
//   WinPrint::Native.raw_print
void Init_extension(void) {
  VALUE WinPrint = rb_define_module("WinPrint");
  VALUE Native = rb_define_class_under(WinPrint, "Native", rb_cObject);
  rb_define_singleton_method(Native, "get_printer_names", get_printer_names, 0);
  rb_define_singleton_method(Native, "raw_print", raw_print, 3);
}
