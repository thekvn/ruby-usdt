require 'bundler/gem_tasks'

namespace :ext do
  desc 'checkout git submodules'
  task :checkout do
    sh('git submodule update --init')
  end

  desc 'make clean in extensions'
  task :clean => :checkout do
    sh('cd ext/libusdt && make clean')
    sh('cd ext/usdt && make clean')
  end
end

task :release => 'ext:clean'
