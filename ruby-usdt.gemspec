# -*- encoding: utf-8 -*-

lib = File.expand_path("../lib", __FILE__)
$:.unshift lib unless $:.include?(lib)

require 'usdt/version'

Gem::Specification.new do |s|
  s.name        = "ruby-usdt"
  s.version     = USDT::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Kevin Chan"]
  s.email       = ["kevin@yinkei.com"]
  s.homepage    = "http://github.com/kevinykchan/ruby-usdt"
  s.extensions  = ['ext/usdt/extconf.rb']
  s.summary     = "ruby bindings to libusdt"
  s.summary     = "ruby bindings to libusdt"
  s.files        = Dir.glob("{lib,ext}/**/*") + %w(README.md LICENSE.md)
  s.require_path = 'lib'
end
