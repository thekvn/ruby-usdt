require 'bundler/gem_tasks'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

namespace :ext do
  desc 'checkout git submodules'
  task :checkout do
    sh('git submodule update --init')
  end

  desc 'make clean in extensions'
  task :clean => :checkout do
    Dir.chdir('ext/libusdt') do
      sh('make clean')
    end
    Dir.chdir('ext/usdt') do
      sh('make clean')
      sh('rm Makefile')
      sh('rm *.a')
      sh('rm *.t sh')
    end
  end
end

task :release => 'ext:clean'
