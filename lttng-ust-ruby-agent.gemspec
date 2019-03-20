require 'rake'
Gem::Specification.new do |s|
  s.name        = "lttng-ust-ruby-agent"
  s.version     = "0.1"
  s.licenses    = ["LGPL-3.0"]
  s.platform    = Gem::Platform::RUBY
  s.summary     = "Tracing ruby applications with LTTNG."
  s.description = "A simple API for sending tracepoints from Ruby programs to LTTNG."
  s.authors     = "Abderrahmane Benbachir"
  s.email       = "anis.benbachir@gmail.com"
  s.date        = "2019-01-01"
  s.files       = Rake::FileList["README.md", "project.yaml", "lib/**/*", "lttng-ust-ruby-agent.gemspec"].to_a

  s.homepage    = "https://github.com/abenbachir/lttng-ust-ruby-agent/"
  s.extensions  = ["ext/lttng_ust_ruby_agent/extconf.rb"]

  s.add_development_dependency "rake", "=12.3.2"
  s.add_development_dependency "yard", "=0.9.16"
  s.add_development_dependency "rspec", "=3.8.0"
end
