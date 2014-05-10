module USDT
  class Probe
    attr_reader :function, :name, :arguments

    def initialize(function, name, *args)
      @function = function.to_sym
      @name = name.to_sym
      @arguments = args
    end

    def enabled?
      false
    end

    def fire(*args)
      false
    end
  end
end
