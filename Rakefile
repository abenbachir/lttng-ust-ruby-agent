# vim: syntax=ruby
# gem install rake-compiler
# gem install asciidoctor
require "rake/extensiontask"

Rake::ExtensionTask.new "lttng_ust_ruby_agent_ext" do |ext|
  ext.lib_dir = "lib"
end

require 'yaml'
require 'find'
require 'erb'
require 'date'
require 'mkmf'


config = YAML.load_file(File.join(__dir__, "project.yaml"))
project = config.fetch('name', File.split(File.expand_path(__dir__)).last)

version = "0.1"


task default: [:build]



task :build => [ :compile] do
  system "gem build #{project}.gemspec"
end

task :install => [:build] do
  system "gem install ./#{project}-#{version}.gem"
end

task :clean do
  rm_f "./#{project}-#{version}.gem"
  rm_f "./lib/#{project.gsub(/-/, '_')}_ext.so"
  rm_rf "tmp"
end
