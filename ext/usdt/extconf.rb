require 'mkmf'

if have_library("dtrace", "dtrace_open", "/usr/include/dtrace.h")
  FileUtils.ln_sf('real.c', 'usdt.c')
  system("cd ../libusdt && make")
  system("cp ../libusdt/usdt.h ../libusdt/*.a ./")
  have_header('usdt.h')
  have_library('usdt')
  create_makefile('usdt', 'real')
else
  create_makefile('usdt', 'stubs')
end
