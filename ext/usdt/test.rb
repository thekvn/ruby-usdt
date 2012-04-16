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
  puts p1.enabled?
  puts p2.enabled?
  sleep 1
end
