require 'spec_helper'
require 'usdt/stubs'

describe USDT::Provider do
  let(:provider) { USDT::Provider.create :ruby, :test }

  describe 'initialization' do
    it 'sets provider' do
      expect(provider.provider).to eq(:ruby)
    end

    it 'sets module' do
      expect(provider.mod).to eq(:test)
    end
  end

  describe 'enable' do
    it 'succeeds' do
      expect(provider.enable).to be true
    end
  end

  describe 'disable' do
    it 'succeeds' do
      expect(provider.disable).to be true
    end
  end

  describe 'probe' do
    let(:probe) { provider.probe(:fn, :name, :string, :string) }

    it 'creates a probe' do
      expect(probe).to be_a(USDT::Probe)
    end

    it 'sets probe function' do
      expect(probe.function).to eq(:fn)
    end

    it 'sets probe name' do
      expect(probe.name).to eq(:name)
    end

    it 'sets probe arguments' do
      expect(probe.arguments).to eq([:string, :string])
    end
  end
end
