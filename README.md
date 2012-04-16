# ruby-usdt

Ruby bindings for [libusdt](https://github.com/chrisa/libusdt).

### Install

    gem install ruby-usdt

### Usage

    require 'usdt'
    require 'pp'

    provider = USDT::Provider.create :myprovider, :test
    p1 = provider.probe("myfn", "probe1",
      "char *", "char *", "char *", 
      "char *", "char *", "char *")

    p2 = provider.probe("myfn", "probe2",
       "int", "int", "int",
      "int", "int", "int")

    provider.enable

### TODO (for first release)

- Firing Probes
- Allow passing :string and :integer for probe args

## License

The MIT License

Copyright (C) 2012 Kevin Chan

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
