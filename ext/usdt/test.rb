require File.expand_path(File.dirname(__FILE__)) + '/usdt'

puts "## Provider"
provider = USDT::Provider.create :ruby, :test
puts provider

puts
puts "## Probe with sym definition"
probe = provider.probe(:myfn, :probe,
                   :string, :string, :integer)
puts probe
puts "probe.function: #{probe.function.inspect}"
puts "probe.name: #{probe.name.inspect}"
puts "probe.arguments: #{probe.arguments.inspect}"

puts
puts "## Probe with string definition"
probe_str = provider.probe('myfn', 'str', :string)
puts probe_str
puts "probe.function: #{probe_str.function.inspect}"
puts "probe.name: #{probe_str.name.inspect}"
puts "probe.arguments: #{probe_str.arguments.inspect}"

provider.enable

puts
puts "run:\nsudo dtrace -n 'ruby*:test:myfn:probe { printf(\"%s %s %d\",
  copyinstr(arg0),
  copyinstr(arg1),
  args[2])
}'"

while true
  if probe.enabled? 
    probe.fire("one", "two", 3)
    puts("Fired!")
  end
  sleep 0.5
end

