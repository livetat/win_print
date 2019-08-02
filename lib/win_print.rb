require "extension"
require "win_print/version"

# Ruby wrappers for native C functions.
# Here we ensure correct encodings before calling C.
module WinPrint
  # Get list of printer names available on the system.
  #
  # Example:
  #   >> WinPrint.get_printer_names
  #   => ["Microsoft XPS Document Writer", "Microsoft Print to PDF", "Fax"]

  def self.get_printer_names
    WinPrint::Native.get_printer_names.map{|pr| pr.force_encoding("CP1252").encode("UTF-8")}
  end

  # Send raw data to a system printer.
  #
  # Example:
  #   >> WinPrint.raw_print("Microsoft Print to PDF", "Some raw data.", "Test Job from WinPrint gem")
  #   => true
  #
  # Arguments:
  #   printer_name: (String)
  #   data: (String) raw data to be sent to the printer
  #   job_name: (String) will be displayed e.g. in the printer queue
  #
  # Note: For data, the encoding matters and the data will be sent as-is to the printer.
  # Will raise RuntimeError if something goes wrong.

  def self.raw_print(printer_name, data, job_name)
    WinPrint::Native.raw_print(printer_name.encode("CP1252"), data,
      job_name.encode("CP1252", invalid: :replace, undef: :replace, replace: ''))
  end
end
