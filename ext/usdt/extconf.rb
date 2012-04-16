require 'mkmf'

have_library("dtrace", "dtrace_open", "/usr/include/dtrace.h")
system("cd ../libusdt && make")
system("cp ../libusdt/usdt.h ../libusdt/*.a ./")
have_library('usdt')
have_header('usdt.h')

create_makefile('usdt')

open("Makefile", "ab") do |mf|
  mf.puts
  mf.print "libusdt.a:\n\t cd ../libusdt && make && cp *.a usdt.h ../usdt/ \n"
end
