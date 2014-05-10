require 'usdt/stubs/probe'

module USDT
  class Provider
    attr_reader :provider, :mod

    def self.create(provider, mod)
      new(provider, mod)
    end

    def initialize(provider, mod)
      @provider = provider
      @mod = mod
    end

    def enable
      true
    end

    def disable
      true
    end

    def probe(*args)
      USDT::Probe.new(*args)
    end
  end
end
