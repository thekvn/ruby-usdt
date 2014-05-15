require 'spec_helper'
require 'usdt/stubs/probe'

describe USDT::Probe do
  let(:probe) { USDT::Probe.new(:fn, :name, :string, :integer) }

  describe 'initialization' do
    context 'with symbols' do
      let(:probe) { USDT::Probe.new(:fn, :name, :string, :integer) }

      it 'sets attributes' do
        expect(probe.function).to eq(:fn)
        expect(probe.name).to eq(:name)
        expect(probe.arguments).to eq([:string, :integer])
      end
    end

    context 'with strings' do
      let(:probe) { USDT::Probe.new('fn', 'name', :string, :integer) }

      it 'symbolizes attributes' do
        expect(probe.function).to eq(:fn)
        expect(probe.name).to eq(:name)
        expect(probe.arguments).to eq([:string, :integer])
      end
    end
  end

  describe 'enabled?' do
    it 'is false' do
      expect(probe.enabled?).to be false
    end
  end

  describe 'fire' do
    it 'is false' do
      expect(probe.fire('thing', 'blah', 0, 1)).to be false
    end
  end
end
