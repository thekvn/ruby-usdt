require './usdt'
require 'pp'

provider = USDT::Provider.create :ruby, :test
pp provider.inspect

p1 = provider.probe("testrb", "p1", "char *", "char *", "char *", "char *", "char *", "char *")
pp p1.inspect

p2 = provider.probe("testrb", "p2", "int", "int", "int", "int", "int", "int")
pp p2.inspect

r = provider.enable
puts r.inspect

while true
  if p1.enabled?
    puts "fire p1"
    puts p1.fire("hello", "hello", "hello",
            "hello", "hello", "hello")
  end

  if p2.enabled?
    puts "fire p2"
    puts p2.fire(100, 200, 300,
            400, 500, 600)
  end
  sleep 1
end
