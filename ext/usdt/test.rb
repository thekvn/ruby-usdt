require File.expand_path(File.dirname(__FILE__), 'usdt')
require 'pp'

provider = USDT::Provider.create :ruby, :test
pp provider.inspect

p1 = provider.probe("testrb", "p1",
                    :string, :string, :string,
                    :integer, :integer, :integer)

p2 = provider.probe("testrb", "p2",
                    :integer, :integer, :integer, :integer)

r = provider.enable
puts r.inspect

while true
  if p1.enabled?
    puts "fire p1"
    puts p1.fire("one", "two", "three", 1, 2, 3)
  end

  if p2.enabled?
    puts "fire p2"
    puts p2.fire( 1, 2, 3, 4)
  end
  sleep 0.5
end
