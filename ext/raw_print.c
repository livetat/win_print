#include <ruby/ruby.h>
#include <windows.h>
#include <stdio.h>

// This code is partially based on examples from here:
// https://support.microsoft.com/de-de/help/138594/howto-send-raw-data-to-a-printer-by-using-the-win32-api

// **********************************************************************
// GetErrorMessage - get message from error code information
//
// Params:
//   dwError       - the error code, usually from GetLastError()
//   lpString      - some caller-defined text to include with the error info
//   errMsg        - buffer to store the message in
//
// Returns: void
//
void GetErrorMessage( DWORD dwError, LPCTSTR lpString, char *errMsg )
{
#define MAX_MSG_BUF_SIZE 512
	TCHAR *msgBuf;
	DWORD	cMsgLen;

	cMsgLen = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, dwError,
				MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &msgBuf,
				MAX_MSG_BUF_SIZE, NULL );
	sprintf( errMsg, TEXT("%s Error [%d]: %s"), lpString, dwError, msgBuf );
	LocalFree( msgBuf );
#undef MAX_MSG_BUF_SIZE
}
// **********************************************************************

// **********************************************************************
// GetPrinterNames - return a ruby array with printer names
//
// Params:
//   arr           - ruby array to put the printer names in
//   errMsg        - Buffer to store the error message in
//
// Returns: TRUE for success, FALSE for failure.
//
BOOL GetPrinterNames(VALUE arr, char *errMsg) {
    BOOL success = TRUE;
    PRINTER_INFO_1* prninfo = NULL;
    DWORD needed, returned;

    // find required size for the buffer
    EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 1, NULL, 0, &needed, &returned);

    // allocate array of PRINTER_INFO structures
    prninfo = (PRINTER_INFO_1*) GlobalAlloc(GPTR, needed);

    // call again
    if (!EnumPrinters(PRINTER_ENUM_LOCAL, NULL,
        1, (LPBYTE) prninfo, needed, &needed, &returned)) {
          GetErrorMessage( GetLastError(), TEXT("EnumPrinters"), errMsg );
          success = FALSE;
    } else {
        for (DWORD i=0; i < returned; i++) {
            if ((prninfo[i].Flags & PRINTER_ENUM_ICON8) == PRINTER_ENUM_ICON8) {
                rb_ary_push(arr, rb_str_new2(prninfo[i].pName));
            }
        }
    }

    GlobalFree(prninfo);

    return success;
}
// **********************************************************************

// **********************************************************************
// RawDataToPrinter - sends binary data directly to a printer
//
// Params:
//   szPrinterName - NULL terminated string specifying printer name
//   lpData        - Pointer to raw data bytes
//   dwCount       - Length of lpData in bytes
//   docName       - Name of the document, displayed in the printer queue
//   errMsg        - Buffer to store the error message in
//
// Returns: TRUE for success, FALSE for failure.
//
BOOL RawDataToPrinter( LPTSTR szPrinterName, LPBYTE lpData, DWORD dwCount, LPTSTR docName, char *errMsg )
{
	HANDLE     hPrinter;
	DOC_INFO_1 DocInfo;
	DWORD      dwJob;
	DWORD      dwBytesWritten;

	// Need a handle to the printer.
	if( ! OpenPrinter( szPrinterName, &hPrinter, NULL ) )
	{
		GetErrorMessage( GetLastError(), TEXT("OpenPrinter"), errMsg );
		return FALSE;
	}

	// Fill in the structure with info about this "document."
	DocInfo.pDocName = docName;
	DocInfo.pOutputFile = NULL;
	DocInfo.pDatatype = TEXT("RAW");
	// Inform the spooler the document is beginning.
	if( (dwJob = StartDocPrinter( hPrinter, 1, (LPBYTE)&DocInfo )) == 0 )
	{
		GetErrorMessage( GetLastError(), TEXT("StartDocPrinter"), errMsg );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Start a page.
	if( ! StartPagePrinter( hPrinter ) )
	{
		GetErrorMessage( GetLastError(), TEXT("StartPagePrinter"), errMsg );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Send the data to the printer.
	if( ! WritePrinter( hPrinter, lpData, dwCount, &dwBytesWritten ) )
	{
		GetErrorMessage( GetLastError(), TEXT("WritePrinter"), errMsg );
		EndPagePrinter( hPrinter );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// End the page.
	if( ! EndPagePrinter( hPrinter ) )
	{
		GetErrorMessage( GetLastError(), TEXT("EndPagePrinter"), errMsg );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Inform the spooler that the document is ending.
	if( ! EndDocPrinter( hPrinter ) )
	{
		GetErrorMessage( GetLastError(), TEXT("EndDocPrinter"), errMsg );
		ClosePrinter( hPrinter );
		return FALSE;
	}
	// Tidy up the printer handle.
	ClosePrinter( hPrinter );
	// Check to see if correct number of bytes were written.
	if( dwBytesWritten != dwCount )
	{
		sprintf( errMsg, TEXT("Wrote %d bytes instead of requested %d bytes.\n"), dwBytesWritten, dwCount );
		return FALSE;
	}
	return TRUE;
}
// End RawDataToPrinter
// **********************************************************************
