require "minitest/autorun"
require "win_print"

class WinPrintTest < Minitest::Test
  def test_printer_names
    # Just run the method and see if it dies (or hopefully not).
    printers = WinPrint.get_printer_names
    assert !printers.nil? # Should be an array.
  end
  
  def test_printing_on_non_existing
    # Try to print on a non-existing printer and verify, if we get the right exception.
    exception = assert_raises RuntimeError do
      WinPrint.raw_print("A printer with this name should definitely not exist.", "Some data.", "And a job name")
    end
    assert_includes exception.message, "[1801]"
  end
end
