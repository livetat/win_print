require "mkmf"

abort "No winspool" unless have_library "winspool"
$LDFLAGS = '-lwinspool'

create_makefile "extension"
