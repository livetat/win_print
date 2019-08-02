# WinPrint Ruby Gem

Ruby gem for raw printing on windows.

The WinPrint library offers a way to send raw data to
a system printer on windows. It also allows to fetch the
printer list.

## Author

Martin Schuster

www.schul-logistik.de

## License

MIT

## Installation

    gem install win_print

## Example usage

    require 'win_print'

    # Get array of printer names:
    #
    WinPrint.get_printer_names
    # => ["Microsoft XPS Document Writer", "Microsoft Print to PDF", "Fax"]


    # Send raw data to a system printer.
    #
    # Arguments:
    #   printer_name: (String)
    #   data: (String) raw data to be sent to the printer
    #   job_name: (String) will be displayed e.g. in the printer queue
    #
    # Note: For data, the encoding matters and the data will be sent as-is to the printer.
    # Will raise RuntimeError if something goes wrong.
    #
    WinPrint.raw_print("Microsoft Print to PDF", "Some raw data.", "Test Job from WinPrint gem")
    # => true
