# ruby-usdt

Native DTrace probes for ruby apps

Ruby bindings for [libusdt](https://github.com/chrisa/libusdt).

Applications, Libraries, and Frameworks can consume ruby-usdt to define USDT  
providers at runtime for instrumentation and analysis by the 
[DTrace](http://en.wikipedia.org/wiki/DTrace) framework.

### Install

    gem install ruby-usdt

When using Bundler to integrate ruby-usdt into a Ruby or Rails project,
you will need to specify that there are git submodule dependencies:

    gem "ruby-usdt", :submodules => true

### Usage

    # Provider.create <provider>, <module>
    #
    # Creates a probe
    #
    # returns USDT::Provider object
    provider = USDT::Provider.create :ruby, :mymod

    # Provider#probe *args
    #
    # args must be either :integer, or :string
    # max 6 args (dtrace provides this many), additional args will be ignored
    # returns USDT::Probe object
    p = provider.probe(:myfn, :probe, :string, :string, :integer)

    # Provider#enable
    # enables the probes defined with #probe
    provider.enable

    while true
      if p.enabled?
        p.fire("omg", "probe!!", 12345)
      end
      sleep 0.5
    end

    # see the probes in action with:
    # you should see continious output of the fire args above
    sudo dtrace -n 'ruby*:mymod:myfn:probe { printf("%s %s %d",
      copyinstr(arg0),
      copyinstr(arg1),
      args[2])
    }'

## Additional Resources

- [User-Level-Statically Defined Tracing](http://www.solarisinternals.com/wiki/index.php/DTrace_Topics_USDT#USDT)
- [DTrace Book](http://www.dtracebook.com/index.php/Main_Page)
- [Libusdt - Creating DTrace Providers at Runtime](http://chrisa.github.com/blog/2011/12/04/libusdt-runtime-dtrace-providers/)

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
